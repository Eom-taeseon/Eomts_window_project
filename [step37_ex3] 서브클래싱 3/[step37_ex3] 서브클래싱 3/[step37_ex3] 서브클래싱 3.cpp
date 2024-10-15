// [step37_ex3] 서브클래싱 3.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step37_ex3] 서브클래싱 3.h"

#include <stdio.h>


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.

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

#define BLOCK_SIZE      80
#define WM_GAME_START   WM_USER+1
#define WM_GAME_END     WM_USER+2

WNDPROC OldProc;    // 컨트롤의 기존 프로시저
INT_PTR CALLBACK PictureSubClassProc(HWND, UINT, WPARAM, LPARAM);

// 정보 대화상자의 메시지 처리기
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG: 
    {
        // 대화상자 크기 조정, 100은 마진
        RECT rt = { 0, 0, BLOCK_SIZE * 3 + 100, BLOCK_SIZE * 3 + 100 };
        AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);

        int nWidth = rt.right - rt.left;
        int nHeight = rt.bottom - rt.top;

        // 디버그 출력으로 ix, iy 값 확인
        TCHAR debugMsg[128];
        wsprintf(debugMsg, _T("[INITDIALOG] nWidth: %d, nHeight: %d\n"), nWidth, nHeight);
        OutputDebugString(debugMsg);

        SetWindowPos(hDlg, NULL, 0, 0, nWidth, nHeight, SWP_NOMOVE);

        // Picture 컨트롤 크기 조정
        HWND hPicture = GetDlgItem(hDlg, IDC_PICTURE1);
        if (hPicture == NULL)
        {
            OutputDebugString(_T("Failed to get handle for Picture control\n"));
        }
        else
        {
            OutputDebugString(_T("Successfully retrieved handle for Picture control\n"));
        }

        SetWindowPos(hPicture, NULL, 50, 50, BLOCK_SIZE * 3, BLOCK_SIZE * 3, 0);

        // Picture ctrl subclassing
        //OldProc = (WNDPROC)SetWindowLong(hPicture, GWLP_WNDPROC, (LONG)PictureSubClassProc); // 64bit/32bit 환경 오류
        OldProc = (WNDPROC)SetWindowLongPtr(hPicture, GWLP_WNDPROC, (LONG_PTR)PictureSubClassProc); // 빌드 > 구성관리자 에서 64bit 확인
        

        if (OldProc == 0)
        {
            OutputDebugString(_T("Failed to subclass Picture control\n"));
        }
        else
        {
            OutputDebugString(_T("Successfully subclassed Picture control\n"));
        }

        SendMessage(hPicture, WM_GAME_START, 0, 0);
        return true;
    }
    
    case WM_CTLCOLORDLG:    // 대화상자 배경색 브러시
    {
        return (INT_PTR)GetStockObject(GRAY_BRUSH);
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

typedef enum { EMPTY, PLAYER1, PLAYER2 } PLAYER;
typedef PLAYER CELL;
typedef enum { NONE, GAMEPLAYER1, GAMEPLAYER2, DRAW } WINNER;

void DrawBlock(HDC hdc, int ix, int iy, HBITMAP hBit)
{
    // BITMAP bit;
    // bit.bmWidth, bit.bmHeight: BLOCK_SIZE
    // GetObject(hBit, sizeof(BITMAP), &bit);
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP oldBit = (HBITMAP)SelectObject(memDC, hBit);
    BitBlt(hdc, ix * BLOCK_SIZE, iy * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, memDC, 0, 0, SRCCOPY);
    SelectObject(memDC, oldBit);
    DeleteDC(memDC);
}

void DrawBoard(HDC hdc, HBITMAP hBitmap[], CELL board[][3])
{
    for (int ix=0; ix<3; ix++)
        for (int iy = 0; iy < 3; iy++)
        {
            int img = (int)board[iy][ix];
            DrawBlock(hdc, ix, iy, hBitmap[img]);
        }
}

WINNER WinnerCheck(int ix, int iy, CELL board[][3])
{
    // if(borad[iy][ix] == EMPTY)   // 이런 경우는 없으므로 주석처리
    // return NONE;

    if (board[iy][0] == board[iy][1] && board[iy][1] == board[iy][2])
        return (WINNER)board[iy][0];    // 가로

    else if(board[0][ix] == board[1][ix] && board[1][ix] == board[2][ix])
        return (WINNER)board[0][ix];    // 세로

    else if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return (WINNER)board[0][0];     // 대각선

    else if (board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return (WINNER)board[2][0];     // 역대각선

    else {  // 비김
        for(int x = 0; x<3; x++)
            for(int y = 0; y<3; y++)
            {
                if (board[x][y] == EMPTY)
                    return NONE;
            }
        return DRAW;    // EMPTY가 하나도 없는 경우
    }
    return NONE;    // EMPTY가 적어도 하나는 있는 경우
}

INT_PTR CALLBACK PictureSubClassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP  hBitmap[3];
    static CELL     board[3][3];
    static BOOL     bBitmapLoaded = FALSE;
    static PLAYER   gamePlayer = PLAYER1;
    static BOOL     gameRunning = FALSE;

    switch (message)
    {
    case WM_GAME_START:
        if (!bBitmapLoaded)
        {
            bBitmapLoaded = TRUE;
            hBitmap[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP0));   // EMPTY
            hBitmap[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));   // O
            hBitmap[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));   // X

            // 비트맵 로드 여부를 디버그 메시지로 확인
            if (hBitmap[0] == NULL)
                OutputDebugString(_T("Failed to load IDB_BITMAP0 (EMPTY)\n"));
            else
                OutputDebugString(_T("Successfully loaded IDB_BITMAP0 (EMPTY)\n"));

            if (hBitmap[1] == NULL)
                OutputDebugString(_T("Failed to load IDB_BITMAP1 (O)\n"));
            else
                OutputDebugString(_T("Successfully loaded IDB_BITMAP1 (O)\n"));

            if (hBitmap[2] == NULL)
                OutputDebugString(_T("Failed to load IDB_BITMAP2 (X)\n"));
            else
                OutputDebugString(_T("Successfully loaded IDB_BITMAP2 (X)\n"));
        }

        gameRunning = TRUE;
        memset(board, EMPTY, sizeof(board));
        InvalidateRect(hWnd, NULL, FALSE);
        return TRUE;

        /*
        // Picture 컨트롤 배경을 변경하고자 하는 경우,
        // 여기서는 블록이 컨트롤 전체를 사용하므로 필요 없음
        case WM_ERASEBKGND:
        {
        HDC hdc = GetDC(hWnd);
        RECT rtClient;
        GetClientRect(hWnd, &rtClient);
        FillRect(hdc, &rtClient, (HBRUSH)(COLOR_WINDOW + 2));
        ReleaseDC(hWnd, hdc);
        return TRUE;
        }
        */

    case WM_LBUTTONDOWN:
    {
        int ix, iy;
        ix = (int)(LOWORD(lParam) / BLOCK_SIZE);
        iy = (int)(HIWORD(lParam) / BLOCK_SIZE);

        if (board[iy][ix] != EMPTY || !gameRunning)
            return true;

        // EMPTY인 경우
        board[iy][ix] = gamePlayer;
        InvalidateRect(hWnd, NULL, FALSE);  // 현재 클릭을 반영

        WINNER winner = WinnerCheck(ix, iy, board);
        if (winner != NONE)
        {
            gameRunning = FALSE;
            TCHAR str[128];

            if (winner == DRAW)
                wsprintf(str, _T("Draw, 한 판 더?"));

            else
                wsprintf(str, _T("Winner = %s, 한 판 더?"),
                    (winner == GAMEPLAYER1) ? _T("O") : _T("X"));

            if (MessageBox(hWnd, str, _T("Msg"), MB_YESNO) == IDYES)
                SendMessage(hWnd, WM_GAME_START, 0, 0);
            else
            {
                SendMessage(hWnd, WM_GAME_END, 0, 0);
                EndDialog(GetParent(hWnd), LOWORD(wParam));
            }
        }

        // Player 전환
        gamePlayer = (gamePlayer == PLAYER1) ? PLAYER2 : PLAYER1;
        return true;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO : 여기에 hdc를 사용하는 그리기 코드 추가
        DrawBoard(hdc, hBitmap, board);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_GAME_END:
        for (int i = 0; i < 3; i++)
            DeleteObject(hBitmap[i]);
        return TRUE;
    }

    return CallWindowProc((WNDPROC)OldProc, hWnd, message, wParam, lParam);
}