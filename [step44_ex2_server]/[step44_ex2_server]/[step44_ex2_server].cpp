// [step44_ex2_server].cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "framework.h"
#include "[step44_ex2_server].h"

#include <string>
#include <map>
using namespace std;

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
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

#define WM_SERVER WM_USER+1
#define BUFSIZE 512

typedef enum {
    LOGIN = 1, 
    LOGEXIST,
    LOGOUT,
    OTHERLOGIN,
    OTHERLOGOUT
} PACKET_TYPE;

typedef struct {
    PACKET_TYPE ptype;
    unsigned short len; // length(data)
    char    data[BUFSIZE];
} PACKET;

// 정보 대화상자의 메시지 처리기
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    static HWND hList;
    static SOCKET server;
    static map<wstring, SOCKET> clients;
    map<wstring, SOCKET>::iterator it;
    PACKET packet;
    WCHAR strTmp[256];
    WCHAR recvID[256];

    switch (message) {
    case WM_INITDIALOG: {
        hList = GetDlgItem(hDlg, IDC_LIST1);

        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);   // Winsock 초기화

        // IPv4, TCP stream, protocol
        server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        SOCKADDR_IN addr;
        ZeroMemory(&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = 10000;
        inet_pton(AF_INET, "loopback", &addr.sin_addr.s_addr);
        bind(server, (LPSOCKADDR)&addr, sizeof(addr));
        if (listen(server, SOMAXCONN) == SOCKET_ERROR) {
            SetWindowText(hDlg, L"listen error");
            closesocket(server);
            WSACleanup();
        }
        SetWindowText(hDlg, L"listening...");
        WSAAsyncSelect(server, hDlg, WM_SERVER, FD_ACCEPT);
        return TRUE;
    }

    case WM_SERVER:
        switch (WSAGETSELECTEVENT(lParam)) {
        case FD_ACCEPT:
        {
            // FD_READ에서 ID 확인 후 clients에 등록
            SOCKET client;
            client = accept(server, NULL, NULL);
            WSAAsyncSelect(client, hDlg, WM_SERVER, FD_READ | FD_CLOSE);
            break;
        }

        case FD_READ:   // 패킷 분석
        {
            SOCKET client = (SOCKET)wParam;
            int nR = recv(client, (char*)&packet, sizeof(PACKET), 0);
            switch (packet.ptype) {
            case LOGIN:
            {
                memcpy(recvID, packet.data, packet.len);
                if (clients.size() == 0) {  // 첫 로그인
                    clients.insert(pair<wstring, SOCKET>(wstring(recvID), client));

                    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)recvID);
                    wsprintf(strTmp, L"%d", clients.size());
                    SetWindowText(GetDlgItem(hDlg, IDC_STATIC_COUNT), strTmp);
                    send(client, (char*)&packet, sizeof(PACKET), 0);    // reply to client
                }

                else {   // 검사하여 없을 때만
                    it = clients.find(wstring(recvID));
                    if (it == clients.end()) {  // not found
                        // 로그인 브로드캐스팅
                        for (it = clients.begin(); it != clients.end(); it++) {
                            // 기존 있는 패킷에 로그인 정보 보냄
                            packet.ptype = OTHERLOGIN;
                            packet.len = (unsigned short)(wcslen(recvID) + 1) * 2;
                            memcpy(packet.data, recvID, packet.len);
                            send(it->second, (char*)&packet, sizeof(PACKET), 0);

                            // 로그인한 곳에 기존 정보 보냄
                            wcscpy_s(strTmp, it->first.c_str());
                            packet.len = (unsigned short)(wcslen(strTmp) + 1) * 2;
                            memcpy(packet.data, strTmp, packet.len);
                            send(client, (char*)&packet, sizeof(PACKET), 0);
                        }

                        // 로그인 피드백
                        clients.insert(pair<wstring, SOCKET>(wstring(recvID), client));
                        SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)recvID);
                        wsprintf(strTmp, L"%d", clients.size());

                        SetWindowText(GetDlgItem(hDlg, IDC_STATIC_COUNT), strTmp);

                        packet.ptype = LOGIN;
                        packet.len = (unsigned short)(wcslen(recvID) + 1) * 2;
                        memcpy(packet.data, recvID, packet.len);
                        send(client, (char*)&packet, sizeof(PACKET), 0);
                    }
                    else {  // found
                        packet.ptype = LOGEXIST;
                        send(client, (char*)&packet, sizeof(PACKET), 0);
                    }
                }
                break;
            }
            case LOGOUT: {
                // 로그아웃 브로드 캐스팅
                for (it = clients.begin(); it != clients.end(); it++) {
                    if (it->second == client)
                        packet.ptype = LOGOUT;
                    else
                        packet.ptype = OTHERLOGOUT;
                    send(it->second, (char*)&packet, sizeof(PACKET), 0);
                }
                memcpy(recvID, packet.data, packet.len);
                clients.erase(wstring(recvID));

                wsprintf(strTmp, L"%d", clients.size());
                SetWindowText(GetDlgItem(hDlg, IDC_STATIC_COUNT), strTmp);

                // ID를 찾아 리스트 박스에서 삭제
                unsigned int k = SendMessage(hList, LB_FINDSTRINGEXACT, 1, (LPARAM)recvID);
                SendMessage(hList, LB_DELETESTRING, k, 0);
                break;
            }
            default:
                break;
            }
            break;
        }

        case FD_CLOSE:
        {
            WCHAR strID[256];
            SOCKET client = (SOCKET)wParam;
            BOOL bFound = FALSE;

            // 닫은 소켓을 찾아 삭제
            for (it = clients.begin(); it != clients.end(); it++) {
                if (it->second == client) {
                    wcscpy_s(strID, it->first.c_str());
                    bFound = TRUE;
                    break;
                }
            }

            if (!bFound) // 로그인 안하고 연결을 끊은 경우
                break;

            // 로그인하고 연결을 끊은 경우, 로그아웃 브로드캐스팅
            for (it = clients.begin(); it != clients.end(); it++) {
                if (it->second != client) {
                    packet.ptype = OTHERLOGOUT;
                    packet.len = (unsigned short)(wcslen(strID) + 1) * 2;
                    memcpy(packet.data, strID, packet.len);
                    send(it->second, (char*)&packet, sizeof(PACKET), 0);
                }
            }
            clients.erase(wstring(strID));  // 소켓 삭제
            unsigned int k; // 리스트 박스에서 찾아 ID 삭제
            k = SendMessage(hList, LB_FINDSTRINGEXACT, 1, (LPARAM)strID);
            SendMessage(hList, LB_DELETESTRING, k, 0);

            wsprintf(strID, L"%d", clients.size());
            SetWindowText(GetDlgItem(hDlg, IDC_STATIC_COUNT), strID);
            closesocket(client);
        }
        break;
        }   // end of swtich (WSAGETSELECTEVENT(lParam))
        return TRUE;

    case WM_CLOSE:
        DestroyWindow(hDlg);
        return TRUE;

    case WM_DESTROY:
        closesocket(server);
        WSACleanup();
        return TRUE;
    }
    return FALSE;
}