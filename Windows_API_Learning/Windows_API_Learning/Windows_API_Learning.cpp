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
    static HFONT hFont;     // HFONT 자료형의 지역변수 hFont를 static 변수로 선언
    static HPEN hPen;       // HPEN 자료형의 지역변수 hFont를 static 변수로 선언
    static HBRUSH hBrush1, hBrush2;   // HBrush 자료형의 지역변수 hBrush, hBrush2를 static 변수로 선언
    //RECT rt = { 50, 300, 200, 450 };

    // Region
    RECT rt = { 50, 50, 200, 200 };
    static HBRUSH hBrush[3];
    static HRGN hRgn[3];
    static POINT pts[3] = { {200, 200}, {200, 300}, {300, 300} };

    // 래스터 연산
    static HBRUSH hRedBrush, hBlueBrush;

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

    case WM_CREATE: // WM_CREATE 메시지에서 
        // CreateFont 함수로 크기 100의 명조 폰트를 hFont에 생성함
        hFont = CreateFont(100, 0, 0, 0, FW_NORMAL, 0, 0, 0, 
            HANGEUL_CHARSET, 0, 0, 0, 0, _T("궁서"));

        // CreatePen 함수로 PS_DASH, 두께 1, 컬러 RGB(0, 0, 255)를 hPen에 생성함
        hPen = CreatePen(PS_DASH, 1, RGB(0, 0, 255));

        // CreateSolidBrush로 RGB(0, 0, 255) 붓 생성
        hBrush1 = CreateSolidBrush(RGB(0, 0, 255));
        // CreateHatchBrush로 HS_BDIAGONAL, RGB(0, 255, 0) 붓 생성
        hBrush2 = CreateHatchBrush(HS_BDIAGONAL, RGB(0, 255, 0));

        // Region 생성
        hBrush[0] = CreateSolidBrush(RGB(255, 0, 0));
        hBrush[1] = CreateSolidBrush(RGB(0, 255, 0));
        hBrush[2] = CreateSolidBrush(RGB(0, 0, 255));

        hRgn[0] = CreateRectRgnIndirect(&rt);
        hRgn[1] = CreateEllipticRgnIndirect(&rt);
        hRgn[2] = CreatePolygonRgn(pts, 3, ALTERNATE);

        // 래스터 연산
        hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
        hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));
        
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps; // 그리기 정보(Device Context, 지우기 정보, 갱신 영역 등)을 갖고 있으며, InvalidateRect와 InvalidateRgn 함수로 설정된다.
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            /*
            // 2. 폰트 생성
            HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);   // DC의 폰트를 hFont로 변경하고 기존 폰트를 반환하여 hOldFont에 저장
            TextOut(hdc, 100, 200, _T("안녕"), 4);    // 변환된 hFont로 "안녕"을 출력

            SelectObject(hdc, hOldFont); // DC의 폰트를 기존 디폴트 폰트로 되돌려 놓음
            TextOut(hdc, 100, 100, _T("Hello"), 5); // 디폴트 폰트로 되돌려 놓은 상태에서 "Hello"를 출력

            // 3. 펜 생성
            MoveToEx(hdc, 100, 100, NULL);
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen); // DC의 펜을 hPen으로 변경하고 기존 폰트를 반환하여 hOldPen에 저장
            LineTo(hdc, 100, 200);
            LineTo(hdc, 200, 200);

            SelectObject(hdc, hOldPen); // DC의 펜을 기존 디폴트 펜으로 되돌려 놓음
            LineTo(hdc, 200, 100);
            LineTo(hdc, 100, 100);

            // 4. 브러시 생성
            // #1
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
            Rectangle(hdc, 350, 50, 700, 400);
            SelectObject(hdc, hOldBrush);

            // #2
            HPEN hOldPen_ = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN)); // 테두리만 없앰
            Rectangle(hdc, 400, 100, 500, 200);
            SelectObject(hdc, hOldPen_);

            // #3
            hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH)); // 안에 색칠만 없앰
            Rectangle(hdc, 550, 100, 650, 200);
            SelectObject(hdc, hOldBrush);

            // #4
            hOldBrush = (HBRUSH)SelectObject(hdc, hBrush2);
            Rectangle(hdc, 400, 250, 500, 350);
            SelectObject(hdc, hOldBrush);

            // 5. SetDCPenColor, SetDCBrushColor, GetStockObject 함수
            SetDCPenColor(hdc, RGB(255, 0, 0)); // hdc의 Pen Color를 RGB(255, 0, 0)으로 설정
            SetDCBrushColor(hdc, RGB(0, 0, 255)); // hdc의 Brush Color를 RGB(0, 0, 255)으로 설정
            HPEN oldPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(DC_PEN)); // SetDCPenColor로 설정된 DC_PEN을 SelectObject로 사용
            HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(DC_BRUSH)); // SetDCBrush로 설정된 DC_BRUSH를 SelectObject로 사용
            Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom); // 위에서 설정한 좌표대로 사각형 그리기 (DC_PEN, DCBRUSH 사용)

            SelectObject(hdc, oldPen); // 이전 Pen으로 복귀
            SelectObject(hdc, oldBrush); // 이전 Brush로 복귀

            SetRect(&rt, 200, 450, 300, 550);
            FillRect(hdc, &rt, (HBRUSH)GetStockObject(DC_BRUSH)); // DC_BRUSH로 다시 색칠
            */

            // 6. 리전(region)
            /*for (int i = 0; i < 3; i++) {
                FillRgn(hdc, hRgn[i], hBrush[i]);
            }*/

            // 7. 래스터 연산
            // #1
            HPEN hOldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hRedBrush);
            Rectangle(hdc, 50, 50, 200, 200);
            SelectObject(hdc, hOldBrush);

            // #2
            SetROP2(hdc, R2_NOTXORPEN);
            hOldBrush = (HBRUSH)SelectObject(hdc, hBlueBrush);
            Rectangle(hdc, 150, 150, 300, 300);
            SelectObject(hdc, hOldBrush);

            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY: // Window 파괴 시 
        DeleteObject(hFont); // hFont 삭제
        DeleteObject(hPen); // hPen 삭제
        DeleteObject(hBrush1);
        DeleteObject(hBrush2);

        // Region
        for (int i = 0; i < 3; i++) {
            DeleteObject(hBrush[i]);
            DeleteObject(hRgn[i]);
        }

        // 래스터 연산
        DeleteObject(hRedBrush);
        DeleteObject(hBlueBrush);

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
