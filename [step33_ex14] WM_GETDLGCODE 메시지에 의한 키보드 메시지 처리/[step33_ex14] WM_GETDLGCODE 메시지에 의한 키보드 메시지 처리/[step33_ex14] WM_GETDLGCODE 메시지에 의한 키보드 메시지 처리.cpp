// [step33_ex14] WM_GETDLGCODE 메시지에 의한 키보드 메시지 처리.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step33_ex14] WM_GETDLGCODE 메시지에 의한 키보드 메시지 처리.h"

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
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_GETDLGCODE: // 주성으로 처리하고 실행해볼 것
    {
        if (lParam)
        {
            LPMSG pMsg = (LPMSG)lParam;
            // if (pMsg->message == WM_KEYDOWN)
            {
                SendMessage(hDlg, pMsg->message, pMsg->wParam, pMsg->lParam);
                return DLGC_WANTMESSAGE;
            }
        }
        break;
    }
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN:
            SetWindowText(hDlg, _T("VK_RETURN, Down"));
            break;

        case VK_SPACE:
            SetWindowText(hDlg, _T("VK_SPACE, Down"));
            break;

        case VK_LEFT:
            SetWindowText(hDlg, _T("VK_LEFT, Down"));
            break;

        case 'A':
            SetWindowText(hDlg, _T("A, Down"));
            break;
        }
        return TRUE;

    case WM_KEYUP:
        switch (wParam) {
        case VK_RETURN:
            SetWindowText(hDlg, _T("VK_RETURN, Up"));
            break;

        case VK_SPACE:
            SetWindowText(hDlg, _T("VK_SPACE, Up"));
            break;

        case VK_LEFT:
            SetWindowText(hDlg, _T("VK_LEFT, UP"));
            break;

        case 'A':
            SetWindowText(hDlg, _T("A, UP"));
            break;
        }
        return TRUE;

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDOK:
            break;
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            break;
        }
        return (INT_PTR)TRUE;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return TRUE;
    }
    return FALSE;
}