// [step33_ex10]Picture 컨트롤.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step33_ex10]Picture 컨트롤.h"

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
    static HBITMAP hBit;
    static HWND hPicture;

    switch (message)
    {
    case WM_INITDIALOG:
    {
        // IDC_PICTURE1 컨트롤의 Image 속성이 IDB_BITMAP1이면 설정 필요 없음
        hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
        hPicture = GetDlgItem(hDlg, IDC_PICTURE1);
        SendMessage(hPicture, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBit);
        return (INT_PTR)TRUE;
    }

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDC_PICTURE1:
            if (HIWORD(wParam) == STN_CLICKED)
                MessageBox(hDlg, _T("Click"), _T("Msg"), MB_OK);
            break;

        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            break;
        }
        return (INT_PTR)TRUE;
    }
    case WM_DESTROY:
        DeleteObject(hBit);
        return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}