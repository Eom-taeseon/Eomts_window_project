// [step40_ex1] 더블 버퍼링 비트맵을 BMP 파일에 저장.cpp : 애플리케이션에 대한 진입점을 정의합니다.
// ref : [step17_ex1] 더블 버퍼링 1
//

#include "framework.h"
#include "[step40_ex1] 더블 버퍼링 비트맵을 BMP 파일에 저장.h"

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    hInst = hInstance;
    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
    return 0;
}

// ref: https://learn.microsoft.com/ko-kr/windows/win32/gdi/capturing-an-image
void SaveBitmap2File(HWND hWnd, LPCTSTR fileName)
{
    // #1 : 화면 hdc -> memDC -> hBit
    HDC hdc = GetDC(hWnd);
    HDC memDC = CreateCompatibleDC(hdc);
    RECT rtClient;
    GetClientRect(hWnd, &rtClient);
    HBITMAP hBit = CreateCompatibleBitmap(hdc, rtClient.right, rtClient.bottom);
    SelectObject(memDC, hBit);
    BitBlt(memDC, 0, 0, rtClient.right, rtClient.bottom,
        hdc, 0, 0, SRCCOPY); // hdc -> memDC

    // #2 : 비트맵 정보
    BITMAP bmp;
    GetObject(hBit, sizeof(BITMAP), &bmp);

    BITMAPINFOHEADER bmlh;
    bmlh.biSize = sizeof(BITMAPINFOHEADER);
    bmlh.biWidth = bmp.bmWidth;
    bmlh.biHeight = bmp.bmHeight;
    bmlh.biPlanes = 1;
    bmlh.biBitCount = bmp.bmBitsPixel; // 32
    bmlh.biCompression = BI_RGB; // uncompressed format
    bmlh.biSizeImage = 0;
    bmlh.biXPelsPerMeter = 0;
    bmlh.biYPelsPerMeter = 0;
    bmlh.biClrUsed = 0;
    bmlh.biClrImportant = 0;

    // #3: 메모리할당, 비트맵 데이터 읽기
    // 가로 폭이 4의 배수가 되어야 함
    DWORD dwBmpSize =
        ((bmlh.biWidth * bmlh.biBitCount + 31) / 32) * 4 * bmlh.biHeight;
    LPBYTE lpBits = (LPBYTE)LocalAlloc(LPTR, dwBmpSize);

    // hBit에서 lpBits로 복사, GetDC(NULL)
    GetDIBits(hdc, hBit, 0, (UINT)bmp.bmHeight, lpBits,
        (BITMAPINFO*)&bmlh, DIB_RGB_COLORS);
    // GetBitmapBits(hBit, dwBmpSize, lpBits);  // Upside down

    // #4: 파일 헤더 생성
    BITMAPFILEHEADER bmFh;
    ZeroMemory(&bmFh, sizeof(BITMAPFILEHEADER));
    bmFh.bfType = 0x4D42;   // BM

    // True color, 헤더 크기 + 비트맵 크기
    DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmFh.bfSize = dwSizeofDIB;  // BMP 파일 크기

    // 비트맵 Offset, True Color, RGB 팔레트 없음
    bmFh.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

    // #5: 파일 저장
    HANDLE hFile = CreateFile(fileName, GENERIC_WRITE, 0,
        NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD dwBytesWritten = 0;
    WriteFile(hFile, (LPBYTE)&bmFh, sizeof(BITMAPFILEHEADER),
        &dwBytesWritten, NULL);
    WriteFile(hFile, (LPBYTE)&bmlh, sizeof(BITMAPFILEHEADER),
        &dwBytesWritten, NULL);
    WriteFile(hFile, (LPBYTE)lpBits, dwBmpSize, &dwBytesWritten, NULL);

    // #6: 메모리 핸들 해제
    LocalFree(lpBits);
    CloseHandle(hFile);
    DeleteDC(memDC);
    DeleteObject(hBit);
    ReleaseDC(hWnd, hdc);
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static RECT rtClient;
    static HDC memDC;
    static HBITMAP hBit;

    switch (message)
    {
    case WM_INITDIALOG:
    {
        return TRUE;
    }

    case WM_SIZE:
    {
        GetClientRect(hDlg, &rtClient);
        HDC hdc = GetDC(hDlg);
        if (hBit)
            DeleteObject(hBit);
        hBit = CreateCompatibleBitmap(hdc, rtClient.right, rtClient.bottom);

        if (memDC)
            DeleteDC(memDC);
        memDC = CreateCompatibleDC(hdc);
        ReleaseDC(hDlg, hdc);
        SelectObject(memDC, hBit);
        return TRUE;
    }

    case WM_LBUTTONDOWN:
    {
        SaveBitmap2File(hDlg, _T("test.bmp"));
        return true;
    }

    case WM_ERASEBKGND:
        return true;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hDlg, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드 추가
        // 배경을 브러시로 채워 지우기
        FillRect(memDC, &rtClient, (HBRUSH)(COLOR_WINDOW + 2));

        HPEN hPen = CreatePen(PS_INSIDEFRAME, 10, RGB(255, 0, 0));
        HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
        HPEN hOldPen = (HPEN)SelectObject(memDC, hPen);
        HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, hBrush);

        Rectangle(memDC, 0, 0, 400, 200);

        SelectObject(memDC, hOldPen);
        SelectObject(memDC, oldBrush);
        DeleteObject(hPen);
        DeleteObject(hBrush);

        BitBlt(hdc, 0, 0, rtClient.right, rtClient.bottom,
            memDC, 0, 0, SRCCOPY);  // memDC -> hdc

        EndPaint(hDlg, &ps);
        return true;
    }

    case WM_CLOSE:
        DestroyWindow(hDlg);
        return true;

    case WM_DESTROY:
        DeleteDC(memDC);
        DeleteObject(hBit);
        return true;
    }
    return false;
}