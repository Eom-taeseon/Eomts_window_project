// [step33_ex5] 에디트 컨트롤.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step33_ex5] 에디트 컨트롤.h"

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
    static HWND hAge, hName;
    static int nAge = 21;
    static TCHAR szName[128] = _T("홍길동");
    static TCHAR szAge[128];

    switch (message)
    {
    case WM_INITDIALOG:
        {
            hAge = GetDlgItem(hDlg, IDC_AGE);
            hName = GetDlgItem(hDlg, IDC_NAME);

            SetDlgItemInt(hDlg, IDC_AGE, nAge, FALSE);
            SetDlgItemText(hDlg, IDC_NAME, szName);
            wsprintf(szAge, _T("%d"), nAge);
            return (INT_PTR)TRUE;
        }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_AGE:
            if (HIWORD(wParam) == EN_CHANGE)
            {
                // TCHAR str[128];
                // GetWindowText(hAge, str, 128);
                // nAge = _wtoi(str);
                //

                nAge = GetDlgItemInt(hDlg, IDC_AGE, NULL, FALSE);
                wsprintf(szAge, _T("%d"), nAge);
                InvalidateRect(hDlg, NULL, TRUE);
            }
            break;

        case IDC_NAME:
            if (HIWORD(wParam) == EN_CHANGE)
            {
                // GetWindowText(hName, szName, 128);
                GetDlgItemText(hDlg, IDC_NAME, szName, 128);
                InvalidateRect(hDlg, NULL, TRUE);
            }
            break;

        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            break;
        }
        return (INT_PTR)TRUE;
        
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hDlg, &ps);
        // TODO : 여기에 hdc를 사용하는 그리기 코드 추가
        TextOut(hdc, 20, 120, szAge, lstrlen(szAge));
        TextOut(hdc, 20, 140, szName, lstrlen(szName));
        EndPaint(hDlg, &ps);
    }
    return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}