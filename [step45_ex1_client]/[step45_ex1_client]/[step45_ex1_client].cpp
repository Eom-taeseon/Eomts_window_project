// [step45_ex1_client].cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step45_ex1_client].h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS // WSAAsyncSelect
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DlgProcLogin(HWND, UINT, WPARAM, LPARAM);

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
#define BLOCK_SIZE  80
#define WM_CLIENT           WM_USER+1
#define WM_CONNECT          WM_USER+2

#define WM_GAME_INIT        WM_USER+3
#define WM_GAME_START       WM_USER+4
#define WM_GAME_END         WM_USER+5  
#define WM_GAME_STATUS      WM_USER+6
#define WM_NETWORKPLAYER    WM_USER+7

// 게임 정보
typedef enum { 
    EMPTY, 
    PLAYER1,
    PLAYER2,
}PLAYER;
typedef PLAYER CELL;
typedef enum {
    NONE,
    GAMEPLAYER1,
    GAMEPLAYER2,
    DRAW
}WINNER;

// 패킷 정보
#define BUFSIZE 512
typedef enum {
    LOGIN=1,
    LOGEXIST,
    LOGOUT,
    OTHERLOGIN,
    OTHERLOGOUT,
    GAME_ASK,
    GAME_ACCEPT,
    GAME_REJECT,
    GAME_POINT,
    GAME_END
}PACKET_TYPE;

typedef struct {
    PACKET_TYPE ptype;
    unsigned short len; // length(data)
    char data[BUFSIZE];
}PACKET;

#define BUFSIZE2 100
typedef struct {    // PACKET의 data 내용
    unsigned short lenFrom;
    WCHAR fromID[BUFSIZE2];

    unsigned short lenTo;
    WCHAR toID[BUFSIZE2];

    unsigned short ix;  // GAME_END일 때는 winner 저장
    unsigned short iy;
}GAME_DATA;

