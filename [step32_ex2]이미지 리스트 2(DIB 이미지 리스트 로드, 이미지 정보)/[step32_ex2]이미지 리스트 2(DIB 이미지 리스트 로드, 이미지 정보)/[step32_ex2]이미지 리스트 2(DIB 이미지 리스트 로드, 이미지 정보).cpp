// [step32_ex2]이미지 리스트 2(DIB 이미지 리스트 로드, 이미지 정보).cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step32_ex2]이미지 리스트 2(DIB 이미지 리스트 로드, 이미지 정보).h"

#include "CommCtrl.h"
#pragma comment(lib, "Comctl32.lib")
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

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HIMAGELIST hIL;
    static int k;   // k=0
    switch (message)
    {
    case WM_INITDIALOG:
    {
        InitCommonControls();   // 공용 컨트롤 DLL 로드 확인
        hIL = ImageList_LoadImage(hInst,
            MAKEINTRESOURCE(IDB_BITMAP1), 512, 1, CLR_NONE,
            IMAGE_BITMAP, LR_CREATEDIBSECTION);

        /*hIL = ImageList_LoadImage(hInst,
            _T("sampleImage.bmp"), 512, 1, CLR_NONE,
            IMAGE_BITMAP, LR_LOADFROMFILE | LR_CREATEDIBSECTION);*/

        IMAGEINFO info;
        ImageList_GetImageInfo(hIL, 0, &info);  // 0 이미지 정보
        //BITMAP bm;
        //GetObject(info.hbmImage, sizeof(BITMAP), &bm);

        RECT rc = info.rcImage;
        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, TRUE);
        int nWidth = rc.right - rc.left;
        int nHeight = rc.bottom - rc.top;
        SetWindowPos(hDlg, NULL, 0, 0, nWidth, nHeight, SWP_NOMOVE);
        return (INT_PTR)TRUE;
    }

    case WM_KEYDOWN:
        if (wParam == VK_SPACE)
        {
            k++;
            if (k >= 4) k = 0;
            InvalidateRect(hDlg, NULL, TRUE);
        }
        return TRUE;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hDlg, &ps);
        // TODO : 여기에 hdc를 사용하는 그리기 코드를 추가
        ImageList_Draw(hIL, k, hdc, 0, 0, ILD_NORMAL);
        EndPaint(hDlg, &ps);
        return TRUE;
    }

    case WM_CLOSE:
        ImageList_Destroy(hIL);
        DestroyWindow(hDlg);    // EndDialog(hDlg, TRUE);
        return TRUE;

    case WM_DESTROY:
        return TRUE;
    }
    return FALSE;
}