// [step33_ex8]콤보 박스.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step33_ex8]콤보 박스.h"

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
LPCTSTR items[] = { _T("직선"), _T("타원"), _T("사각형") };
enum { LINE, ELLIPSE, RECTAGLE };

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hCombo;
    static int graphType = LINE;
    static POINT pts[] = { {200, 50}, {300, 300} };

    switch (message)
    {
    case WM_INITDIALOG:
    {
        hCombo = GetDlgItem(hDlg, IDC_COMBO1);
        SetFocus(hCombo);
        
        for (int i = 0; i < 3; i++)
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)items[i]);
        SendMessage(hCombo, CB_SETCURSEL, LINE, 0);
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
    {
        int i;
        TCHAR str[128];
        switch (LOWORD(wParam))
        {
        case IDC_COMBO1:
            switch (HIWORD(wParam))
            {
            case CBN_SELCHANGE:
                graphType = (int)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0); // lParam == hCombo
                SendMessage((HWND)lParam, CB_GETLBTEXT, graphType, (LPARAM)str);
                SetWindowText(hDlg, str);
                InvalidateRect(hDlg, NULL, TRUE);
                break;

            case CBN_EDITCHANGE:
                GetWindowText((HWND)lParam, str, 128);
                // CB_FINDSTRING, CB_FINDSTRINGEXACT 메시지
                i = (int)SendMessage((HWND)lParam, CB_SELECTSTRING, 1, (LPARAM)str);

                if (i != CB_ERR)
                {
                    graphType = i;
                    InvalidateRect(hDlg, NULL, TRUE);
                }
                SetWindowText(hDlg, str);
                break;
            }
            break;
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            break;
        }
        return (INT_PTR)TRUE;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hDlg, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드 추가
        switch (graphType)
        {
        case LINE:
            MoveToEx(hdc, pts[0].x, pts[0].y, NULL);
            LineTo(hdc, pts[1].x, pts[1].y);
            break;
        case RECTAGLE:
            Rectangle(hdc, pts[0].x, pts[0].y, pts[1].x, pts[1].y);
            break;
        case ELLIPSE:
            Ellipse(hdc, pts[0].x, pts[0].y, pts[1].x, pts[1].y);
            break;
        }
        EndPaint(hDlg, &ps);
    }
    return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}