WCHAR loginID[BUFSIZE];
WNDPROC OldProc;
INT_PTR CALLBACK PictureSubClassProc(HWND, UINT, WPARAM, LPARAM);
SOCKET client;

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    static HWND hList;  // 서버 로그인 정보
    //static SOCKET client;   
    static SOCKADDR_IN addr;
    PACKET packet;
    static BOOL bConnected = FALSE; // 서버 연결
    static BOOL bLogin = FALSE;     // 로그인
    static HWND hPicture;           // 게임 컨트롤
    static BOOL bGameRunning = FALSE;

    switch (message) {
    case WM_INITDIALOG:
    {
        hList = GetDlgItem(hDlg, IDC_LIST1);
        // Winsock 초기화
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);

        ZeroMemory(&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = 20000;

        //inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

        // 서버가 다른 컴퓨터에 있으면 IP 주소를 변경해야함
        inet_pton(AF_INET, "182.226.196.208", &addr.sin_addr.s_addr);
        SendMessage(hDlg, WM_CONNECT, 0, 0);

        // ref: step37_ex3
        // Picture 컨트롤 크기 조정
        hPicture = GetDlgItem(hDlg, IDC_PICTURE1);
        SetWindowPos(hPicture, NULL, 0, 0, BLOCK_SIZE * 3, BLOCK_SIZE * 3, SWP_NOMOVE);

        // Picture 컨트롤 서브클래싱
        //OldProc = (WNDPROC)SetWindowLong(hPicture, GWL_WNDPROC, (LONG)PictureSubClassProc);
        OldProc = (WNDPROC)SetWindowLongPtr(hPicture, GWLP_WNDPROC, (LONG_PTR)PictureSubClassProc);
        SendMessage(hPicture, WM_GAME_INIT, 0, 0);

        HFONT hFont = CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0,
            HANGEUL_CHARSET, 0, 0, 0, 0, _T("바탕"));
        SendDlgItemMessage(hDlg, IDC_BUTTON_LOGIN, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendDlgItemMessage(hDlg, IDC_BUTTON_CONNECT, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendDlgItemMessage(hDlg, IDC_BUTTON_GAME, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendDlgItemMessage(hDlg, IDC_LIST1, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendDlgItemMessage(hDlg, IDC_STATIC_TURN, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendDlgItemMessage(hDlg, IDC_STATIC_PLAYER, WM_SETFONT, (WPARAM)hFont, FALSE);

        return TRUE;
    }

    case WM_CTLCOLORDLG:    // 대화상자 배경색 브러시
    {
        return (INT_PTR)GetStockObject(GRAY_BRUSH);
    }

    case WM_CTLCOLORSTATIC:
    {
        HWND ctrlWnd = (HWND)lParam;
        HDC hdc = (HDC)wParam;
        if (ctrlWnd == GetDlgItem(hDlg, IDC_STATIC) ||
            ctrlWnd == GetDlgItem(hDlg, IDC_STATIC_TURN)) {
            SetTextColor(hdc, RGB(0, 0, 255));
            SetBkMode(hdc, TRANSPARENT);
            return (INT_PTR)GetStockObject(GRAY_BRUSH);
        }
    }
    break;

    case WM_CONNECT:
    {
        // IPv4, TCP stream, protocol
        client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (connect(client, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR) {
            closesocket(client);
            bConnected = FALSE;
            SetWindowText(hDlg, L"Disconnected...");
            SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_CONNECT), L"서버 연결");
        }

        else {  // connection OK
            bConnected = TRUE;
            WSAAsyncSelect(client, hDlg, WM_CLIENT, FD_READ | FD_CLOSE);
            SetWindowText(hDlg, L"Connected...");
            SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_CONNECT), L"서버 나가기");
        }
        return TRUE;
    }

    // wParam : socket, LOWORD(lParm): network event
    case WM_CLIENT:
        switch (WSAGETSELECTEVENT(lParam)) {
        case FD_CLOSE:  // 소켓이 닫힐 때
            closesocket((SOCKET)wParam);
            bLogin = FALSE;
            bConnected = FALSE;

            SetWindowText(hDlg, L"Disconnected...");
            SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_LOGIN), L"로그인");
            SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_CONNECT), L"서버 연결");
            SendMessage(hList, LB_RESETCONTENT, 0, 0);
            SendMessage(hPicture, WM_GAME_INIT, 0, 0);
            break;

        case FD_READ:
        {
            // client = (SOCKET)wParam;
            WCHAR recvID[BUFSIZE];
            WCHAR strTmp[BUFSIZE];
            int nR = recv(client, (char*)&packet, sizeof(PACKET), 0);
            switch (packet.ptype) {
            case LOGIN:
            {
                bLogin = TRUE;
                memcpy(loginID, packet.data, packet.len);
                wsprintf(strTmp, L"[%s]님 환영합니다.", loginID);
                SetWindowText(hDlg, strTmp);
                SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_LOGIN), L"로그 아웃");
                break;
            }

            case LOGOUT:
            {
                SendMessage(hList, LB_RESETCONTENT, 0, 0);
                bGameRunning = SendMessage(hPicture, WM_GAME_STATUS, 0, 0);

                if (bGameRunning)
                    SendMessage(hPicture, WM_GAME_INIT, 0, 0);

                bLogin = FALSE;
                memset(loginID, 0, BUFSIZE);
                SetWindowText(hDlg, L"로그인이 필요합니다.");
                SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_LOGIN), L"로그인");
                break;
            }

            case LOGEXIST:
            {
                bLogin = FALSE;
                SetWindowText(hDlg, L"서버에 같은 아이디가 존재합니다");
                break;
            }

            case OTHERLOGIN:    // IDC_LIST1에 추가
            {
                memcpy(recvID, packet.data, packet.len);
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)recvID);
                break;
            }

            case OTHERLOGOUT:   // IDC_LIST1에서 삭제
            {
                memcpy(recvID, packet.data, packet.len);
                unsigned int k = SendMessage(hList, LB_FINDSTRINGEXACT, 1, (LPARAM)recvID);
                SendMessage(hList, LB_DELETESTRING, k, 0);

                WCHAR networkID[100];
                bGameRunning = SendMessage(hPicture, WM_GAME_STATUS, (WPARAM)&networkID, 0);

                // 게임 중인 상대가 로그아웃
                if (bGameRunning && lstrcmp(recvID, networkID) == 0)
                    SendMessage(hPicture, WM_GAME_INIT, 0, 0);
                break;
            }

            case GAME_ASK:  // fromID <-> toID 교환해 응답
            {
                GAME_DATA tmp, gdata;
                memcpy(&tmp, packet.data, packet.len);

                gdata.lenFrom = tmp.lenTo;
                memcpy(gdata.fromID, tmp.toID, tmp.lenTo);
                gdata.lenTo = tmp.lenFrom;
                memcpy(gdata.toID, tmp.fromID, tmp.lenFrom);

                bGameRunning = SendMessage(hPicture, WM_GAME_STATUS, 0, 0);
                if (bGameRunning)
                    packet.ptype = GAME_REJECT;
                else {
                    if (MessageBox(hDlg, _T("게임 할까요 ?"), _T("Msg"), MB_YESNO | MB_TOPMOST) == IDYES) {
                        // 수락한 사람은 PLAYER 2
                        packet.ptype = GAME_ACCEPT;
                        SendMessage(hPicture, WM_GAME_START, (WPARAM)PLAYER2, (LPARAM)(tmp.fromID));
                        unsigned int k = SendMessage(hList, LB_FINDSTRINGEXACT, 1, (LPARAM)tmp.fromID);
                        SendMessage(hList, LB_SETCURSEL, k, 0);
                        WCHAR str[100];
                        wsprintf(str, L"Player2[%s]: X", loginID);
                        SetDlgItemText(hDlg, IDC_BUTTON_GAME, str);
                        EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_GAME), FALSE);
                        wsprintf(str, L"Player1[%s]: O", tmp.fromID);
                        SetDlgItemText(hDlg, IDC_STATIC_PLAYER, str);
                        ShowWindow(GetDlgItem(hDlg, IDC_STATIC_PLAYER), SW_SHOW);

                        SetDlgItemText(hDlg, IDC_STATIC_TURN, L"O - Turn");
                        ShowWindow(GetDlgItem(hDlg, IDC_STATIC_TURN), SW_SHOW);
                    }

                    else {
                        packet.ptype = GAME_REJECT;
                    }
                }

                // 패킷 응답
                packet.len = sizeof(GAME_DATA);
                memcpy(packet.data, &gdata, packet.len);
                send(client, (char*)&packet, sizeof(PACKET), 0);
                break;
            }

            case GAME_ACCEPT:   // 요청해서 수락을 받은 메시지, 즉 요청한 사람
            {
                GAME_DATA tmp;
                memcpy(&tmp, packet.data, packet.len);

                // 요청한 사람은 Player1
                SendMessage(hPicture, WM_GAME_START, (WPARAM)PLAYER1, (LPARAM)(tmp.fromID));
                WCHAR str[100];
                wsprintf(str, L"PLAYER1[%s]: O", loginID);
                SetDlgItemText(hDlg, IDC_BUTTON_GAME, str);
                EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_GAME), FALSE);
                wsprintf(str, L"Player2[%s]: X", tmp.fromID);
                SetDlgItemText(hDlg, IDC_STATIC_PLAYER, str);
                ShowWindow(GetDlgItem(hDlg, IDC_STATIC_PLAYER), SW_SHOW);

                // 시작은 O-Turn
                SetDlgItemText(hDlg, IDC_STATIC_TURN, L"O - Turn");
                ShowWindow(GetDlgItem(hDlg, IDC_STATIC_TURN), SW_SHOW);
                break;
            }

            case GAME_REJECT:
            {
                bGameRunning = FALSE;
                EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_GAME), TRUE);
                MessageBox(hDlg, _T("거절하였습니다"), _T("Msg"), MB_OK | MB_TOPMOST);
                break;
            }

            case GAME_POINT:
            {
                GAME_DATA tmp;
                memcpy(&tmp, packet.data, packet.len);
                int ix = tmp.ix;
                int iy = tmp.iy;
                SendMessage(hPicture, WM_NETWORKPLAYER, (WPARAM)ix, (LPARAM)iy);
                break;
            }

            case GAME_END:
            {
                GAME_DATA tmp;
                memcpy(&tmp, packet.data, packet.len);

                TCHAR str[128];
                if ((WINNER)tmp.ix == DRAW)
                    wsprintf(str, _T("Draw!"));
                else
                    wsprintf(str, _T("WINNER = %s"), ((WINNER)tmp.ix == GAMEPLAYER1) ? _T("O") : _T("X"));

                SetDlgItemText(hDlg, IDC_BUTTON_GAME, L"게임 시작");
                EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_GAME), TRUE);
                ShowWindow(GetDlgItem(hDlg, IDC_STATIC_TURN), SW_HIDE);
            }
            }
        }
        return TRUE;
        }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_BUTTON_CONNECT:
            if (!bConnected)
                SendMessage(hDlg, WM_CONNECT, 0, 0);
            else {
                closesocket(client);
                SendMessage(hDlg, WM_CLIENT, 0, MAKEWORD(FD_CLOSE, 0));
            }
            break;

        case IDC_BUTTON_LOGIN:
            if (!bConnected)
                break;
            if (!bLogin) {
                if (DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hDlg, DlgProcLogin) == IDOK) {
                    if (wcslen(loginID) == 0)
                        break;
                    packet.ptype = LOGIN;
                    packet.len = (unsigned short)(wcslen(loginID) + 1) * 2;
                    memcpy(packet.data, loginID, packet.len);
                    send(client, (char*)&packet, sizeof(PACKET), 0);
                }
            }
            else {
                packet.ptype = LOGOUT;
                packet.len = (unsigned short)(wcslen(loginID) + 1) * 2;
                memcpy(packet.data, loginID, packet.len);
                send(client, (char*)&packet, sizeof(PACKET), 0);
            }
            break;

        case IDC_BUTTON_GAME:
            if (bGameRunning == FALSE) {
                EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_GAME), FALSE);
                packet.ptype = GAME_ASK;
                packet.len = sizeof(GAME_DATA);

                GAME_DATA tmp;

                // from
                tmp.lenFrom = (unsigned short)(wcslen(loginID) + 1) * 2;
                memcpy(tmp.fromID, loginID, tmp.lenFrom);

                // to
                WCHAR strID[128];
                int i = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
                if (i < 0) {
                    EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_GAME), TRUE);
                    MessageBox(hDlg, _T("게임 상대를 선택하세요!"), _T("Msg"), MB_OK | MB_TOPMOST);
                    break;
                }

                SendMessage(hList, LB_GETTEXT, i, (LPARAM)strID);
                tmp.lenTo = (unsigned short)(wcslen(loginID) + 1) * 2;
                memcpy(tmp.toID, strID, tmp.lenTo);

                memcpy(packet.data, &tmp, packet.len);
                send(client, (char*)&packet, sizeof(PACKET), 0);
            }
            else {
                EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_GAME), TRUE);
                MessageBox(hDlg, _T("상대가 게임중입니다. 다른 상대를 선택하세요!"), _T("Msg"), MB_OK | MB_TOPMOST);
            }
            break;
        }
        return TRUE;

    case WM_CLOSE:
        if (bLogin) {
            packet.ptype = LOGOUT;
            packet.len = (unsigned short)(wcslen(loginID) + 1) * 2;
            memcpy(packet.data, loginID, packet.len);
            send(client, (char*)&packet, sizeof(PACKET), 0);
        }

        SendMessage(hPicture, WM_GAME_END, 0, 0);
        DestroyWindow(hDlg);
        return TRUE;

    case WM_DESTROY:
        return TRUE;
    }
    return FALSE;
}

