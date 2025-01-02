// [step44_ex1_server].cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include "framework.h"
#include "[step44_ex1_server].h"

#include <string>       // wstring
#include <map>          // map STL
using namespace std;    // std::wstring, std::map

#define _WINSOCK_DEPRECATED_NO_WARNINGS // WSAAsyncSelect
#include <WinSock2.h>
#include <WS2tcpip.h>   // inet_pton
#pragma comment(lib, "Ws2_32.lib")

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:

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
#define BUFSIZE 256
typedef enum {
    LOGIN=1,
    LOGEXIST,
    LOGOUT,
    MYMSG
} PACKET_TYPE;
typedef struct {
    PACKET_TYPE ptype;
    unsigned short len; // length(data)
    char data[BUFSIZE];
}PACKET;

// 정보 대화상자의 메시지 처리기
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hList;
    static SOCKET server;
    static map<wstring, SOCKET> clients;
    map<wstring, SOCKET>::iterator it;
    PACKET packet;
    WCHAR recvID[40];
    WCHAR strTmp[40];

    switch (message)
    {
    case WM_INITDIALOG:
    {
        hList = GetDlgItem(hDlg, IDC_LIST1);
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
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
            break;
        }
        SetWindowText(hDlg, L"Listening...");
        WSAAsyncSelect(server, hDlg, WM_SERVER, FD_ACCEPT);
        return (INT_PTR)TRUE;
    }

    // wParam:socket
    // LOWORD(lParam) : network event, HIWORD(lParam) : error code
    case WM_SERVER:
        switch (WSAGETSELECTEVENT(lParam))   // LOWORD(lParam)
        {
        case FD_ACCEPT:
        {
            // FD_READ에서 ID확인 후 clients에 등록
            SOCKET client;
            client = accept(server, NULL, NULL);
            WSAAsyncSelect(client, hDlg, WM_SERVER,
                FD_READ | FD_CLOSE);
            break;
        }

        case FD_READ:   // 패킷 분석
        {
            SOCKET client = (SOCKET)wParam;
            int nR = recv(client, (char*)&packet, sizeof(PACKET), 0);

            switch (packet.ptype)
            {
            case LOGIN:
            {
                memcpy(recvID, packet.data, packet.len);
                if (clients.size() == 0)
                {
                    clients.insert(pair < wstring, SOCKET>(wstring(recvID), client));
                    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)recvID);
                    wsprintf(strTmp, L"%d", clients.size());
                    SetWindowText(GetDlgItem(hDlg, IDC_STATIC_COUNT), strTmp);
                }
                else
                {
                    it = clients.find(wstring(recvID));
                    if (it == clients.end()) // not found
                    {
                        clients.insert(pair<wstring, SOCKET>
                            (wstring(recvID), client));
                        SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)recvID);
                        wsprintf(strTmp, L"%d", clients.size());
                        SetWindowText(GetDlgItem(hDlg, IDC_STATIC_COUNT), strTmp);
                    }
                    else // found
                        packet.ptype = LOGEXIST;
                }

                // reply to client
                send(client, (char*)&packet, sizeof(PACKET), 0);
                break;
            }

            case LOGOUT:
            {
                // reply to client
                send(client, (char*)&packet, sizeof(PACKET), 0);
                // clients에서 삭제
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
            }   // end of switch (packet.ptype)
            break;
        }   // end of FD_READ

        case FD_CLOSE:
        {
            WCHAR strID[256];
            SOCKET client = (SOCKET)wParam;

            // 닫은 소켓을 찾아 삭제
            for (it = clients.begin(); it != clients.end();)
            {
                if (it->second == client)
                {
                    // memcpy(strID, it->first.c_str(), (it->first.length()+1) * 2);
                    wcscpy_s(strID, it->first.c_str());
                    clients.erase(it++);
                }
                else
                    it++;
            }

            unsigned int k; // 리스트 박스에서 찾아 ID 삭제
            k = SendMessage(hList, LB_FINDSTRINGEXACT, 1, (LPARAM)strID);
            SendMessage(hList, LB_DELETESTRING, k, 0);

            wsprintf(strTmp, L"%d", clients.size());
            SetWindowText(GetDlgItem(hDlg, IDC_STATIC_COUNT), strTmp);
            closesocket(client);
        }
        break;
        }
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