// Windows_API_Learning.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Windows_API_Learning.h"

#define MAX_LOADSTRING 100

// [step11_ex1] 마우스 왼쪽 버튼 클릭 위치에 원 그리기
#define R   50

// [step11_ex2] 마우스 드래그에 의한 직선 그리기
#define M   5

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
    // [step11_ex1] 마우스 왼쪽 버튼 클릭 위치에 원 그리기
    static POINT ptMouse;
    static BOOL bDraw_click = FALSE;

    // [step11_ex2] 마우스 드래그에 의한 직선 그리기
    static HPEN hBluePen;
    static BOOL bDraw_drag = FALSE;
    static POINT ptStart, ptEnd;

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
        hBluePen = CreatePen(PS_DASH, 1, RGB(0, 0, 255));
        break;

    case WM_LBUTTONDOWN:
        // [step11_ex1] 마우스 왼쪽 버튼 클릭 위치에 원 그리기
        //ptMouse.x = LOWORD(lParam);
        //ptMouse.y = HIWORD(lParam);
        //bDraw_click = TRUE;
        //InvalidateRect(hWnd, nullptr, TRUE);   // 흔적을 남김

        // [step11_ex2] 마우스 드래그에 의한 직선 그리기
        ptStart.x = LOWORD(lParam);
        ptStart.y = HIWORD(lParam);
        ptEnd = ptStart;
        break;

    case WM_LBUTTONUP:
        // [step11_ex2] 마우스 드래그에 의한 직선 그리기
        bDraw_drag = TRUE;
        InvalidateRect(hWnd, nullptr, TRUE);
        break;

    case WM_MOUSEMOVE:
        if (wParam & MK_LBUTTON)
        {
            // #1
            HDC hdc = GetDC(hWnd);
            int oldMode = SetROP2(hdc, R2_NOTXORPEN);

            HPEN hOldPen = (HPEN)SelectObject(hdc, hBluePen);
            MoveToEx(hdc, ptStart.x, ptStart.y, NULL);
            LineTo(hdc, ptEnd.x, ptEnd.y);

            // #2
            ptEnd.x = LOWORD(lParam);
            ptEnd.y = HIWORD(lParam);

            MoveToEx(hdc, ptStart.x, ptStart.y, NULL);
            LineTo(hdc, ptEnd.x, ptEnd.y);

            SelectObject(hdc, hOldPen);
            SetROP2(hdc, oldMode);
            ReleaseDC(hWnd, hdc);
        }
        break;

    case WM_KEYDOWN:
        break;

    case WM_CHAR:
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps; // 그리기 정보(Device Context, 지우기 정보, 갱신 영역 등)을 갖고 있으며, InvalidateRect와 InvalidateRgn 함수로 설정된다.
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            // [step11_ex1] 마우스 왼쪽 버튼 클릭 위치에 원 그리기
            /*if (bDraw)
                Ellipse(hdc, ptMouse.x - R, ptMouse.y - R, ptMouse.x + R, ptMouse.y + R);*/

            // [step11_ex2] 마우스 드래그에 의한 직선 그리기
            if (bDraw_drag)
            {
                MoveToEx(hdc, ptStart.x, ptStart.y, NULL);
                LineTo(hdc, ptEnd.x, ptEnd.y);
                Rectangle(hdc, ptStart.x - M, ptStart.y - M, ptStart.x + M, ptStart.y + M);
                Rectangle(hdc, ptEnd.x - M, ptEnd.y - M, ptEnd.x + M, ptEnd.y + M);
                
            }
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        DeleteObject(hBluePen);
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