// 정보 대화상자의 메시지 처리기
INT_PTR CALLBACK DlgProcLogin(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
    {
        // 로그인 대화상자를 부모 대화상자 가운데에 출력
        HWND hParent = GetParent(hDlg);
        RECT rcParent, rcDlg, rc;
        GetWindowRect(hParent, &rcParent);
        GetWindowRect(hDlg, &rcDlg);
        CopyRect(&rc, &rcParent);   // rc = rcDlgMain;

        OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
        OffsetRect(&rc, -rc.left, -rc.top);
        OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom);
        SetWindowPos(hDlg, HWND_TOP,
            rcParent.left + (rc.right / 2),
            rcParent.top + (rc.bottom / 2),
            0, 0, SWP_NOSIZE);

        HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("바탕"));
        SendDlgItemMessage(hDlg, IDC_EDIT_LOGIN, WM_SETFONT, (WPARAM)hFont, FALSE);
        return TRUE;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
            GetDlgItemText(hDlg, IDC_EDIT_LOGIN, loginID, 128);
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        }
    }
    return FALSE;   
}

void DrawBlock(HDC hdc, int ix, int iy, HBITMAP hBit) {
    // BITMAP bit;
    // bit.bmWidth, bit.bmHeight: BLOCK_SIZE
    // GetObject(hBit, sizeof(BITMAP), &bit);
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP oldBit = (HBITMAP)SelectObject(memDC, hBit);
    BitBlt(hdc, ix * BLOCK_SIZE, iy * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, memDC, 0, 0, SRCCOPY);
    SelectObject(memDC, oldBit);
    DeleteDC(memDC);
}

