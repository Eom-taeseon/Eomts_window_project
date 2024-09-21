// [step33_ex9]정적 컨트롤.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step33_ex9]정적 컨트롤.h"

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
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int nCount = 10;
    static BOOL bTimer = FALSE;

    switch (message)
    {
    case WM_INITDIALOG:
    {
        SetDlgItemInt(hDlg, IDC_COUNTER, nCount, FALSE);
        return (INT_PTR)TRUE;
    }

    case WM_TIMER:
        nCount--;
        SetDlgItemInt(hDlg, IDC_COUNTER, nCount, FALSE);
        if (nCount == 0)
            SendDlgItemMessage(hDlg, IDC_BUTTON2, BM_CLICK, 0, 0);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON1:
            if (bTimer) {
                bTimer = FALSE;
                KillTimer(hDlg, 1);
                SetDlgItemText(hDlg, IDC_BUTTON1, _T("START"));
            }
            else {
                bTimer = TRUE;
                SetTimer(hDlg, 1, 1000, NULL);
                SendMessage(hDlg, WM_TIMER, 1, 0);
                SetDlgItemText(hDlg, IDC_BUTTON1, _T("Stop"));
            }
            break;

        case IDC_BUTTON2:
            bTimer = FALSE;
            nCount = 10;
            KillTimer(hDlg, 1);
            SetDlgItemInt(hDlg, IDC_COUNTER, nCount, FALSE);
            SetDlgItemText(hDlg, IDC_BUTTON1, _T("Start"));
            break;
            
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            break;
        }
        return (INT_PTR)TRUE;
    }
    case WM_DESTROY:
        if (bTimer)
            KillTimer(hDlg, 1);
        return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}