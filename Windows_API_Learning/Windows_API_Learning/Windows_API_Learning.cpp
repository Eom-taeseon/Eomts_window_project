// Windows_API_Learning.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Windows_API_Learning.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM); // [step32] 공통

// [step32_ex1] 메뉴 항목 선택 대화상자 1(비지역 변수와 부모 윈도우 핸들 사용)
/*int graph = IDM_RECT;*/

// [step32_ex2] 메뉴 항목 선택 대화상자 2(사용자 정의 메시지 전달)
/*int graph = IDM_RECT;
#define GRAPH_TYPE WM_USER+1    // 사용자 정의 메시지*/

// [step32_ex3] 모드리스 대화상자(사용자 정의 메시지 전달)
#define GRAPH_TYPE  WM_USER+1   // 사용자 정의 메시지
HWND hDlgMain;  // 대화상자 핸들, NULL 초기화

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
    // [step32_ex3] 모드리스 대화상자(사용자 정의 메시지 전달)
    static int graph = IDM_RECT;

    switch (message)
    {
    // [step32_ex2] 메뉴 항목 선택 대화상자 2(사용자 정의 메시지 전달)
    // [step32_ex3] 모드리스 대화상자(사용자 정의 메시지 전달)
    case GRAPH_TYPE:    // 사용자 정의 메시지 처리
        graph = (int)wParam;    // IDM_RECT, IDM_ELLIPSE
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            // [step32_ex1] 메뉴 항목 선택 대화상자 1(비지역 변수와 부모 윈도우 핸들 사용)
            // [step32_ex2] 메뉴 항목 선택 대화상자 2(사용자 정의 메시지 전달)
            // [step32_ex3] 모드리스 대화상자(사용자 정의 메시지 전달)
            case IDM_DIALOG1:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
                break;
            case IDM_RECT:
                graph = IDM_RECT;
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case IDM_ELLIPSE:
                graph = IDM_ELLIPSE;
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

            // [step32_ex1] 메뉴 항목 선택 대화상자 1(비지역 변수와 부모 윈도우 핸들 사용)
            // [step32_ex2] 메뉴 항목 선택 대화상자 2(사용자 정의 메시지 전달)
            if (graph == IDM_RECT)
                Rectangle(hdc, 50, 50, 300, 400);
            else
                Ellipse(hdc, 50, 50, 300, 400);

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

// [step32] 공통
// 정보 대화상자의 메시지 처리기
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hWndParent;
    switch (message)
    {
    case WM_INITDIALOG:
        hWndParent = GetParent(hDlg);   // 부모 윈도우
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON1:
            // [step32_ex1] 메뉴 항목 선택 대화상자 1(비지역 변수와 부모 윈도우 핸들 사용)
            //graph = IDM_RECT;

            // [step32_ex2] 메뉴 항목 선택 대화상자 2(사용자 정의 메시지 전달)
            //SendMessage(hWndParent, GRAPH_TYPE, (WPARAM)IDM_RECT, 0);

            // [step32_ex3] 모드리스 대화상자(사용자 정의 메시지 전달)
            SendMessage(hWndParent, GRAPH_TYPE, (WPARAM)IDM_RECT, 0);

            InvalidateRect(hWndParent, NULL, TRUE);
            break;
        case IDC_BUTTON2:
            // [step32_ex1] 메뉴 항목 선택 대화상자 1(비지역 변수와 부모 윈도우 핸들 사용)
            //graph = IDM_ELLIPSE;

            // [step32_ex2] 메뉴 항목 선택 대화상자 2(사용자 정의 메시지 전달)
            //SendMessage(hWndParent, GRAPH_TYPE, (WPARAM)IDM_ELLIPSE, 0);

            // [step32_ex3] 모드리스 대화상자(사용자 정의 메시지 전달)
            SendMessage(hWndParent, GRAPH_TYPE, (WPARAM)IDM_ELLIPSE, 0);

            InvalidateRect(hWndParent, NULL, TRUE);
            break;
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            break;
        }
        return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}