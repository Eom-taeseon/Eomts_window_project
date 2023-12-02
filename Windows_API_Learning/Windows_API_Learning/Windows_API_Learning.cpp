// Windows_API_Learning.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Windows_API_Learning.h"

#define MAX_LOADSTRING 100

// [step12_ex1] 타이머에 의한 원 움직이기
#define STEP    10
#define R       50

// [step12_ex2] TimerProc에 의한 원 움직이기
POINT ptCircle;
RECT rtClient;
int direction = VK_RIGHT;

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// [step12_ex2] TimerProc에 의한 원 움직이기
// TimerProc 생성
void TimerProc(HWND, UINT, UINT_PTR, DWORD);

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

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; // 더블클릭 가능한 윈도우 스타일 등록 : CB_CBLCLKS
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSAPILEARNING));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL; // 메뉴 없애기
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, // POPUP 형식으로 설정하여 타이틀 바를 없앤다
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
    /*static POINT    ptCircle;
    static RECT     rtClient;
    static int      direction = VK_RIGHT;*/

    switch (message)
    {
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

    case WM_CREATE:
        //SetTimer(hWnd, 1, 10, nullptr); // 타이머 번호 : 1, 50밀리 초 단위로 작동, TimerProc는 없음
        SetTimer(hWnd, 1, 1, (TIMERPROC)TimerProc); // 타이머 번호 : 1, 50밀리 초 단위로 작동, TimerProc는 없음
        break;

    case WM_SIZE:
        GetClientRect(hWnd, &rtClient); 
        ptCircle.x = rtClient.right / 2;    // 중앙에서 시작
        ptCircle.y = rtClient.bottom / 2;   // 중앙에서 시작
        break;

    case WM_LBUTTONDOWN:
        break;

    case WM_LBUTTONUP:
        break;

    case WM_MOUSEMOVE:
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

    case WM_CHAR:
        break;

    case WM_TIMER:
        //switch (direction)
        //{
        //case VK_LEFT:
        //    ptCircle.x -= STEP;
        //    break;

        //case VK_RIGHT:
        //    ptCircle.x += STEP;
        //    break;

        //case VK_UP:
        //    ptCircle.y -= STEP;
        //    break;

        //case VK_DOWN:
        //    ptCircle.y += STEP;
        //    break;
        //}

        //// 경계를 점검하여 방향 변경
        //if (ptCircle.x - R <= 0)
        //{
        //    ptCircle.x = R;
        //    direction = VK_RIGHT;
        //}

        //if (ptCircle.x + R >= rtClient.right)
        //{
        //    ptCircle.x = rtClient.right - R;
        //    direction = VK_LEFT;
        //}

        //if (ptCircle.y - R <= 0)
        //{
        //    ptCircle.y = R;
        //    direction = VK_DOWN;
        //}

        //if (ptCircle.y + R >= rtClient.bottom)
        //{
        //    ptCircle.y = rtClient.bottom - R;
        //    direction = VK_UP;
        //}

        //RECT rt;
        //SetRect(&rt, ptCircle.x - R - 10, ptCircle.y - R - 10,
        //    ptCircle.x + R + 10, ptCircle.y + R + 10);
        //InvalidateRect(hWnd, &rt, TRUE);
        //InvalidateRect(hWnd, NULL, TRUE);   // 전체 영역 지우고 그리기
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps; // 그리기 정보(Device Context, 지우기 정보, 갱신 영역 등)을 갖고 있으며, InvalidateRect와 InvalidateRgn 함수로 설정된다.
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            Ellipse(hdc, ptCircle.x - R, ptCircle.y - R, ptCircle.x + R, ptCircle.y + R);

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

void TimerProc(HWND hWnd, UINT uMsg, UINT_PTR nID, DWORD dwTime)
{
    switch (direction)
    {
    case VK_LEFT:
        ptCircle.x -= STEP;
        if (ptCircle.x <= R)
            direction = VK_RIGHT;
        break;

    case VK_RIGHT:
        ptCircle.x += STEP;
        if (ptCircle.x >= rtClient.right - R)
            direction = VK_LEFT;
        break;

    case VK_UP:
        ptCircle.y -= STEP;
        if (ptCircle.y <= R)
            direction = VK_DOWN;
        break;

    case VK_DOWN:
        ptCircle.y += STEP;
        if (ptCircle.y >= rtClient.bottom - R)
            direction = VK_UP;
        break;
    }

    RECT rt;
    SetRect(&rt, ptCircle.x - R - 10, ptCircle.y - R - 10,
        ptCircle.x + R + 10, ptCircle.y + R + 10);
    InvalidateRect(hWnd, &rt, TRUE);
    //InvalidateRect(hWnd, nullptr, TRUE);
}