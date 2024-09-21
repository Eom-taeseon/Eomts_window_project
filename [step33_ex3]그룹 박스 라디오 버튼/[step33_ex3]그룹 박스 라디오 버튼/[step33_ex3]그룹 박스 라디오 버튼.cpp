// [step33_ex3]그룹 박스 라디오 버튼.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step33_ex3]그룹 박스 라디오 버튼.h"

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

INT_PTR CALLBACK DlgProc(
    HWND hDlg,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{
    static HWND hR[8];
    static POINT pts[2] = { { 20, 250}, {300, 400} };
    static int penType = IDC_BLACK_PEN;
    static int brushType = IDC_NULL_BRUSH;

    switch (message)
    {
    case WM_INITDIALOG:
        for (int i = 0; i < 8; i++)
            hR[i] = GetDlgItem(hDlg, IDC_NULL_PEN + i);
        SendMessage(hR[1], BM_SETCHECK, BST_CHECKED, 0);
        SendMessage(hR[5], BM_SETCHECK, BST_CHECKED, 0);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_NULL_PEN:
            penType = IDC_NULL_PEN;
            break;

        case IDC_BLACK_PEN:
            penType = IDC_BLACK_PEN;
            break;

        case IDC_RED_PEN:
            penType = IDC_RED_PEN;
            break;

        case IDC_BLUE_PEN:
            penType = IDC_BLUE_PEN;
            break;

        case IDC_NULL_BRUSH:
            brushType = IDC_NULL_BRUSH;
            break;

        case IDC_BLACK_BRUSH:
            brushType = IDC_BLACK_BRUSH;
            break;

        case IDC_RED_BRUSH:
            brushType = IDC_RED_BRUSH;
            break;

        case IDC_BLUE_BRUSH:
            brushType = IDC_BLUE_BRUSH;
            break;

        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            break;
        }
        InvalidateRect(hDlg, NULL, TRUE);
        return (INT_PTR)TRUE;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hDlg, &ps);
        // TODO : 여기에 hdc를 사용하는 그리기 코드 추가
        HPEN hPen = NULL;
        HBRUSH hBrush = NULL;

        switch (penType) {
        case IDC_NULL_PEN:
            hPen = (HPEN)GetStockObject(NULL_PEN);
            break;

        case IDC_BLACK_PEN:
            hPen = (HPEN)CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
            break;

        case IDC_RED_PEN:
            hPen = (HPEN)CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
            break;

        case IDC_BLUE_PEN:
            hPen = (HPEN)CreatePen(PS_SOLID, 4, RGB(0, 0, 255));
            break;
        }

        switch (brushType) {
        case IDC_NULL_BRUSH:
            hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
            break;

        case IDC_BLACK_BRUSH:
            //hBrush = CreateSolidBrush(RGB(0, 0, 0));
            hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
            break;

        case IDC_RED_BRUSH:
            hBrush = CreateSolidBrush(RGB(244, 0, 0));
            break;

        case IDC_BLUE_BRUSH:
            hBrush = CreateSolidBrush(RGB(0, 0, 255));
            break;
        }

        HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        Rectangle(hdc, pts[0].x, pts[0].y, pts[1].x, pts[1].y);

        SelectObject(hdc, oldPen);
        DeleteObject(hPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(hBrush);
        EndPaint(hDlg, &ps);
    }
    return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}