// [step37_ex1] 서브클래싱 1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step37_ex1] 서브클래싱 1.h"

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WNDPROC OldEditProc;                            // 컨트롤의 원래 프로시저 저장

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK EditSubClassProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    hInst = hInstance;
    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
    return 0;
}

// 정보 대화상자의 메시지 처리기입니다.
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hEdit;

    switch (message)
    {
    case WM_INITDIALOG:
    {
        hEdit = GetDlgItem(hDlg, IDC_EDIT1);
        SetFocus(hEdit);
        OldEditProc = (WNDPROC)SetWindowLong(hEdit, GWLP_WNDPROC, (LONG)EditSubClassProc);  // hEidt의 WndProc를 EditSubClassProc 함수로 서브클래싱 + 이전 WndProc는 OlgEditProc에 저장
        return true;
    }

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            break;
        }
        return true;
    }
    }
    return false;
}

// 서브클래스 프로시저
INT_PTR CALLBACK EditSubClassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    //case WM_GETDLGCODE: // 주석 처리하고 실행해볼 것
    //    return DLGC_WANTALLKEYS;    // 모든 키보드 입력을 WndProc EditSubClassProc에서 처리할 것을 알리는 DLGC_WANTALLKEYS를 반환
    case WM_CHAR:
    {
        UINT nChar = (UINT)wParam;

        if (    // 입력키가 숫자, back, tab, space, return(=enter)가 아니면 
            (nChar < '0' || nChar>'9') &&
            nChar != VK_BACK && nChar != VK_TAB &&
            nChar != VK_SPACE && nChar != VK_RETURN
        )
        {
            return MessageBeep(0);  // 비프음 출력
        }
        break;
    }
    }
    return CallWindowProc((WNDPROC)OldEditProc, hWnd, message, wParam, lParam);
}