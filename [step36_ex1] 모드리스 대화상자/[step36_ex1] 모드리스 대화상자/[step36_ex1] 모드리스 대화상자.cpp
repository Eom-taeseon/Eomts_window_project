// [step36_ex1] 모드리스 대화상자.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step36_ex1] 모드리스 대화상자.h"

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    HWND hDlg;
    hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
    ShowWindow(hDlg, nCmdShow);

    // 기본 메시지 루프
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN:
            SetWindowText(hDlg, _T("VK_RETURN, DOWN"));
            break;
        case VK_SPACE:
            SetWindowText(hDlg, _T("VK_SPACE, Down"));
            break;
        case VK_LEFT:
            SetWindowText(hDlg, _T("VK_LEFT, DOWN"));
            break;
        case 'A':
            SetWindowText(hDlg, _T("A, DOWN"));
            break;
        }
        return TRUE;

    case WM_KEYUP:
        switch (wParam)
        {
        case VK_RETURN:
            SetWindowText(hDlg, _T("VK_RETURN, UP"));
            break;
        case VK_SPACE:
            SetWindowText(hDlg, _T("VK_SPACE, UP"));
            break;
        case VK_LEFT:
            SetWindowText(hDlg, _T("VK_LEFT, UP"));
            break;
        case 'A':
            SetWindowText(hDlg, _T("A, UP"));
            break;
        }
        return true;

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDOK:
            break;
        case IDCANCEL:
            DestroyWindow(hDlg);
            break;
        }
        return true;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return true;
    }
    return false;
}