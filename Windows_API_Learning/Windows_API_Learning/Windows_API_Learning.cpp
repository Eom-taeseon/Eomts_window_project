// Windows_API_Learning.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Windows_API_Learning.h"

#define MAX_LOADSTRING 100

// [step31_ex1] 푸시 버튼 오너 드로우 1 : WM_CTLCOLORBTN
/*#define IDC_BUTTON1 1001
#define IDC_BUTTON2 1002*/

// [step31_ex2] 푸시 버튼 오너 드로우 2 : WM_DRAWITEM
/*#define IDC_BUTTON1 1001
#define IDC_BUTTON2 1002*/

// [step31_ex3] 리스트 박스 오너 드로우
/*#define IDC_LIST1 1001
COLORREF colors[] = {
    RGB(255, 255, 255),
    RGB(255, 0, 0),
    RGB(0, 255, 0),
    RGB(0, 0, 255),
    RGB(0, 0, 0)
};*/

// [step31_ex4] 콤보 박스 오너 드로우
#define IDC_COMBO1 1001
COLORREF colors[] = {
    RGB(255, 255, 255),
    RGB(255, 0, 0),
    RGB(0, 255, 0),
    RGB(0, 0, 255),
    RGB(0, 0, 0)
};

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
    // [step31_ex1] 푸시 버튼 오너 드로우 1 : WM_CTLCOLORBTN
    /*static HWND hC1, hC2;
    static HBRUSH hBrush1, hBrush2;
    static int graph = IDC_BUTTON1;*/

    // [step31_ex2] 푸시 버튼 오너 드로우 2 : WM_DRAWITEM
    /*static HWND hC1, hC2;
    static int graph = IDC_BUTTON1;*/

    // [step31_ex3] 리스트 박스 오너 드로우
    /*static HWND hList;
    static COLORREF brushColor = colors[0];
    RECT rt = { 20, 250, 350, 400 };*/

    // [step31_ex4] 콤보 박스 오너 드로우
    static HWND hCombo;
    static COLORREF brushColor = colors[0];
    RECT rt = { 20, 250, 350, 400 };

    switch (message)
    {
    case WM_CREATE:
        // [step31_ex1] 푸시 버튼 오너 드로우 1 : WM_CTLCOLORBTN
        /*hC1 = CreateWindowW(_T("BUTTON"), _T("사각형"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, // 오너 드로우 버튼 hC1 생성
            10, 10, 100, 40, hWnd, (HMENU)IDC_BUTTON1, hInst, nullptr);
        hC2 = CreateWindowW(_T("BUTTON"), _T("타원"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,   // 오너 드로우 버튼 hC2 생성
            10, 60, 100, 40, hWnd, (HMENU)IDC_BUTTON2, hInst, nullptr);
        hBrush1 = CreateSolidBrush(RGB(0, 0, 255)); // hC1 컨트롤의 배경색 hBrush1 생성
        hBrush2 = CreateSolidBrush(RGB(0, 255, 0)); // hC2 컨트롤의 배경색 hBrush2 생성*/

        // [step31_ex2] 푸시 버튼 오너 드로우 2 : WM_DRAWITEM
        /*hC1 = CreateWindowW(_T("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
            10, 10, 100, 40, hWnd, (HMENU)IDC_BUTTON1, hInst, nullptr);
        hC2 = CreateWindowW(_T("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
            10, 60, 100, 40, hWnd, (HMENU)IDC_BUTTON2, hInst, nullptr);*/
           
        // [step31_ex3] 리스트 박스 오너 드로우
        /*hList = CreateWindowW(_T("LISTBOX"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | LBS_OWNERDRAWFIXED,
            20, 40, 150, 220, hWnd, (HMENU)IDC_LIST1, hInst, nullptr);
        for (int i = 0; i < 5; i++)
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)colors[i]);*/

        // [step31_ex4] 콤보 박스 오너 드로우
        hCombo = CreateWindowW(_T("COMBOBOX"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED,
            20, 40, 150, 220, hWnd, (HMENU)IDC_COMBO1, hInst, nullptr);
        for (int i = 0; i < 5; i++)
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)colors[i]);
        break;
    case WM_CTLCOLORBTN:
    // [step31_ex1] 푸시 버튼 오너 드로우 1 : WM_CTLCOLORBTN
    /* {
        HWND ctrlWnd = (HWND)lParam;
        HDC hdc = (HDC)wParam;
        RECT rt;
        GetClientRect(ctrlWnd, &rt);

        if (ctrlWnd == hC1)
        {
            SetTextColor(hdc, RGB(255, 0, 0));      // hC1 글자색 설정
            SetBkMode(hdc, TRANSPARENT);            // 글자 배경 모드를 TRANSPARENT로 설정
            DrawText(hdc, _T("사각형"), -1, &rt,    
                DT_CENTER | DT_VCENTER | DT_SINGLELINE);    // 화면 중앙에 "사각형" 출력
            return (INT_PTR)hBrush1;    // 컨트롤의 배경에 사용할 hBrush1 qksghks
        }

        else if (ctrlWnd == hC2)
        {
            SetTextColor(hdc, RGB(0, 0, 255));
            SetBkMode(hdc, TRANSPARENT);
            DrawText(hdc, _T("타원"), -1, &rt,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            return (INT_PTR)hBrush2;
        }
    }*/
    break;
    case WM_DRAWITEM:
    // [step31_ex2] 푸시 버튼 오너 드로우 2 : WM_DRAWITEM
    /*{
        LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;   // lParam을 pdis에 저장
        HDC hdc = pdis->hDC;    // pdis->hDC를 hdc에 저장
        RECT rt = pdis->rcItem; // GetClientRect(pdis->hwndItem, &rt);

        SetBkMode(hdc, TRANSPARENT);
        switch (pdis->CtlID)
        {
            case IDC_BUTTON1:
                if (pdis->itemState & ODS_SELECTED) {
                    SetTextColor(hdc, RGB(255, 0, 0));
                    FillRect(hdc, &rt, GetSysColorBrush(COLOR_HIGHLIGHT));
                    HPEN hPen = CreatePen(PS_INSIDEFRAME, 4, RGB(255, 0, 0));
                    HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
                    Rectangle(hdc, 0, 0, rt.right, rt.bottom);
                    SelectObject(hdc, oldPen);
                    DrawText(hdc, _T("사각형"), -1, &rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }

                else {
                    SetTextColor(hdc, RGB(0, 0, 255));
                    FillRect(hdc, &rt, GetSysColorBrush(COLOR_BTNSHADOW));
                    DrawText(hdc, _T("사각형"), -1, &rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }
                break;

            case IDC_BUTTON2:
                SetTextColor(hdc, RGB(0, 0, 255));
                if (pdis->itemState & ODS_SELECTED) {
                    SetTextColor(hdc, RGB(255, 0, 0));
                    FillRect(hdc, &rt, GetSysColorBrush(COLOR_HIGHLIGHT));
                    DrawText(hdc, _T("타원"), -1, &rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }
                else {
                    SetTextColor(hdc, RGB(0, 0, 255));
                    FillRect(hdc, &rt, GetSysColorBrush(COLOR_BTNFACE));
                    DrawText(hdc, _T("타원"), -1, &rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }
                break;
        }
    }*/

    // [step31_ex3] 리스트 박스 오너 드로우
    /* {
        LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
        HDC hdc = pdis->hDC;
        RECT rt = pdis->rcItem;
        COLORREF color = (COLORREF)pdis->itemData;  // colors[i]

        if (pdis->CtlID == IDC_LIST1)
        {
            HBRUSH hBrushBkg;
            if (pdis->itemState & ODS_SELECTED)
                hBrushBkg = GetSysColorBrush(COLOR_HIGHLIGHT);
            else
                hBrushBkg = GetSysColorBrush(COLOR_WINDOW);
            FillRect(hdc, &rt, hBrushBkg);

            HBRUSH hBrush = CreateSolidBrush(color);
            SetRect(&rt, rt.left + 2, rt.top + 2, rt.right - 2, rt.bottom - 2);
            FillRect(hdc, &rt, hBrush);
            DeleteObject(hBrush);
        }
        return TRUE;
    }*/

    // [step31_ex4] 콤보 박스 오너 드로우
    {
        LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
        HDC hdc = pdis->hDC;
        RECT rt = pdis->rcItem;
        COLORREF color = (COLORREF)pdis->itemData; // colors[i]

        if (pdis->CtlID == IDC_COMBO1)
        {
            HBRUSH hBrushBkg;
            if (pdis->itemState & ODS_SELECTED)
                hBrushBkg = GetSysColorBrush(COLOR_HIGHLIGHT);
            else
                hBrushBkg = GetSysColorBrush(COLOR_WINDOW);
            FillRect(hdc, &rt, hBrushBkg);

            HBRUSH hBrush = CreateSolidBrush(color);
            SetRect(&rt, rt.left + 2, rt.top + 2, rt.right - 2, rt.bottom - 2);
            FillRect(hdc, &rt, hBrush);
            DeleteObject(hBrush);
        }
        return TRUE;
    }
    break;
    case WM_MEASUREITEM:
    // [step31_ex3] 리스트 박스 오너 드로우
    /* {
        LPMEASUREITEMSTRUCT pmis = (LPMEASUREITEMSTRUCT)lParam;
        pmis->itemHeight = 40;
        return TRUE;
    }*/

    // [step31_ex4] 콤보 박스 오너 드로우
    {
        LPMEASUREITEMSTRUCT pmis = (LPMEASUREITEMSTRUCT)lParam;
        pmis->itemHeight = 40;
        return TRUE;
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        // [step31_ex1] 푸시 버튼 오너 드로우 1 : WM_CTLCOLORBTN
        /*case IDC_BUTTON1:
                graph = IDC_BUTTON1;
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case IDC_BUTTON2:
                graph = IDC_BUTTON2;
                InvalidateRect(hWnd, NULL, TRUE);
                break;*/

        // [step31_ex2] 푸시 버튼 오너 드로우 2 : WM_DRAWITEM
        /*case IDC_BUTTON1:
                    graph = IDC_BUTTON1;
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;
                case IDC_BUTTON2:
                    graph = IDC_BUTTON2;
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;*/

        // [step31_ex3] 리스트 박스 오너 드로우
        /*case IDC_LIST1:
                if (HIWORD(wParam) == LBN_SELCHANGE)
                {
                    int i;
                    i = (int)SendMessage((HWND)lParam, LB_GETCURSEL, 0, 0);
                    // brushColor = colors[i];
                    brushColor = (COLORREF)SendMessage((HWND)lParam, LB_GETITEMDATA, i, 0);
                    InvalidateRect(hWnd, NULL, TRUE);
                }
                break;*/
        
        // [step31_ex4] 콤보 박스 오너 드로우
        case IDC_COMBO1:
            if (HIWORD(wParam) == CBN_SELCHANGE)
            {
                int i;
                i = (int)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
                // brushColor = colors[i];
                brushColor = (COLORREF)SendMessage((HWND)lParam, CB_GETITEMDATA, i, 0);
                InvalidateRect(hWnd, NULL, TRUE);
            }
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

            // [step31_ex1] 푸시 버튼 오너 드로우 1 : WM_CTLCOLORBTN
            /*if (graph == IDC_BUTTON1)
                Rectangle(hdc, 150, 50, 400, 400);
            else
                Ellipse(hdc, 150, 50, 400, 400);*/

            // [step31_ex2] 푸시 버튼 오너 드로우 2 : WM_DRAWITEM
            /*if (graph == IDC_BUTTON1)
                Rectangle(hdc, 150, 50, 400, 400);
            else
                Ellipse(hdc, 150, 50, 400, 400);*/

            // [step31_ex3] 리스트 박스 오너 드로우
            /*HBRUSH hBrush = CreateSolidBrush(brushColor);
            HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

            Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom);

            SelectObject(hdc, oldBrush);
            DeleteObject(hBrush);*/

            // [step31_ex4] 콤보 박스 오너 드로우
            HBRUSH hBrush = CreateSolidBrush(brushColor);
            HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

            Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom);

            SelectObject(hdc, oldBrush);
            DeleteObject(hBrush);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        // [step31_ex1] 푸시 버튼 오너 드로우 1 : WM_CTLCOLORBTN
        /*DeleteObject(hBrush1);
        DeleteObject(hBrush2);*/
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
