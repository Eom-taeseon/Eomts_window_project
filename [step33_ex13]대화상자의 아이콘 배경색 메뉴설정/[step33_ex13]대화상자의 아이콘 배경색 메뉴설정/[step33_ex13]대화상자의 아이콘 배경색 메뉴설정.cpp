// [step33_ex13]대화상자의 아이콘 배경색 메뉴설정.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step33_ex13]대화상자의 아이콘 배경색 메뉴설정.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
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
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hC1, hC2;
    static HBRUSH hBrush;
    static HICON hIcon1, hIcon2;
    
    switch (message)
    {
    case WM_INITDIALOG:
    {
        hBrush = CreateSolidBrush(RGB(0, 0, 255));
        hIcon1 = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));
        hIcon2 = LoadIcon(hInst, MAKEINTRESOURCE(IDI_STEP33EX13));
        SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon1);
        SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon2);
        return (INT_PTR)TRUE;
    }

    case WM_CTLCOLORDLG:    // 대화 상자의 배경색 브러시
    {
        return (INT_PTR)hBrush;
    }

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, About);
            break;

        case IDM_EXIT:
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            break;
        }
        return (INT_PTR)TRUE;
    }

    case WM_DESTROY:
        DeleteObject(hBrush);
        DeleteObject(hIcon1);
        DeleteObject(hIcon2);
        return (INT_PTR)TRUE;
    }

    return (INT_PTR)FALSE;
}

// 정보 대화상자의 메시지 처리기
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}