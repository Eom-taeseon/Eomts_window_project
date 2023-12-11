// SandBox_1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "SandBox_1.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 전역변수(자체 설정)
#define STEP        10 // 스텝(=속도)
#define Player_Size 30 // 플레이어의 크기
#define Arrow_Size  60 // 방향 화살표 크기

static POINT    ptCircle;
static RECT     rtClient;
static int      direction = VK_RIGHT;

static POINT    ptArrow;
static POINT    pts[3];

void TimeProc(HWND hWnd, UINT uMsg, UINT_PTR nID, DWORD dwTime); // 타이머 프로시저

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
    LoadStringW(hInstance, IDC_SANDBOX1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SANDBOX1));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SANDBOX1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SANDBOX1);
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
    static HRGN hRgn; // 공격용 화살표의 영역범위
    static HBRUSH hBrush; // 공격용 화살표 시각화용 브러시 생성
    //static POINT pts[3] = {{200, 200}, {200, 300}, {300, 250}};
    

    TCHAR str[2][128];
    _stprintf_s(str[0], _T("%d, %d"), ptArrow.x, ptArrow.y);
    _stprintf_s(str[1], _T("%d, %d"), ptCircle.x, ptCircle.y);
    //TCHAR str[128] = _T("ptArrow.x");

    /*static POINT pts[3] = { {ptArrow.x, ptArrow.y - (Arrow_Size/2)}, {ptArrow.x, ptArrow.y + (Arrow_Size/2)}, {ptArrow.x+Arrow_Size, ptArrow.y}};*/

    switch (message)
    {
    case WM_CREATE:
        SetTimer(hWnd, 1, 20, (TIMERPROC)TimeProc);

        // 화살표 영역 생성
        hBrush = CreateSolidBrush(RGB(255, 0, 0));
        hRgn = CreatePolygonRgn(pts,3, ALTERNATE);

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

    case WM_SIZE:
        GetClientRect(hWnd, &rtClient);

        // 중앙에서 시작
        ptCircle.x = rtClient.right / 2;
        ptCircle.y = rtClient.bottom / 2;

        ptArrow.x = rtClient.right / 2;
        ptArrow.y = rtClient.bottom / 2;
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:
            direction = VK_LEFT;
            break;

        case VK_RIGHT:
            direction = VK_RIGHT;
            break;

        case VK_UP:
            direction = VK_UP;
            break;

        case VK_DOWN:
            direction = VK_DOWN;
            break;
        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            // 0. 플레이어 그리기
            Ellipse(hdc, ptCircle.x - Player_Size, ptCircle.y - Player_Size, 
                ptCircle.x + Player_Size, ptCircle.y + Player_Size);

            // 1. 화살표 그리기            
            FillRgn(hdc, hRgn, hBrush);
            MoveToEx(hdc, pts[0].x, pts[0].y, NULL);
            LineTo(hdc, pts[1].x, pts[1].y);
            LineTo(hdc, pts[2].x, pts[2].y);
            LineTo(hdc, pts[0].x, pts[0].y);
            TextOut(hdc, 100, 100, str[0], lstrlen(str[0]));
            TextOut(hdc, 100, 120, str[1], lstrlen(str[1]));

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
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

void TimeProc(HWND hWnd, UINT uMsg, UINT_PTR nID, DWORD dwTime)
{
    switch (direction)
    {
    case VK_LEFT:
        ptCircle.x -= STEP;
        
        // 화살표 생성
        ptArrow.x = ptCircle.x - Player_Size;
        ptArrow.y = ptCircle.y;

        ptArrow.x -= STEP;
        pts[0] = { ptArrow.x, ptArrow.y - (Arrow_Size / 2) };
        pts[1] = { ptArrow.x, ptArrow.y + (Arrow_Size / 2) };
        pts[2] = { ptArrow.x - Arrow_Size, ptArrow.y };

        if (ptCircle.x <= Player_Size)
            direction = VK_RIGHT;
        break;

    case VK_RIGHT:
        ptCircle.x += STEP;
        
        // 화살표 생성
        ptArrow.x = ptCircle.x + Player_Size;
        ptArrow.y = ptCircle.y;

        ptArrow.x += STEP;
        pts[0] = { ptArrow.x, ptArrow.y - (Arrow_Size / 2) };
        pts[1] = { ptArrow.x, ptArrow.y + (Arrow_Size / 2) };
        pts[2] = { ptArrow.x + Arrow_Size, ptArrow.y };

        if (ptCircle.x >= rtClient.right - Player_Size)
            direction = VK_LEFT;
        break;

    case VK_UP:
        ptCircle.y -= STEP;
        
        // 화살표 생성
        ptArrow.x = ptCircle.x;
        ptArrow.y = ptCircle.y - Player_Size;
        ptArrow.y -= STEP;
        pts[0] = { ptArrow.x - (Arrow_Size / 2), ptArrow.y };
        pts[1] = { ptArrow.x + (Arrow_Size / 2), ptArrow.y };
        pts[2] = { ptArrow.x, ptArrow.y - Arrow_Size };

        if (ptCircle.y <= Player_Size)
            direction = VK_DOWN;
        break;

    case VK_DOWN:
        ptCircle.y += STEP;
        
        // 화살표 생성
        ptArrow.x = ptCircle.x;
        ptArrow.y = ptCircle.y + Player_Size;
        ptArrow.y += STEP;
        pts[0] = { ptArrow.x - (Arrow_Size / 2), ptArrow.y };
        pts[1] = { ptArrow.x + (Arrow_Size / 2), ptArrow.y };
        pts[2] = { ptArrow.x, ptArrow.y + Arrow_Size };

        if (ptCircle.y >= rtClient.bottom - Player_Size)
            direction = VK_UP;
        break;

    default:
        break;
    }

    RECT rt;

    SetRect(&rt, ptCircle.x - Player_Size - 10, ptCircle.y - Player_Size - 10, 
        ptCircle.x + Player_Size + 10, ptCircle.y + Player_Size + 10);
    InvalidateRect(hWnd, NULL, TRUE); // 네모 부분만 매번 업데이트
}