// [step33_ex4]비트맵 푸시 버튼 컨트롤.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step33_ex4]비트맵 푸시 버튼 컨트롤.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

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

// 정보 대화상자의 메시지 처리기
INT_PTR CALLBACK DlgProc(
    HWND hDlg,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{
    HWND hC1, hC2;
    static HBITMAP hBitmap1, hBitmap2;
    static int graph = IDC_BUTTON1;

    switch (message)
    {
    case WM_INITDIALOG:
    {
        hC1 = GetDlgItem(hDlg, IDC_BUTTON1);
        hC2 = GetDlgItem(hDlg, IDC_BUTTON2);

        // hC1에 생성한 비트맵 설정
        RECT rt;
        GetClientRect(hC1, &rt);
        HDC hDC = GetDC(hDlg);
        HDC memDC = CreateCompatibleDC(hDC);
        hBitmap1 = CreateCompatibleBitmap(hDC, rt.right, rt.bottom);
        SelectObject(memDC, hBitmap1);
        SetDCBrushColor(memDC, RGB(0, 0, 255));

        SetRect(&rt, rt.left + 5, rt.top + 5, rt.right - 5, rt.bottom - 5);
        FillRect(memDC, &rt, (HBRUSH)GetStockObject(DC_BRUSH));

        SetBkMode(memDC, TRANSPARENT);
        SetTextColor(memDC, RGB(255, 0, 0));
        DrawText(memDC, _T("사각형"), -1, &rt,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        DeleteDC(memDC);
        ReleaseDC(hDlg, hDC);
        SendMessage(hC1, BM_SETIMAGE,
            (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap1);

        // hC2에 로드한 비트맵 설정
        hBitmap2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
        SendMessage(hC2, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap2);

        return (INT_PTR)TRUE;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON1:
            graph = IDC_BUTTON1;
            InvalidateRect(hDlg, NULL, TRUE);
            break;

        case IDC_BUTTON2:
            graph = IDC_BUTTON2;
            InvalidateRect(hDlg, NULL, TRUE);
            break;

        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        return (INT_PTR)TRUE;

    case WM_PAINT:
        {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hDlg, &ps);
        // TODO : 여기에 hdc를 사용하는 그리기 코드 추가
        if (graph == IDC_BUTTON1)
            Rectangle(hdc, 150, 50, 400, 400);
        else
            Ellipse(hdc, 150, 50, 400, 400);
        EndPaint(hDlg, &ps);
        }
        return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}