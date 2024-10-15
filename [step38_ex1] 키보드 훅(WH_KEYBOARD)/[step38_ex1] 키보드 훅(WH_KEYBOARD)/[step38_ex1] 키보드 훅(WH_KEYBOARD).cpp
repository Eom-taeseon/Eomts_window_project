// [step38_ex1] 키보드 훅(WH_KEYBOARD).cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step38_ex1] 키보드 훅(WH_KEYBOARD).h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HHOOK hhk;  // 훅 핸들

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);


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
        hhk = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, NULL, GetCurrentThreadId());
        return (INT_PTR)TRUE;
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
        return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code < 0)
        return CallNextHookEx(hhk, code, wParam, lParam);
    
    switch (wParam)
    {
    case VK_ESCAPE:
        SetWindowText(GetActiveWindow(), _T("VK_ESCAPE"));
        return true;

    case VK_SPACE:
        SetWindowText(GetActiveWindow(), _T("VK_SPACE"));
        return true;

    case VK_RETURN:
        SetWindowText(GetActiveWindow(), _T("VK_RETURN"));
        return true;
    }

    return CallNextHookEx(hhk, code, wParam, lParam);
}