void DrawBoard(HDC hdc, HBITMAP hBitmap[], CELL board[][3]) {
    for(int ix=0; ix<3; ix++)
        for (int iy = 0; iy < 3; iy++) {
            int img = (int)board[iy][ix];
            DrawBlock(hdc, ix, iy, hBitmap[img]);
        }
}

WINNER WinnerCheck(int ix, int iy, CELL board[][3]) {
    // if(board[iy][ix] == EMPTY)   // 이런 경우는 없음
    //      return NONE;
    if (board[iy][0] == board[iy][1] && board[iy][1] == board[iy][2])
        return (WINNER)board[iy][0];    // 가로
    else if (board[0][ix] == board[1][ix] && board[1][ix] == board[2][ix])
        return (WINNER)board[0][ix];    // 세로
    else if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return (WINNER)board[0][0];    // 대각선
    else if (board[2][0] == board[1][1] && board[1][1] == board[0][2])
        return (WINNER)board[2][0];    // 역대각선

    else {  // 비김 확인
        for(int x = 0;x<3;x++)
            for (int y = 0; y < 3; y++) {
                if (board[y][x] == EMPTY)
                    return NONE;
            }
        return DRAW;    // EMPTY가 하나도 없는 경우
    }
    return NONE;    // EMPTY가 적어도 하나 있는 경우
}

