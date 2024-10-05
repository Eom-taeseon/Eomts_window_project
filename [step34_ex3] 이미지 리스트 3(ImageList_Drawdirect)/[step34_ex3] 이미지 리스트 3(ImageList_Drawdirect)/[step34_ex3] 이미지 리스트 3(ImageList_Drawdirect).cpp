// [step34_ex3] 이미지 리스트 3(ImageList_Drawdirect).cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step34_ex3] 이미지 리스트 3(ImageList_Drawdirect).h"

#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")

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

#include <time.h>
void Shuffle(IMAGELISTDRAWPARAMS card[])    // 카드 섞기 함수
{
    int i, j;
    IMAGELISTDRAWPARAMS tmp;
    srand(time(NULL));
    for (int k = 0; k < 1000; k++)
    {
        i = rand() % 52; // 2장을 뽑아 교환
        j = rand() % 52;
        tmp = card[i];
        card[i] = card[j];
        card[j] = tmp;
    }
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HIMAGELIST hIL;
    static IMAGELISTDRAWPARAMS card[52];

    switch (message)
    {
    case WM_INITDIALOG:
    {
        InitCommonControls();   // 공용 컨트롤 DLL 로드 확인
        hIL = ImageList_LoadImage(hInst,
            MAKEINTRESOURCE(IDB_BITMAP1), 71, 1, CLR_NONE,
            IMAGE_BITMAP, LR_DEFAULTCOLOR); // 284 x 1248

        // 이미지 정보를 card 배열에 저장
        for (int i = 0; i < 52; i++)
        {
            IMAGELISTDRAWPARAMS dp;
            dp.cbSize = sizeof(IMAGELISTDRAWPARAMS);
            dp.himl = hIL;
            dp.i = (int)i / 13;
            // dp.x = 0;    // (x, y)는 출력할 때 변경
            // dp.y = 0;
            dp.cx = 71;
            dp.cy = 96; // 96 = 1248 / 13
            dp.xBitmap = 0;
            dp.yBitmap = (i % 13) * dp.cy;
            dp.rgbBk = CLR_NONE; // CLR_DEFAULT
            dp.rgbFg = CLR_NONE;
            dp.fStyle = ILD_NORMAL;
            dp.dwRop = SRCCOPY;
            dp.hdcDst = GetDC(hDlg);
            card[i] = dp;
        }
        return (INT_PTR)TRUE;
    }

    case WM_KEYDOWN:
        if (wParam == VK_SPACE)
        {
            Shuffle(card);
            InvalidateRect(hDlg, NULL, TRUE);
        }
        return TRUE;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hDlg, &ps);
        // TODO : 여기에 hdc를 사용하는 그리기 코드 추가

        for (int i = 0; i < 13; i++)
        {
            card[i].x = i * (card[i].cx + 10);
            card[i].y = 0;
            ImageList_DrawIndirect(&card[i]);

            int j = i + 13;
            card[j].x = i * (card[j].cx + 10);
            card[j].y = card[j].cy + 10;
            ImageList_DrawIndirect(&card[j]);

            j = i + 13 * 2;
            card[j].x = i * (card[j].cx + 10);
            card[j].y = (card[j].cy + 10) * 2;
            ImageList_DrawIndirect(&card[j]);

            j = i + 13 * 3;
            card[j].x = i * (card[j].cx + 10);
            card[j].y = (card[j].cy + 10) * 3;
            ImageList_DrawIndirect(&card[j]);
        }
        EndPaint(hDlg, &ps);
        return TRUE;
    }

    case WM_CLOSE:
        ImageList_Destroy(hIL);
        DestroyWindow(hDlg);    // EndDialog(hDlg, TRUE);
        return TRUE;

    case WM_DESTROY:
        return TRUE;
    }
    return FALSE;
}