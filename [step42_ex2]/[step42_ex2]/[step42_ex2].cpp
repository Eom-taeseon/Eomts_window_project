// [step42_ex2].cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step42_ex2].h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

HWND hMainWnd;  // 윈도우 핸들
BOOL bThreadExitALL = TRUE; // thread 반복을 종료하기 위한 변수
DWORD WINAPI BallThreadProc(LPVOID lpParameter);

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
    LoadStringW(hInstance, IDC_STEP42EX2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STEP42EX2));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STEP42EX2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_STEP42EX2);
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
    static int nCount = 0;
    switch (message)
    {
    case WM_CREATE:
        hMainWnd = hWnd;
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_THREAD_CREATE: // 스레드 생성 메뉴 항목
                bThreadExitALL = TRUE;
                CreateThread(NULL, 0, BallThreadProc,
                    (LPVOID)nCount++, 0, 0);
                break;
            case IDM_THREAD_EXITALL:
                bThreadExitALL = FALSE;
                InvalidateRect(hWnd, NULL, TRUE);
                break;
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

#include <time.h>   // time
#define RADIUS 30   // 원의 반지름
DWORD WINAPI BallThreadProc(LPVOID lpParameter)
{
    int nCount = (int)lpParameter;
    HBRUSH ballBrush, oldBrush;
    RECT rect;
    int nx, ny;     // 원의 위치
    int dx, dy;     // 이동 벡터

    srand((unsigned)time(NULL));    // 난수 초기화
    dx = rand() % 3 + 1;
    dy = rand() % 3 + 1;

    HDC hdc = GetDC(hMainWnd);
    GetClientRect(hMainWnd, &rect);

    // 원이 나타나는 위치를 client 영역의 상단에서 랜덤하게
    nx = rand() % (rect.right - rect.left - 100) + 50;
    ny = RADIUS;

    ballBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
    SelectObject(hdc, GetStockObject(NULL_PEN));

    // 윈도우 프로시저에서 Thread/Thread Exit All 메뉴를 클릭할 때까지 반복
    while (bThreadExitALL)
    {
        GetClientRect(hMainWnd, &rect);

        // 이전 위치의 원을 흰색으로 지운다
        SelectObject(hdc, GetStockObject(WHITE_BRUSH));
        Ellipse(hdc, nx - RADIUS, ny - RADIUS, nx + RADIUS, ny + RADIUS);

        // 원의 이동 좌표 계산
        nx += dx;
        ny += dy;

        // client 영역의 바운드를 넘으면 방향 전환
        if (nx<RADIUS || nx>rect.right - rect.left - RADIUS)
            dx *= -1;
        if (ny<RADIUS || ny>rect.bottom - rect.top - RADIUS)
            dy *= -1;

        // 랜덤으로 생성해 놓은 브러시로 원을 그림
        oldBrush = (HBRUSH)SelectObject(hdc, ballBrush);
        Ellipse(hdc, nx - RADIUS, ny - RADIUS, nx + RADIUS, ny + RADIUS);
        SelectObject(hdc, oldBrush);
        GdiFlush(); // 스레드의 현재까지의 GDI 명령어를 모두 처리
        Sleep(20);  // 20 미리초 정지
    }
    DeleteObject(ballBrush);
    ReleaseDC(hMainWnd, hdc);
    return nCount;  // ExitThread(nCount);
}