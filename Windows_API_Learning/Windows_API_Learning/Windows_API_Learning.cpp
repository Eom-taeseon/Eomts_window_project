// Windows_API_Learning.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Windows_API_Learning.h"

#define MAX_LOADSTRING 100

// [step29_ex1] 정적 컨트롤
//#define IDC_COUNTER 1001
//#define IDC_BUTTON1 1002
//#define IDC_BUTTON2 1003

// [step29_ex2] 정적 컨트롤에 비트맵 출력 
#define IDC_PICTURE1 1001

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
    // [step29_ex1] 정적 컨트롤
    /*static int  nCount = 10;
    static BOOL bTimer = FALSE;*/

    // [step29_ex2] 정적 컨트롤에 비트맵 출력
    static HBITMAP hBit;
    static BITMAP bit;
    HWND hPicture;

    switch (message)
    {
    case WM_CREATE:
        // [step29_ex1] 정적 컨트롤
        /*CreateWindowW(_T("STATIC"), _T("COUNTER"), WS_CHILD | WS_VISIBLE,   // _T("STATIC") 윈도우 클래스로 _T("COUNTER") 이름의 정적 컨트롤 생성
            20, 20, 70, 20, hWnd, (HMENU)IDC_STATIC, hInst, nullptr);
        
        CreateWindowW(_T("STATIC"), NULL, WS_CHILD | WS_VISIBLE,            // _T("STATIC") 윈도우 클래스로 변수 nCount를 출력하기 위한 정적 컨트롤 생성
            100, 20, 100, 20, hWnd, (HMENU)IDC_COUNTER, hInst, nullptr); 

        CreateWindowW(_T("BUTTON"), _T("Start"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, // _T("BUTTON") 윈도우 클래스로 _T("Start") 이름의 버튼 컨트롤 생성
            20, 60, 120, 40, hWnd, (HMENU)IDC_BUTTON1, hInst, nullptr);

        CreateWindowW(_T("BUTTON"), _T("Reset"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, // _T("BUTTON") 윈도우 클래스로 _T("Reset") 이름의 버튼 컨트롤 생성
            20, 120, 120, 40, hWnd, (HMENU)IDC_BUTTON2, hInst, nullptr);

        SetDlgItemInt(hWnd, IDC_COUNTER, nCount, FALSE);    // 변수 nCount의 값을 IDC_COUNTER에 출력
        */
        
        // [step29_ex2] 정적 컨트롤에 비트맵 출력
        hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1)); // IDB_BITMAP1을 불러와서
        GetObject(hBit, sizeof(BITMAP), &bit);  // 비트맵 hBit의 정보를 bit에 저장한다
        hPicture = CreateWindowW(_T("STATIC"), NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP | SS_NOTIFY,
            10, 10, bit.bmWidth, bit.bmHeight, hWnd, (HMENU)IDC_PICTURE1, hInst, NULL);
        SendMessage(hPicture, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBit);
        break;
    case WM_TIMER:
        // [step29_ex1] 정적 컨트롤
        /*nCount--;
        SetDlgItemInt(hWnd, IDC_COUNTER, nCount, FALSE);
        if (nCount == 0)
            SendDlgItemMessageW(hWnd, IDC_BUTTON2, BM_CLICK, 0, 0)*/
        break;
    case WM_SIZE:
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            // [step29_ex1] 정적 컨트롤
            /*case IDC_BUTTON1:   // _T("Start") 버튼
                if (bTimer)     // bTimer가 True라면 (= 타이머가 작동중이라면)
                {
                    bTimer = FALSE; // 타이머 해제
                    KillTimer(hWnd, 1);
                    SetDlgItemText(hWnd, IDC_BUTTON1, _T("Start")); // IDC_BUTTON의 텍스트를 Start로 복귀
                }
                else   // 그 외 == bTimer가 FALSE라면 == 타이머가 작동하지 않는다면
                {
                    bTimer = TRUE;  // 타이머 가동
                    SetTimer(hWnd, 1, 1000, NULL);  // 1 Timer 생성
                    SendMessage(hWnd, WM_TIMER, 1, 0); 
                    SetDlgItemText(hWnd, IDC_BUTTON1, _T("Stop"));  // IDC_BUTTON의 텍스트를 Stop으로 복귀
                }
                break;

            case IDC_BUTTON2:   // _T("Reset") 버튼
                bTimer = FALSE; // 타이머 해제
                nCount = 10;    // nCount 초기화
                KillTimer(hWnd, 1);
                SetDlgItemInt(hWnd, IDC_COUNTER, nCount, FALSE);
                SetDlgItemText(hWnd, IDC_BUTTON1, _T("Start")); // IDC_BUTTON1의 텍스트를 Start로 복귀
                break;
            */
                
            // [step29_ex2] 정적 컨트롤에 비트맵 출력
            case IDC_PICTURE1:
                if (HIWORD(wParam) == STN_CLICKED) // STN_DBLCLK
                    MessageBox(hWnd, _T("Click"), _T("Msg"), MB_OK);
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
        // [step29_ex1] 정적 컨트롤
        /*if (bTimer)
            KillTimer(hWnd, 1);*/
        // [step29_ex2] 정적 컨트롤에 비트맵 출력
        DeleteObject(hBit);
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
