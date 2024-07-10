// Windows_API_Learning.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Windows_API_Learning.h"

#define MAX_LOADSTRING 100

// [step30_ex1] 체크 박스의 컬러 변경
/*#define IDC_BUTTON1 1001
#define IDC_BUTTON2 1002*/

// [step30_ex2] 그룹 박스와 라디오 버튼의 컬러 변경
/*enum {
    IDC_RED = 1001,
    IDC_GREEN, IDC_BLUE
};*/

// [step30_ex3] 정적 컨트롤과 에디트 컨트롤의 컬러와 폰트 변경
/*#define IDC_EDIT1   1001*/

// [step30_ex4] 리스트 박스의 컬러와 폰트 변경
#define IDC_LIST1   1001
LPCTSTR items[] = { _T("직선"), _T("타원"), _T("사각형") };

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
    // [step30_ex1] 체크 박스의 컬러 변경
    /*static HWND hC1, hC2;
    static HBRUSH hBrush1, hBrush2;*/

    // [step30_ex2] 그룹 박스와 라디오 버튼의 컬러 변경
    /*static HWND hR[3];
    static HBRUSH hBrush[4];*/

    // [step30_ex3] 정적 컨트롤과 에디트 컨트롤의 컬러와 폰트 변경
    /*static HWND     hEdit1, hStatic;
    static HBRUSH   hBrush1, hBrush2;
    static HFONT    hFont;*/

    // [step30_ex4] 리스트 박스의 컬러와 폰트 변경
    static HWND hList;
    static HBRUSH hBrush1;
    static HFONT hFont;

    switch (message)
    {
    case WM_CREATE:
        // [step30_ex1] 체크 박스의 컬러 변경
        /*hC1 = CreateWindowW(_T("BUTTON"), _T("check1"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
            10, 20, 100, 40, hWnd, (HMENU)IDC_BUTTON1, hInst, nullptr);
        hC2 = CreateWindowW(_T("BUTTON"), _T("check2"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
            10, 60, 100, 40, hWnd, (HMENU)IDC_BUTTON2, hInst, nullptr);

        hBrush1 = CreateSolidBrush(RGB(0, 0, 255));
        hBrush2 = CreateSolidBrush(RGB(255, 0, 0));*/

        // [step30_ex2] 그룹 박스와 라디오 버튼의 컬러 변경
        /*CreateWindowW(_T("BUTTON"), _T("Pen Color"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
            10, 10, 120, 200, hWnd, (HMENU)IDC_STATIC, hInst, nullptr);

        hR[0] = CreateWindowW(_T("BUTTON"), _T("RED"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
            20, 40, 100, 40, hWnd, (HMENU)IDC_RED, hInst, nullptr);
        hR[1] = CreateWindowW(_T("BUTTON"), _T("GREEN"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            20, 80, 100, 40, hWnd, (HMENU)IDC_GREEN, hInst, nullptr);
        hR[2] = CreateWindowW(_T("BUTTON"), _T("BLUE"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            20, 120, 100, 40, hWnd, (HMENU)IDC_BLUE, hInst, nullptr);
        SendMessage(hR[1], BM_SETCHECK, BST_CHECKED, 0);

        hBrush[0] = CreateSolidBrush(RGB(255, 0, 0));
        hBrush[1] = CreateSolidBrush(RGB(0, 255, 0));
        hBrush[2] = CreateSolidBrush(RGB(0, 0, 255));
        hBrush[3] = CreateSolidBrush(RGB(0, 255, 255));*/

        // [step30_ex3] 정적 컨트롤과 에디트 컨트롤의 컬러와 폰트 변경
        /*hStatic = CreateWindowW(_T("STATIC"), _T("COLOR"), WS_CHILD | WS_VISIBLE | WS_BORDER,
            20, 20, 200, 40, hWnd, (HMENU)IDC_STATIC, hInst, nullptr);
        hEdit1 = CreateWindowW(_T("EDIT"), _T("hello"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
            20, 80, 350, 250, hWnd, (HMENU)IDC_EDIT1, hInst, nullptr);

        hBrush1 = CreateSolidBrush(RGB(255, 0, 0));
        hBrush2 = CreateSolidBrush(RGB(0, 0, 255));
        hFont = CreateFont(40, 0, 0, 0, FW_NORMAL, 0, 0, 0,
            HANGEUL_CHARSET, 0, 0, 0, 0, _T("바탕"));

        SendMessage(hStatic, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hEdit1, WM_SETFONT, (WPARAM)hFont, FALSE);
        SetFocus(hEdit1);*/

        // [step30_ex4] 리스트 박스의 컬러와 폰트 변경
        hList = CreateWindowW(_T("LISTBOX"), NULL, WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_NOTIFY,
            20, 40, 120, 100, hWnd, (HMENU)IDC_LIST1, hInst, nullptr);
        for (int i = 0; i < 3; i++)
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)items[i]);
        SendMessage(hList, LB_SETCURSEL, 0, 0);
        hBrush1 = CreateSolidBrush(RGB(255, 0, 0));
        hFont = CreateFont(40, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("바탕"));
        SendMessage(hList, WM_SETFONT, (WPARAM)hFont, FALSE);
        break;
    case WM_CTLCOLORSTATIC: // for check box
    // [step30_ex1] 체크 박스의 컬러 변경
    /*{
        HWND ctrlWnd = (HWND)lParam;
        HDC hdc = (HDC)wParam;
        if (ctrlWnd == hC1)
        {
            SetTextColor(hdc, RGB(255, 0, 0));
            // SetTextColor (hdc, RGB(0, 0, 255));
            SetBkMode(hdc, TRANSPARENT);
            return (INT_PTR)hBrush1;
        }
        else
        {
            SetTextColor(hdc, RGB(0, 0, 255));
            SetBkMode(hdc, TRANSPARENT);
            return (INT_PTR)hBrush2;
        }
    }*/
    // [step30_ex2] 그룹 박스와 라디오 버튼의 컬러 변경
    /*{
        HWND ctrlWnd = (HWND)lParam;
        HDC hdc = (HDC)wParam;

        if (ctrlWnd == hR[0])
        {
            SetTextColor(hdc, RGB(255, 255, 0));
            // SetBkColor(hdc, RGB(255, 0, 0));
            SetBkMode(hdc, TRANSPARENT);
            return (INT_PTR)hBrush[0];
        }
        else if (ctrlWnd == hR[1])
        {
            SetTextColor(hdc, RGB(255, 255, 0));
            SetBkMode(hdc, TRANSPARENT);
            return (INT_PTR)hBrush[1];
        }
        else if (ctrlWnd == hR[2])
        {
            SetTextColor(hdc, RGB(255, 255, 0));
            SetBkMode(hdc, TRANSPARENT);
            return (INT_PTR)hBrush[2];
        }
        else
        {
            SetTextColor(hdc, RGB(255, 255, 0));
            SetBkMode(hdc, TRANSPARENT);
            return (INT_PTR)hBrush[3];
        }
    }*/
    // [step30_ex3] 정적 컨트롤과 에디트 컨트롤의 컬러와 폰트 변경
    /*{
        HWND ctrlWnd = (HWND)lParam;
        HDC hdc = (HDC)wParam;
        if (ctrlWnd == hStatic)
        {
            SetTextColor(hdc, RGB(0, 255, 0));
            SetBkMode(hdc, TRANSPARENT);
            return (INT_PTR)hBrush1;
        }
    }*/
    break;

    case WM_CTLCOLOREDIT:
    // [step30_ex3] 정적 컨트롤과 에디트 컨트롤의 컬러와 폰트 변경
    /* {
        HWND ctrlWnd = (HWND)lParam;
        HDC hdc = (HDC)wParam;
        if (ctrlWnd == hEdit1)
        {
            SetTextColor(hdc, RGB(255, 0, 0));
            SetBkColor(hdc, RGB(0, 0, 255));
            return (INT_PTR)hBrush2;
        }
    }*/
    break;

    case WM_CTLCOLORLISTBOX:
    // [step30_ex4] 리스트 박스의 컬러와 폰트 변경
    {
        HWND ctrlWnd = (HWND)lParam;
        HDC hdc = (HDC)wParam;
        if (ctrlWnd == hList)
        {
            SetTextColor(hdc, RGB(0, 0, 255));
            // SetBkColor(hdc, RGB(0, 255, 0));
            SetBkMode(hdc, TRANSPARENT);
            return (LRESULT)hBrush1;
        }
    }
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
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        // [step30_ex1] 체크 박스의 컬러 변경
        /*DeleteObject(hBrush1);
        DeleteObject(hBrush2);*/

        // [step30_ex2] 그룹 박스와 라디오 버튼의 컬러 변경
        /*for (int i = 0; i < 4; i++)
            DeleteObject(hBrush[i]);*/

        // [step30_ex3] 정적 컨트롤과 에디트 컨트롤의 컬러와 폰트 변경
        /*DeleteObject(hBrush1);
        DeleteObject(hBrush2);
        DeleteObject(hFont);*/

        // [step30_ex4] 리스트 박스의 컬러와 폰트 변경
        DeleteObject(hBrush1);
        DeleteObject(hFont);
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
