// Windows_API_Learning.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Windows_API_Learning.h"

#define MAX_LOADSTRING 100

// [step15_ex1] MM_TEXT 모드, 원점 (cxClient/2, cyClient/2)
//#define SIZE 100

// [step15_ex2] MM_LOMETRIC 모드, 원점 변경
//#define SIZE 500

// [step15_ex3] MM_ISOMETIRC 모드, 원점 (cxClient/2, cyClient)
//#define WIDTH 400
//#define HEIGHT 400
//#define SIZE 100

// [step15_ex4] sine 그래프 그리기 MM_ISOTROPIC 모드, 원점 (cxClient / 2, cyClient / 2) STL vector 사용 sine 데이터 저장
#define _USE_MATH_DEFINES // for M_PI
#include <math.h>
#define RADIAN(X) ((X) * M_PI / 180)
#define WIDTH 600
#define HEIGHT 200
#define SCALE 100
#include <vector>   // for STL vector
using namespace std;

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSAPILEARNING, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSAPILEARNING));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSAPILEARNING));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSAPILEARNING);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // [step15_ex1] MM_TEXT 모드, 원점 (cxClient/2, cyClient/2)
    // [step15_ex2] MM_LOMETRIC 모드, 원점 변경
    // [step15_ex3] MM_ISOMETIRC 모드, 원점 (cxClient/2, cyClient)
    // [step15_ex4] sine 그래프 그리기 MM_ISOTROPIC 모드, 원점 (cxClient / 2, cyClient / 2) STL vector 사용 sine 데이터 저장
    static int cxClient, cyClient;

    // [step15_ex4] sine 그래프 그리기 MM_ISOTROPIC 모드, 원점 (cxClient / 2, cyClient / 2) STL vector 사용 sine 데이터 저장
    static vector<POINT> sine_points;

    switch (message)
    {
    case WM_CREATE:
    // [step15_ex3] MM_ISOMETIRC 모드, 원점 (cxClient/2, cyClient)
    /* {
        // WM_CREATE 메시지에서 윈도우의 Client 영역의 크기를 WIDTH, HEIGHT로 변경
        RECT rtClient = { 0, 0, WIDTH, HEIGHT };    // Client Area
        AdjustWindowRect(&rtClient, WS_OVERLAPPEDWINDOW, TRUE);

        int nWidth = rtClient.right - rtClient.left;
        int nHeight = rtClient.bottom - rtClient.top;
        SetWindowPos(hWnd, NULL, 0, 0, nWidth, nHeight, SWP_NOMOVE);
        MoveWindow(hWnd, 0, 0, nWidth, nHeight, TRUE);
    }*/

    // [step15_ex4] sine 그래프 그리기 MM_ISOTROPIC 모드, 원점 (cxClient / 2, cyClient / 2) STL vector 사용 sine 데이터 저장
    {
        RECT rtClient = { 0, 0, WIDTH, HEIGHT }; // Client Area
        AdjustWindowRect(&rtClient, WS_OVERLAPPEDWINDOW, TRUE);

        int nWidth = rtClient.right - rtClient.left;
        int nHeight = rtClient.bottom - rtClient.top;

        SetWindowPos(hWnd, NULL, 0, 0, nWidth, nHeight, SWP_NOMOVE);

        for (double t = -2 * M_PI - 1; t <= 2 * M_PI + 1; t += 0.1)
        {
            POINT pt;
            pt.x = round(t * SCALE);
            pt.y = (int)(sin(t) * SCALE);
            sine_points.push_back(pt);
        }
    }
    break;
    case WM_SIZE:
        // [step15_ex1] MM_TEXT 모드, 원점 (cxClient/2, cyClient/2)
        // [step15_ex2] MM_LOMETRIC 모드, 원점 변경
        // [step15_ex3] MM_ISOMETIRC 모드, 원점 (cxClient/2, cyClient)
        // [step15_ex4] sine 그래프 그리기 MM_ISOTROPIC 모드, 원점 (cxClient / 2, cyClient / 2) STL vector 사용 sine 데이터 저장
        // 클라이언트 크기를 cxClient, cyClinet에 저장
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        break;
    
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

        // [step15_ex1] MM_TEXT 모드, 원점 (cxClient/2, cyClient/2)
        /*
        //SetMapMode(hdc, MM_TEXT);
        SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL); // 뷰 포트의 원점을 클라이언트 영역의 중심 (cxClient / 2, cyClient / 2)로 변경함
        //SetWindowOrgEx(hdc, -cxClient / 2, -cyClient / 2, NULL);

        RECT rt;
        GetClientRect(hWnd, (LPRECT)&rt);   // 클라이언트 영역을 rt에 저장
        DPtoLP(hdc, (LPPOINT)&rt, 2);       // rt의 두 좌표 (left, top), (right, bottom)을 윈도우 좌표로 변환
        Rectangle(hdc, rt.left + 50, rt.top + 50, rt.right - 50, rt.bottom - 50);   // Rectangle 함수로 50만큼의 여백을 준 사각형 그림

        TCHAR str[128];
        wsprintf(str, _T("(%d, %d)"), rt.left + 50, rt.top + 50);
        DrawText(hdc, str, -1, &rt, DT_LEFT | DT_TOP);  // (rt.left, rt.top) 좌표를 문자열로 출력

        wsprintf(str, _T("(%d %d)"), rt.right - 50, rt.bottom - 50);
        DrawText(hdc, str, -1, &rt, DT_RIGHT | DT_BOTTOM | DT_SINGLELINE);  // (rt.right, rt.bottom) 좌표를 문자열로 출력

        // X-축
        MoveToEx(hdc, -SIZE, 0, NULL);
        LineTo(hdc, SIZE, 0);

        // Y-축
        MoveToEx(hdc, 0, -SIZE, NULL);
        LineTo(hdc, 0, SIZE);*/

        // [step15_ex2] MM_LOMETRIC 모드, 원점 변경
        /*SetMapMode(hdc, MM_LOMETRIC); // MM_HIMETRIC
        //SetViewportOrgEx(hdc, 0, cyClient, NULL);   // MM_LOMETRIC 매핑 모드에서 원점을 클라이언트 영역의 왼쪽 아래로 변경
        SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);    // MM_LOMETRIC 매핑 모드에서 원점을 클라이언트 영역의 가운데로 변경 (사용 X)

        //POINT pt;
        //pt.x = cxClient;
        //pt.y = cyClient;
        //DPtoLP(hdc, &pt, 1);
        //SetWindowOrgEx(hdc, -pt.x / 2, -pt.y / 2, NULL);  // MM_LOMETRIC 매핑 모드에서 클라이언트 영역을 pt에 저장한 뒤, 임의로 변경 (사용 X)

        RECT rt;
        GetClientRect(hWnd, (LPRECT)&rt);   // 클라이언트 영역을 rt에 저장
        DPtoLP(hdc, (LPPOINT)&rt, 2);       // rt의 두 좌표 (left, top), (right, bottom)을 윈도우 좌표로 변환
        Rectangle(hdc, rt.left + 50, rt.top - 50, rt.right - 50, rt.bottom + 50);   // Rectangle 함수로 50만큼의 여백을 준 사각형 그림

        TCHAR str[128];
        wsprintf(str, _T("(%d, %d)"), rt.left, rt.top);
        DrawText(hdc, str, -1, &rt, DT_LEFT | DT_TOP);  // (rt.left, rt.top) 좌표를 문자열로 출력

        wsprintf(str, _T("(%d %d)"), rt.right, rt.bottom);
        DrawText(hdc, str, -1, &rt, DT_RIGHT | DT_BOTTOM | DT_SINGLELINE);  // (rt.right, rt.bottom) 좌표를 문자열로 출력

        HPEN hPen = CreatePen(PS_SOLID, 10, RGB(0, 0, 255));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        // X-축
        MoveToEx(hdc, -SIZE, 0, NULL);
        LineTo(hdc, SIZE, 0);

        // Y-축
        MoveToEx(hdc, 0, -SIZE, NULL);
        LineTo(hdc, 0, SIZE);
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);*/

        // [step15_ex3] MM_ISOMETIRC 모드, 원점 (cxClient/2, cyClient)
        /*SetMapMode(hdc, MM_ISOTROPIC);  // SetMapMode 함수로 MM_ISOTROPIC 매핑 모드를 설정
        //SetMapMode(hdc, MM_ANISOTROPIC);

        // #1

        SetViewportOrgEx(hdc, 0, cyClient, NULL);   // org = (left, lower)
        SetWindowExtEx(hdc, 1000, 1000, NULL);
        SetViewportExtEx(hdc, cxClient, -cyClient, NULL); // #1-1
        //SetViewportExtEx(hdc, cxClient / 2, -cyClient / 2, NULL); // #1-2


        // #2
        SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL); // org=center
        SetWindowExtEx(hdc, 1000, 1000, NULL);
        SetViewportExtEx(hdc, cxClient, -cyClient, NULL);   // #2-1
        //SetViewportExtEx(hdc, cxClient / 2, -cyClient / 2, NULL);   // #2-2


        RECT rt;
        GetClientRect(hWnd, (LPRECT)&rt);
        DPtoLP(hdc, (LPPOINT)&rt, 2);

        Rectangle(hdc, rt.left + 50, rt.top - 50, rt.right - 50, rt.bottom + 50);

        TCHAR str[128];
        wsprintf(str, _T("(%d %d)"), rt.left, rt.top);
        DrawText(hdc, str, -1, &rt, DT_LEFT | DT_TOP);
        wsprintf(str, _T("(%d %d)"), rt.right, rt.bottom);
        DrawText(hdc, str, -1, &rt, DT_RIGHT | DT_BOTTOM | DT_SINGLELINE);

        HPEN hPen = CreatePen(PS_SOLID, 10, RGB(0, 0, 255));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        // X-축
        MoveToEx(hdc, -SIZE, 0, NULL);
        LineTo(hdc, SIZE, 0);

        // Y-축
        MoveToEx(hdc, 0, -SIZE, NULL);
        LineTo(hdc, 0, SIZE);

        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
        */

        // [step15_ex4] sine 그래프 그리기 MM_ISOTROPIC 모드, 원점 (cxClient / 2, cyClient / 2) STL vector 사용 sine 데이터 저장
        SetMapMode(hdc, MM_ISOTROPIC);
        SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);
        SetWindowExtEx(hdc, 10, 10, NULL);  // 
        SetViewportExtEx(hdc, 4, -4, NULL);
        //SetViewportExtEx(hdc, 10, -10, NULL);
        
        RECT rt;
        GetClientRect(hWnd, (LPRECT)&rt);
        DPtoLP(hdc, (LPPOINT)&rt, 2);
        //Rectangle(hdc, rt.left + 10, rt.top - 10, rt.right - 10, rt.bottom + 10);

        TCHAR str[128];
        wsprintf(str, _T("(%d %d)"), rt.left, rt.top);
        DrawText(hdc, str, -1, &rt, DT_LEFT | DT_TOP);

        wsprintf(str, _T("(%d %d)"), rt.right, rt.bottom);
        DrawText(hdc, str, -1, &rt, DT_RIGHT | DT_BOTTOM | DT_SINGLELINE);

        // X-축
        MoveToEx(hdc, rt.left, 0, NULL);
        LineTo(hdc, rt.right, 0);

        // Y-축
        MoveToEx(hdc, 0, rt.bottom, NULL);
        LineTo(hdc, 0, rt.top);

        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        for (int i = 0; i < sine_points.size(); i++)
        {
            POINT pt = sine_points[i];
            if (i == 0)
                MoveToEx(hdc, pt.x, pt.y, NULL);
            else
                LineTo(hdc, pt.x, pt.y);
        }

        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    
        EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

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
