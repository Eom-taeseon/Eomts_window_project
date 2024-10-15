// [step38_ex2] 저수준 키보드 훅(WH_KEYBOARD_LL).cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step38_ex2] 저수준 키보드 훅(WH_KEYBOARD_LL).h"

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HHOOK hhk; // 훅 핸들

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK LowLevelKeyboardProc(int, WPARAM, LPARAM);

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
    switch (message)
    {
    case WM_INITDIALOG:
    {
        // dwThreadId = 0으로 global hook 설치
        hhk = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
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
        return (INT_PTR)TRUE;
    }

    case WM_DESTROY:
        UnhookWindowsHookEx(hhk);
        return true;
    }
    return false;
}

LRESULT CALLBACK LowLevelKeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    HWND hWnd = GetActiveWindow();  // 현재 활성 윈도우 핸들
    if (code < 0)
        return CallNextHookEx(hhk, code, wParam, lParam);

    PKBDLLHOOKSTRUCT pkb = (PKBDLLHOOKSTRUCT)lParam;

    switch (wParam)
    {
    case WM_KEYDOWN:
        switch (pkb->vkCode)
        {
        case VK_ESCAPE:
            SetWindowText(hWnd, _T("VK_ESCAPE, DOWN"));
            break;
        case VK_SPACE:
            SetWindowText(hWnd, _T("VK_SPACE, DOWN"));
            break;
        case VK_RETURN:
            SetWindowText(hWnd, _T("VK_RETURN, DOWN"));
            break;
        }
        return true;

    case WM_KEYUP:
        switch (pkb->vkCode)
        {
        case VK_ESCAPE:
            SetWindowText(hWnd, _T("VK_ESCAPE, UP"));
            break;
        case VK_SPACE:
            SetWindowText(hWnd, _T("VK_SPACE, UP"));
            break;
        case VK_RETURN:
            SetWindowText(hWnd, _T("VK_RETURN, UP"));
            break;
        }
        return true;    
    }
    return CallNextHookEx(hhk, code, wParam, lParam);
}