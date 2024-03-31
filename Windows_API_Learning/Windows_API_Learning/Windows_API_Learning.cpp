// Windows_API_Learning.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Windows_API_Learning.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// [step14_ex2] 클라이언크 크기(400X400)를 갖는 윈도우 생성1
// [step14_ex3] 클라이언크 크기(400X400)를 갖는 윈도우 생성2
#define WIDTH 400
#define HEIGHT 400

// [step14_ex4] 하나의 블록의 크기가 SIZE X SIZE인 M X N 보드
#define SIZE 100
#define M 5
#define N 5
enum blockType { EMPTY, RED, BLUE };

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
    //wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSAPILEARNING);
    wcex.lpszMenuName   = NULL;
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

   // [step14_ex2] 클라이언크 크기(400X400)를 갖는 윈도우 생성1
   //RECT rtClient = { 0, 0, WIDTH, HEIGHT };
   //AdjustWindowRect(&rtClient, WS_OVERLAPPEDWINDOW, TRUE);

   //int nWidth = rtClient.right - rtClient.left;
   //int nHeight = rtClient.bottom - rtClient.top;
   //HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
   //    CW_USEDEFAULT, 0, nWidth, nHeight, nullptr, nullptr, hInstance, nullptr);

   // [step14_ex4] 하나의 블록의 크기가 SIZE X SIZE인 M X N 보드
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU,
       CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   // original
   //HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
   //   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
    // [step14_ex1] 윈도우 타이틀에 시간 출력
    SYSTEMTIME st;
    TCHAR szTime[128];

    // [step14_ex4] 하나의 블록의 크기가 SIZE X SIZE인 M X N 보드
    int ix, iy;
    int nWidth, nHeight;
    HBRUSH hBrush, oldBrush;
    static int board[M][N];

    switch (message)
    {
    case WM_CREATE:
        // [step14_ex1] 윈도우 타이틀에 시간 출력
        SetTimer(hWnd, 1, 1000, NULL);
        SendMessage(hWnd, WM_TIMER, 1, 0);

        // [step14_ex3] 클라이언크 크기(400X400)를 갖는 윈도우 생성2
        //{
        //    RECT rtClient = { 0, 0, WIDTH, HEIGHT };
        //    AdjustWindowRect(&rtClient, WS_OVERLAPPEDWINDOW, TRUE);

        //    int nWidth = rtClient.right - rtClient.left;
        //    int nHeight = rtClient.bottom - rtClient.top;
        //    MoveWindow(hWnd, 0, 0, nWidth, nHeight, TRUE);
        //}

        // [step14_ex4] 하나의 블록의 크기가 SIZE X SIZE인 M X N 보드
        {
            RECT rtClient = { 0, 0, SIZE * N, SIZE * M };
            AdjustWindowRect(&rtClient, WS_CAPTION | WS_SYSMENU, FALSE);
            
            nWidth = rtClient.right - rtClient.left;
            nHeight = rtClient.bottom - rtClient.top;
            SetWindowPos(hWnd, NULL, 0, 0, nWidth, nHeight, SWP_NOMOVE);
        }
        break;
    case WM_TIMER:
        // [step14_ex1] 윈도우 타이틀에 시간 출력
        GetLocalTime(&st);
        wsprintf(szTime, _T("%d-%d-%d : %d:%d:%d"),
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond
        );
        SetWindowText(hWnd, szTime);
        SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)szTime);
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

    case WM_LBUTTONDOWN:
    {
        ix = LOWORD(lParam) / SIZE;
        iy = HIWORD(lParam) / SIZE;
        if (ix >= N || iy >= M)
            break;

        board[iy][ix]++;
        if (board[iy][ix] > BLUE)
            board[iy][ix] = EMPTY;

        InvalidateRect(hWnd, NULL, TRUE); // 화면 초기화
    }
    break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            // [step14_ex2] 클라이언크 크기(400X400)를 갖는 윈도우 생성1
            //Ellipse(hdc, 0, 0, WIDTH, HEIGHT);

            // [step14_ex4] 하나의 블록의 크기가 SIZE X SIZE인 M X N 보드
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                {
                    int x = j * SIZE;
                    int y = i * SIZE;
                    Rectangle(hdc, x, y, x + SIZE, y + SIZE);
                    if (board[i][j] == EMPTY)
                        continue;

                    HPEN oldPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(NULL_PEN));

                    COLORREF color = RGB(0, 0, 0);
                    switch (board[i][j])
                    {
                    case RED:
                        color = RGB(255, 0, 0);
                        break;
                    case BLUE:
                        color = RGB(0, 0, 255);
                        break;
                    }

                    hBrush = CreateSolidBrush(color);
                    oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

                    Rectangle(hdc, x, y, x + SIZE, y + SIZE);
                    //Ellipse(hdc, x + 5, y + 5, x + SIZE - 5, y + SIZE - 5);

                    SelectObject(hdc, oldBrush);
                    SelectObject(hdc, oldPen);
                }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        // [step14_ex1] 윈도우 타이틀에 시간 출력
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
