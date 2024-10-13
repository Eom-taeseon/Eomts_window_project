// [step36_ex2] 모드리스 대화상자_2.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step36_ex2] 모드리스 대화상자_2.h"

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    HWND hDlg;
    hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
    ShowWindow(hDlg, nCmdShow);

    // 기본 메시지 루프
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

// [step 12_ex1] 타이머에 의한 원 움직이기
#define STEP    10
#define R       20

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static POINT ptCircle;
    static RECT rtClient;
    static int direction = VK_RIGHT;

    switch (message)
    {
    case WM_INITDIALOG:
        SetTimer(hDlg, 1, 10, NULL);
        return (INT_PTR)true;

    case WM_SIZE:
        GetClientRect(hDlg, &rtClient);
        ptCircle.x = rtClient.right / 2;
        ptCircle.y = rtClient.bottom / 2;
        return true;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:
            direction = VK_LEFT;
            SetWindowText(hDlg, _T("LEFT"));
            break;

        case VK_RIGHT:
            direction = VK_RIGHT;
            SetWindowText(hDlg, _T("RIGHT"));
            break;

        case VK_UP:
            direction = VK_UP;
            SetWindowText(hDlg, _T("UP"));
            break;

        case VK_DOWN:
            direction = VK_DOWN;
            SetWindowText(hDlg, _T("DOWN"));
            break;
        }
        return (INT_PTR)TRUE;

    case WM_TIMER:
        switch (direction)
        {
        case VK_LEFT:
            ptCircle.x -= STEP;
            break;
        case VK_RIGHT:
            ptCircle.x += STEP;
            break;
        case VK_UP:
            ptCircle.y -= STEP;
            break;
        case VK_DOWN:
            ptCircle.y += STEP;
            break;
        }
        // check the boundaries and change the direction
        if (ptCircle.x - R <= 0)
        {
            ptCircle.x = R;
            direction = VK_RIGHT;
        }

        if (ptCircle.x + R >= rtClient.right)
        {
            ptCircle.x = rtClient.right - R;
            direction = VK_LEFT;
        }

        if (ptCircle.y - R <= 0)
        {
            ptCircle.y = R;
            direction = VK_DOWN;
        }

        if (ptCircle.y + R >= rtClient.bottom)
        {
            ptCircle.y = rtClient.bottom - R;
            direction = VK_UP;
        }

        InvalidateRect(hDlg, NULL, TRUE);   // 전체 영역 지우고 그리기
        return (INT_PTR)TRUE;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hDlg, &ps);
        // TODO : 여기에 hdc를 사용하는 그리기 코드 추가
        Ellipse(hdc, ptCircle.x - R, ptCircle.y - R, ptCircle.x + R, ptCircle.y + R);

        EndPaint(hDlg, &ps);
        return (INT_PTR)TRUE;
    }

    case WM_CLOSE:
        DestroyWindow(hDlg);
        return TRUE;

    case WM_DESTROY:
        KillTimer(hDlg, 1);
        PostQuitMessage(0);
        return (INT_PTR)TRUE;
    }

    return false;
}