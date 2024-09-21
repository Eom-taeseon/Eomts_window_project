// [step33_ex11]푸시 버튼 오너 드로우 1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step33_ex11]푸시 버튼 오너 드로우 1.h"

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
    static HWND hC1, hC2;
    static HBRUSH hBrush1, hBrush2;
    static int graph = IDC_BUTTON1;

    switch (message)
    {
    case WM_INITDIALOG:
    {
        hC1 = GetDlgItem(hDlg, IDC_BUTTON1);
        hC2 = GetDlgItem(hDlg, IDC_BUTTON2);
        hBrush1 = CreateSolidBrush(RGB(0, 0, 255));
        hBrush2 = CreateSolidBrush(RGB(0, 255, 0));
        return (INT_PTR)TRUE;
    }

    case WM_CTLCOLORBTN:
    {
        HWND ctrlWnd = (HWND)lParam;
        HDC hdc = (HDC)wParam;
        RECT rt;
        GetClientRect(ctrlWnd, &rt);

        if (ctrlWnd == hC1) {
            SetTextColor(hdc, RGB(255, 0, 0));
            SetBkMode(hdc, TRANSPARENT);
            DrawText(hdc, _T("사각형"), -1, &rt,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            return (INT_PTR)hBrush1;
        }

        else { //hC2
            SetTextColor(hdc, RGB(0, 0, 255));
            SetBkMode(hdc, TRANSPARENT);
            DrawText(hdc, _T("타원"), -1, &rt,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            return (INT_PTR)hBrush2;
        }
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
    {
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
            break;
        }
        return (INT_PTR)TRUE;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hDlg, &ps);
        // TODO : 여기에 hdc를 사용하는 그르기 코드 추가
        if (graph == IDC_BUTTON1)
            Rectangle(hdc, 150, 50, 400, 400);
        else
            Ellipse(hdc, 150, 50, 400, 400);

        EndPaint(hDlg, &ps);
    }
    break;

    case WM_DESTROY:
        DeleteObject(hBrush1);
        DeleteObject(hBrush2);
        return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}