// [step33_ex2]체크 박스 컨트롤.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step33_ex2]체크 박스 컨트롤.h"

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

// 정보 대화 상자의 메시지 처리기
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static BOOL bRect = TRUE;
    static HWND hC1;
    switch (message)
    {
    case WM_INITDIALOG:
        hC1 = GetDlgItem(hDlg, IDC_CHECK1);
        SendMessage(hC1, BM_SETCHECK, BST_CHECKED, 0);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_CHECK1:    // hC1 == (HWND)lParam;
            bRect = SendMessage(hC1, BM_GETCHECK, 0, 0) == BST_CHECKED;
            InvalidateRect(hDlg, NULL, TRUE);
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
        if (bRect)
            Rectangle(hdc, 150, 50, 400, 400);
        else
            Ellipse(hdc, 150, 50, 400, 400);
        EndPaint(hDlg, &ps);
    }
    return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}