// Windows_API_Learning.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Windows_API_Learning.h"

#define MAX_LOADSTRING 100

// [step25_ex1] 푸시 버튼 컨트롤
//#define IDC_BUTTON1 1001
//#define IDC_BUTTON2 1002

// [step25_ex2] 체크 박스 컨트롤
//#define IDC_BUTTON1 1001

// [step25_ex3] 그룹 박스와 라디오 버튼 컨트롤
/*//#define IDC_STATIC -1 // resource.h에 이미 있음
#define IDC_NULL_PEN    1001
#define IDC_BLACK_PEN   1002
#define IDC_RED_PEN     1003
#define IDC_BLUE_PEN    1004
enum { IDC_NULL_BRUSH = 2001, IDC_WHITE_BRUSH, IDC_BLACK_BRUSH, IDC_BLUE_BRUSH };*/

// [step25_ex4] 비트맵 푸시 버튼 컨트롤과 BS_BITMAP
#define IDC_BUTTON1 1001
#define IDC_BUTTON2 1002

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
    // [step25_ex1] 푸시 버튼 컨트롤
    /*HWND hC1, hC2;
    static int graph = IDC_BUTTON1;*/

    // [step25_ex2] 체크 박스 컨트롤
    /*static HWND hC1;
    static BOOL bRect = TRUE;*/

    // [step25_ex3] 그룹 박스와 라디오 버튼 컨트롤
    /*static HWND hR[8];
    static POINT pts[2] = { {20, 250}, {300, 400} };
    static int penType = IDC_BLACK_PEN;
    static int brushType = IDC_WHITE_BRUSH;*/

    // [step25_ex4] 비트맵 푸시 버튼 컨트롤과 BS_BITMAP
    HWND hC1, hC2;
    static HBITMAP hBitmap1, hBitmap2;
    static int graph = IDC_BUTTON1;

    switch (message)
    {
    case WM_CREATE:
        // [step25_ex1] 푸시 버튼 컨트롤
        /*hC1 = CreateWindowW(_T("BUTTON"), _T("사각형"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 10, 100, 40, hWnd, (HMENU)IDC_BUTTON1, hInst, nullptr);
        hC2 = CreateWindowW(_T("BUTTON"), _T("타원"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 60, 100, 40, hWnd, (HMENU)IDC_BUTTON2, hInst, nullptr);*/

        // [step25_ex2] 체크 박스 컨트롤
        /*hC1 = CreateWindowW(_T("BUTTON"), _T("사각형"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
            10, 10, 100, 40, hWnd, (HMENU)IDC_BUTTON1, hInst, nullptr);
        SendMessage(hC1, BM_SETCHECK, BST_CHECKED, 0);*/

        // [step25_ex3] 그룹 박스와 라디오 버튼 컨트롤
        /*CreateWindowW(_T("BUTTON"), _T("Pen Color"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
            10, 10, 120, 200, hWnd, (HMENU)IDC_STATIC, hInst, nullptr);

        hR[0] = CreateWindowW(_T("BUTTON"), _T("NULL Pen"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
            20, 40, 100, 40, hWnd, (HMENU)IDC_NULL_PEN, hInst, nullptr);
        hR[1] = CreateWindowW(_T("BUTTON"), _T("BLACK"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            20, 80, 100, 40, hWnd, (HMENU)IDC_BLACK_PEN, hInst, nullptr);
        hR[2] = CreateWindowW(_T("BUTTON"), _T("RED"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            20, 120, 100, 40, hWnd, (HMENU)IDC_RED_PEN, hInst, nullptr);
        hR[3] = CreateWindowW(_T("BUTTON"), _T("BLUE"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            20, 160, 100, 40, hWnd, (HMENU)IDC_BLUE_PEN, hInst, nullptr);
        SendMessage(hR[1], BM_SETCHECK, BST_CHECKED, 0);

        CreateWindowW(_T("BUTTON"), _T("Brush Color"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
            140, 10, 120, 200, hWnd, (HMENU)IDC_STATIC, hInst, nullptr);
        hR[4] = CreateWindowW(_T("BUTTON"), _T("NULL Brush"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
            150, 40, 100, 40, hWnd, (HMENU)IDC_NULL_BRUSH, hInst, nullptr);
        hR[5] = CreateWindowW(_T("BUTTON"), _T("White"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            150, 80, 100, 40, hWnd, (HMENU)IDC_WHITE_BRUSH, hInst, nullptr);
        hR[6] = CreateWindowW(_T("BUTTON"), _T("Black"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            150, 120, 100, 40, hWnd, (HMENU)IDC_BLACK_BRUSH, hInst, nullptr);
        hR[7] = CreateWindowW(_T("BUTTON"), _T("Blue"), WS_CHILD |WS_VISIBLE|BS_AUTORADIOBUTTON,
            150, 160, 100, 40, hWnd, (HMENU)IDC_BLUE_BRUSH, hInst, nullptr);
        SendMessage(hR[5], BM_SETCHECK, BST_CHECKED, 0);*/

        // [step25_ex4] 비트맵 푸시 버튼 컨트롤과 BS_BITMAP
    {
        hC1 = CreateWindowW(_T("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
            10, 10, 100, 40, hWnd, (HMENU)IDC_BUTTON1, hInst, nullptr);
        hC2 = CreateWindowW(_T("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
            10, 60, 100, 40, hWnd, (HMENU)IDC_BUTTON2, hInst, nullptr);

        // hC1에 생성한 비트맵 설정
        RECT rt;
        GetClientRect(hC1, &rt);
        HDC hDC = GetDC(hWnd);
        HDC memDC = CreateCompatibleDC(hDC);
        hBitmap1 = CreateCompatibleBitmap(hDC, rt.right, rt.bottom);
        SelectObject(memDC, hBitmap1);
        SetDCBrushColor(memDC, RGB(0, 0, 255));

        SetRect(&rt, rt.left + 5, rt.top + 5, rt.right - 5, rt.bottom - 5);
        FillRect(memDC, &rt, (HBRUSH)GetStockObject(DC_BRUSH));

        SetBkMode(memDC, TRANSPARENT);
        SetTextColor(memDC, RGB(255, 0, 0));
        DrawText(memDC, _T("사각형"), -1, &rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        DeleteDC(memDC);
        ReleaseDC(hWnd, hDC);
        SendMessage(hC1, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap1);

        // hC2에 로드한 비트맵 설정
        hBitmap2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
        SendMessage(hC2, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap2);
    }
        break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            // [step25_ex1] 푸시 버튼 컨트롤
            /*case IDC_BUTTON1:
                graph = IDC_BUTTON1;
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case IDC_BUTTON2:
                graph = IDC_BUTTON2;
                InvalidateRect(hWnd, NULL, TRUE);
                break;*/

            // [step25_ex2] 체크 박스 컨트롤
            /*case IDC_BUTTON1: // (HWND)lParam == hC1
                bRect = SendMessage(hC1, BM_GETCHECK, 0, 0) == BST_CHECKED;
                InvalidateRect(hWnd, NULL, TRUE);
                break;*/

            // [step25_ex3] 그룹 박스와 라디오 버튼 컨트롤
            /*case IDC_NULL_PEN:
                penType = IDC_NULL_PEN;
                break;
            case IDC_BLACK_PEN:
                penType = IDC_BLACK_PEN;
                break;
            case IDC_RED_PEN:
                penType = IDC_RED_PEN;
                break;
            case IDC_BLUE_PEN:
                penType = IDC_BLUE_PEN;
                break;
            
            case IDC_NULL_BRUSH:
                brushType = IDC_NULL_BRUSH;
                break;
            case IDC_WHITE_BRUSH:
                brushType = IDC_WHITE_BRUSH;
                break;
            case IDC_BLACK_BRUSH:
                brushType = IDC_BLACK_BRUSH;
                break;
            case IDC_BLUE_BRUSH:
                brushType = IDC_BLUE_BRUSH;
                break;*/

            // [step25_ex4] 비트맵 푸시 버튼 컨트롤과 BS_BITMAP
            case IDC_BUTTON1:
                graph = IDC_BUTTON1;
                InvalidateRect(hWnd, NULL, TRUE);
                break;

            case IDC_BUTTON2:
                graph = IDC_BUTTON2;
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
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            // [step25_ex1] 푸시 버튼 컨트롤
            /*if (graph == IDC_BUTTON1)
                Rectangle(hdc, 150, 50, 400, 400);
            else
                Ellipse(hdc, 150, 50, 400, 400);*/

            // [step25_ex2] 체크 박스 컨트롤
            /*if (bRect)
                Rectangle(hdc, 150, 50, 400, 400);
            else
                Ellipse(hdc, 150, 50, 400, 400);*/

            // [step25_ex3] 그룹 박스와 라디오 버튼 컨트롤
            /*HPEN hPen{};
            HBRUSH hBrush;

            switch (penType)
            {
            case IDC_NULL_PEN:
                hPen = (HPEN)GetStockObject(NULL_PEN);
                break;
            case IDC_RED_PEN:
                hPen = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
                break;
            case IDC_BLUE_PEN:
                hPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 255));
                break;
            case IDC_BLACK_PEN:
                hPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
                break;
            }

            switch (brushType)
            {
            case IDC_NULL_BRUSH:
                hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
                break;
            case IDC_WHITE_BRUSH:
                // hBrush = CreateSolidBrush(RGB(255, 255, 255));
                hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
                break;
            case IDC_BLUE_BRUSH:
                hBrush = CreateSolidBrush(RGB(0, 0, 255));
                break;
            default:    // IDC_BLACK_BRUSH
                // hBrush = CreateSolidBrush(RGB(0, 0, 0));
                hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
                break;
            }

            HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
            HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

            Rectangle(hdc, pts[0].x, pts[0].y, pts[1].x, pts[1].y);

            SelectObject(hdc, oldPen);
            DeleteObject(hPen);
            SelectObject(hdc, oldBrush);
            DeleteObject(hBrush);*/

            // [step25_ex4] 비트맵 푸시 버튼 컨트롤과 BS_BITMAP
            if (graph == IDC_BUTTON1)
                Rectangle(hdc, 150, 50, 400, 400);
            else
                Ellipse(hdc, 150, 50, 400, 400);

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