INT_PTR CALLBACK PictureSubClassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    // static SOCKET client;
    static WCHAR networkID[100];
    static HBITMAP hBitmap[3];
    static CELL board[3][3];
    static BOOL bBitmapLoaded = FALSE;
    static PLAYER gamePlayer;
    static BOOL bGameRunning;
    static BOOL bMyTurn = TRUE;
    static HWND hParent;    // 부모 대화상자

    switch (message) {
    case WM_GAME_INIT:
        if (!bBitmapLoaded) {
            bBitmapLoaded = TRUE;
            hBitmap[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP0));
            hBitmap[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
            hBitmap[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
        }
        hParent = GetParent(hWnd);
        SetDlgItemText(hParent, IDC_BUTTON_GAME, L"게임 시작");
        EnableWindow(GetDlgItem(hParent, IDC_BUTTON_GAME), TRUE);
        ShowWindow(GetDlgItem(hParent, IDC_STATIC_PLAYER), SW_HIDE);
        ShowWindow(GetDlgItem(hParent, IDC_STATIC_TURN), SW_HIDE);

        // 선택 해제
        SendMessage(GetDlgItem(hParent, IDC_LIST1), LB_SETCURSEL, -1, 0);
        bGameRunning = FALSE;
        memset(board, EMPTY, sizeof(board));
        InvalidateRect(hWnd, NULL, FALSE);
        return TRUE;

    case WM_GAME_START:
        gamePlayer = (PLAYER)wParam;
        if (gamePlayer == PLAYER1)
            bMyTurn = TRUE;
        else
            bMyTurn = FALSE;
        memcpy(networkID, (WCHAR*)lParam, (lstrlen((WCHAR*)lParam) + 1) * 2);
        bGameRunning = TRUE;
        return TRUE;

    case WM_GAME_STATUS:
    {
        WCHAR* ptr = (WCHAR*)wParam;
        if (ptr)
            memcpy(ptr, networkID, (lstrlen(networkID) + 1) * 2);
        return bGameRunning;
    }

    case WM_NETWORKPLAYER:
    {
        int ix = (int)wParam;
        int iy = (int)lParam;
        if (board[iy][ix] != EMPTY || !bGameRunning)
            return TRUE;

        // 상대방 데이터를 출력하기 위해 토글
        board[iy][ix] = (gamePlayer == PLAYER1) ? PLAYER2 : PLAYER1;
        bMyTurn = TRUE;
        if (gamePlayer == PLAYER1)
            SetDlgItemText(hParent, IDC_STATIC_TURN, L"O - TURN");
        else
            SetDlgItemText(hParent, IDC_STATIC_TURN, L"X - TURN");
        InvalidateRect(hWnd, NULL, FALSE);
        return TRUE;
    }

    case WM_LBUTTONDOWN:
    {
        if (!bMyTurn)
            return TRUE;
        int ix, iy;
        ix = (int)(LOWORD(lParam) / BLOCK_SIZE);
        iy = (int)(HIWORD(lParam) / BLOCK_SIZE);
        if (board[iy][ix] != EMPTY || !bGameRunning)
            return TRUE;

        // EMPTY인 경우만
        board[iy][ix] = gamePlayer;

        // 네트워크 전송
        PACKET packet;
        packet.ptype = GAME_POINT;
        packet.len = sizeof(GAME_DATA);

        GAME_DATA gameData;

        // from 
        gameData.lenFrom = (unsigned short)(wcslen(loginID) + 1) * 2;
        memcpy(gameData.fromID, loginID, gameData.lenFrom);

        // to
        gameData.lenTo = (unsigned short)(wcslen(networkID) + 1) * 2;
        memcpy(gameData.toID, networkID, gameData.lenTo);

        gameData.ix = ix;
        gameData.iy = iy;

        memcpy(packet.data, &gameData, packet.len);
        send(client, (char*)&packet, sizeof(PACKET), 0);

        InvalidateRect(hWnd, NULL, FALSE);  // 현재 클릭 반영

        WINNER winner = WinnerCheck(ix, iy, board);
        if (winner != NONE) {
            bGameRunning = FALSE;
            TCHAR str[128];
            if (winner == DRAW)
                wsprintf(str, _T("Draw!!"));
            else
                wsprintf(str, _T("Winner = %s"), (winner == GAMEPLAYER1) ? _T("O") : _T("X"));

            // from, to는 위의 것 그대로
            packet.ptype = GAME_END;
            //packet.len = sizeof(GAME_DATA);
            gameData.ix = (unsigned short)winner;
            memcpy(packet.data, &gameData, packet.len);
            send(client, (char*)&packet, sizeof(PACKET), 0);

            if (MessageBox(hWnd, str, _T("Msg"), MB_OK | MB_TOPMOST) == IDOK) {
                SendMessage(hWnd, WM_GAME_INIT, 0, 0);
            }
        }
        else {
            bMyTurn = FALSE;    // 플레이어 전환
            if (gamePlayer == PLAYER1)
                SetDlgItemText(hParent, IDC_STATIC_TURN, L"X - turn");
            else
                SetDlgItemText(hParent, IDC_STATIC_TURN, L"O - turn");
        }
        return TRUE;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        DrawBoard(hdc, hBitmap, board);
        EndPaint(hWnd, &ps);
        return TRUE;
    }

    case WM_GAME_END:
        for (int i = 0; i < 3; i++)
            DeleteObject(hBitmap[i]);
        bBitmapLoaded = FALSE;
        bGameRunning = FALSE;
        return TRUE;
    }

    return CallWindowProc((WNDPROC)OldProc, hWnd, message, wParam, lParam);
}
