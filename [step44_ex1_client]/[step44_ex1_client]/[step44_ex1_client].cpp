// [step44_ex1_client].cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "framework.h"
#include "[step44_ex1_client].h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS // WSAAsyncSelect
#include <WinSock2.h>
#include <WS2tcpip.h>   // inet_pton
#pragma comment(lib, "Ws2_32.lib")

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
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

#define WM_CLIENT WM_USER+2
#define BUFSIZE 512
typedef enum {
    LOGIN = 1,
    LOGEXIST,
    LOGOUT
} PACKET_TYPE;
typedef struct {
    PACKET_TYPE ptype;
    unsigned short len; // length(data)
    char data[BUFSIZE];
} PACKET;

// 정보 대화상자의 메시지 처리기
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static SOCKET client;
    static SOCKADDR_IN addr;
    PACKET packet;
    static WCHAR loginID[BUFSIZE];  // login ID
    static int nTrial = 10;
    static BOOL bLogin = FALSE, bConnected = FALSE;

    switch (message) {
    case WM_INITDIALOG:
    {
        // WinSock 초기화
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);

        ZeroMemory(&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = 10000;

        // 서버가 다른 컴퓨터에 있으면 IP 주소를 변경해야함
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
        SetTimer(hDlg, 1, 1000, NULL);
        SendMessage(hDlg, WM_TIMER, 1, 0);
        return TRUE;
    }

    case WM_TIMER:
        nTrial--;   // 시도 횟수 감소
        // IPv4, TCP stream, protocol
        client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (connect(client, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
        {
            closesocket(client);
            bConnected = FALSE;
        }
        else    // connection OK
        {
            bConnected = TRUE;
            WSAAsyncSelect(client, hDlg, WM_CLIENT, FD_READ | FD_CLOSE);
            SetWindowText(hDlg, L"Connected...");
            KillTimer(hDlg, 1);
        }

        if (nTrial == 0)
            KillTimer(hDlg, 1);
        return TRUE;

    case WM_CLIENT:
        switch (WSAGETSELECTEVENT(lParam))
        {
        case FD_CLOSE:  // 소켓이 닫힐 때
            closesocket((SOCKET)wParam);    // 클라이언트 소켓 닫음
            bLogin = FALSE;
            bConnected = FALSE;
            EnableWindow(GetDlgItem(hDlg, IDC_EDIT_LOGIN), TRUE);
            SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_LOGIN), L"로그인");
            SetWindowText(hDlg, L"Disconnected...");

            nTrial = 20;    // 자동 연결 시도
            SetTimer(hDlg, 1, 1000, NULL);
            SendMessage(hDlg, WM_TIMER, 1, 0);
            break;

        case FD_READ:
        {
            // client = (SOCKET)wParam;
            int nR = recv(client, (char*)&packet, sizeof(PACKET), 0);
            switch (packet.ptype)
            {
            case LOGIN:
                bLogin = TRUE;
                memcpy(loginID, packet.data, packet.len);
                SetWindowText(hDlg, L"환영합니다.");
                EnableWindow(GetDlgItem(hDlg, IDC_EDIT_LOGIN), FALSE);
                SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_LOGIN), L"로그 아웃");
                break;

            case LOGOUT:
                bLogin = FALSE;
                memset(loginID, 0, BUFSIZE);
                SetWindowText(hDlg, L"로그인이 필요합니다.");
                EnableWindow(GetDlgItem(hDlg, IDC_EDIT_LOGIN), TRUE);
                SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_LOGIN), L"로그인");
                break;

            case LOGEXIST:
                bLogin = FALSE;
                SetWindowText(hDlg, L"서버에 같은 아이디가 존재합니다.");
                break;
            }
            break;
        }
        }
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_BUTTON_LOGIN:
            if (!bConnected)
                break;
            if (bLogin) {   // logout
                packet.ptype = LOGOUT;
                packet.len = (unsigned short)(wcslen(loginID) + 1) * 2;
                memcpy(packet.data, loginID, packet.len);
            }

            else {            // login
                WCHAR strID[BUFSIZE];
                GetDlgItemText(hDlg, IDC_EDIT_LOGIN, strID, 128);
                if (wcslen(strID) == 0)
                    break;
                packet.ptype = LOGIN;
                packet.len = (unsigned short)(wcslen(strID) + 1) * 2;
                memcpy(packet.data, strID, packet.len);
            }
            send(client, (char*)&packet, sizeof(PACKET), 0);
            break;
        }
        return TRUE;

    case WM_CLOSE:
        DestroyWindow(hDlg);
        return TRUE;
    case WM_DESTROY:
        closesocket(client);
        WSACleanup();
        return TRUE;
    }
    return FALSE;
}