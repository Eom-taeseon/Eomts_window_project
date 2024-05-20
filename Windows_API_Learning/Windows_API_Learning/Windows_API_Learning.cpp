// Windows_API_Learning.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Windows_API_Learning.h"

#define MAX_LOADSTRING 100

// [step17_ex2] 더블 버퍼링 2(타이머에 의한 원 움직이기)
/*#define STEP 10
#define R 50*/

// [step17_ex3] 더블 버퍼링 3(마우스 사각형 선택, 이동, 크기 변경)
typedef enum {
    OUTSIDE, INSIDE,
    LEFT_TOP, LEFT_BOTTOM, RIGHT_TOP, RIGHT_BOTTOM
}LOCATION_TYPE;

typedef struct {
    RECT rect;
    COLORREF penColor;
    COLORREF brushColor;
    BOOL bSelected;
}RECT_OBJECT;

#define M 10

void SetCornersRect(RECT rt, RECT rtCorners[])
{
    SetRect(&rtCorners[0], rt.left - M, rt.top - M, rt.left + M, rt.top + M);
    SetRect(&rtCorners[1], rt.right - M, rt.top - M, rt.right + M, rt.top + M);
    SetRect(&rtCorners[2], rt.left - M, rt.bottom - M, rt.left + M, rt.bottom + M);
    SetRect(&rtCorners[3], rt.right - M, rt.bottom - M, rt.right + M, rt.bottom + M);
}

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
    // [step17_ex1] 더블 버퍼링 1
    // static int cxClient, cyClient;
    /*static RECT rtClient;
    static HDC memDC;
    static HBITMAP hBit;*/

    // [step17_ex2] 더블 버퍼링 2(타이머에 의한 원 움직이기)
    /*static HDC memDC;
    static HBITMAP hBit;

    static POINT ptCircle;
    static RECT rtClient;
    static int direction = VK_RIGHT;*/

    // [step17_ex3] 더블 버퍼링 3(마우스 사각형 선택, 이동, 크기 변경)
    static RECT rtClient;
    static HDC memDC;
    static HBITMAP hBit;

    static RECT_OBJECT object;
    static RECT rtMove;
    static LOCATION_TYPE clickLoc;
    static POINT ptMouseDown;

    POINT ptMouseMove, ptOffset;
    RECT rt;
    HPEN hPen, hOldPen;
    HBRUSH hBrush, hOldBrush;

    switch (message)
    {
    case WM_CREATE:
        // [step17_ex2] 더블 버퍼링 2(타이머에 의한 원 움직이기)
        //SetTimer(hWnd, 1, 50, NULL);

        // [step17_ex3] 더블 버퍼링 3(마우스 사각형 선택, 이동, 크기 변경)
        object.rect = { 100, 00, 400, 400 };
        object.penColor = RGB(0, 0, 0);
        object.brushColor = RGB(255, 255, 0);
        object.bSelected = FALSE;
        break;

    case WM_SIZE:
        // [step17_ex1] 더블 버퍼링 1
        /* {
            //cxClient = LOWORD(lParam);
            //cyClient = HIWORD(lParam);

            GetClientRect(hWnd, &rtClient);

            HDC hdc = GetDC(hWnd);
            if (hBit)
                DeleteObject(hBit);
            hBit = CreateCompatibleBitmap(hdc, rtClient.right, rtClient.bottom);

            if (memDC)
                DeleteDC(memDC);
            memDC = CreateCompatibleDC(hdc);
            ReleaseDC(hWnd, hdc);
            SelectObject(memDC, hBit);
        }*/

        // [step17_ex2] 더블 버퍼링 2(타이머에 의한 원 움직이기)
        /* {
            GetClientRect(hWnd, &rtClient);
            ptCircle.x = rtClient.right / 2;
            ptCircle.y = rtClient.bottom / 2;

            HDC hdc = GetDC(hWnd);
            if (hBit)
                DeleteObject(hBit);
            hBit = CreateCompatibleBitmap(hdc, rtClient.right, rtClient.bottom);

            if (memDC)
                DeleteDC(memDC);
            memDC = CreateCompatibleDC(hdc);
            ReleaseDC(hWnd, hdc);
            SelectObject(memDC, hBit);
        }*/

        // [step17_ex3] 더블 버퍼링 3(마우스 사각형 선택, 이동, 크기 변경)
        {
            GetClientRect(hWnd, &rtClient);
            HDC hdc = GetDC(hWnd);
            if (hBit)
                DeleteObject(hBit);
            hBit = CreateCompatibleBitmap(hdc, rtClient.right, rtClient.bottom);

            if (memDC)
                DeleteDC(memDC);
            memDC = CreateCompatibleDC(hdc);

            ReleaseDC(hWnd, hdc);
            SelectObject(memDC, hBit);
        }
        break;
    case WM_KEYDOWN:
        // [step17_ex2] 더블 버퍼링 2(타이머에 의한 원 움직이기)
        /*switch (wParam)
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
        }*/
        break;
    case WM_TIMER:
        // [step17_ex2] 더블 버퍼링 2(타이머에 의한 원 움직이기)
        /*switch (direction)
        {
        case VK_LEFT:
            ptCircle.x -= STEP;
            break;
        case VK_RIGHT:
            ptCircle.x += STEP;
            break;
        case VK_UP:
            ptCircle.y -= STEP;
            break;
        case VK_DOWN:
            ptCircle.y += STEP;
            break;
        }
        // check the boundaries and change the direction
        if (ptCircle.x - R <= 0)
        {
            ptCircle.x = R;
            direction = VK_RIGHT;
        }
        if (ptCircle.x + R >= rtClient.right)
        {
            ptCircle.x = rtClient.right - R;
            direction = VK_LEFT;
        }
        if (ptCircle.y - R <= 0)
        {
            ptCircle.y = R;
            direction = VK_DOWN;
        }
        if (ptCircle.y + R >= rtClient.bottom)
        {
            ptCircle.y = rtClient.bottom - R;
            direction = VK_UP;
        }
        InvalidateRect(hWnd, NULL, FALSE);*/
        break;
    case WM_LBUTTONDOWN:
        ptMouseDown.x = LOWORD(lParam);
        ptMouseDown.y = HIWORD(lParam);

        if (object.bSelected == FALSE)  // 기존 선택이 없는 경우
        {
            if (PtInRect(&object.rect, ptMouseDown))
            {
                object.bSelected = TRUE;
                clickLoc = INSIDE;
                rtMove = object.rect;
            }
            else
            {
                object.bSelected = FALSE;
                clickLoc = OUTSIDE;
            }
        }

        else // 기존 선택이 있는 경우
        {
            RECT rtCorners[4];
            SetCornersRect(object.rect, rtCorners);
            if (PtInRect(&rtCorners[0], ptMouseDown))
                clickLoc = LEFT_TOP;
            else if (PtInRect(&rtCorners[1], ptMouseDown))
                clickLoc = RIGHT_TOP;
            else if (PtInRect(&rtCorners[2], ptMouseDown))
                clickLoc = LEFT_BOTTOM;
            else if (PtInRect(&rtCorners[3], ptMouseDown))
                clickLoc = RIGHT_BOTTOM;
            else if (PtInRect(&object.rect, ptMouseDown))
                clickLoc = INSIDE;
            else
                clickLoc = OUTSIDE;
            
            if (clickLoc == OUTSIDE)
                object.bSelected = FALSE;
        }
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_MOUSEMOVE:
        ptMouseMove.x = LOWORD(lParam);
        ptMouseMove.y = HIWORD(lParam);
        if (!(wParam & MK_LBUTTON) || !object.bSelected)
            break;

        ptOffset.x = ptMouseMove.x - ptMouseDown.x;
        ptOffset.y = ptMouseMove.y - ptMouseDown.y;
        rt = object.rect;
        
        switch (clickLoc)
        {
        case INSIDE:
            SetRect(&rtMove, rt.left + ptOffset.x, rt.top + ptOffset.y, rt.right + ptOffset.x, rt.bottom + ptOffset.y);
            break;
        case LEFT_TOP:
            SetRect(&rtMove, rt.left + ptOffset.x, rt.top + ptOffset.y, rt.right, rt.bottom);
            break;
        case LEFT_BOTTOM:
            SetRect(&rtMove, rt.left + ptOffset.x, rt.top, rt.right, rt.bottom + ptOffset.y);
            break;
        case RIGHT_TOP:
            SetRect(&rtMove, rt.left, rt.top + ptOffset.y, rt.right + ptOffset.x, rt.bottom);
            break;
        case RIGHT_BOTTOM:
            SetRect(&rtMove, rt.left, rt.top, rt.right + ptOffset.x, rt.bottom + ptOffset.y);
            break;
        }
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_LBUTTONUP:
        if (object.bSelected)
        {
            // 좌표 순서 재조정
            if (rtMove.left > rtMove.right)  // swap
            {
                int tmp;
                tmp = rtMove.left;
                rtMove.left = rtMove.right;
                rtMove.right = tmp;
            }

            if (rtMove.top > rtMove.bottom)  // swap
            {
                int tmp;
                tmp = rtMove.top;
                rtMove.top = rtMove.bottom;
                rtMove.bottom = tmp;
            }
            object.rect = rtMove;
        }
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_ERASEBKGND:
        // [step17_ex1] 더블 버퍼링 1
        // [step17_ex2] 더블 버퍼링 2(타이머에 의한 원 움직이기)
        // [step17_ex3] 더블 버퍼링 3(마우스 사각형 선택, 이동, 크기 변경)
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

            // [step17_ex1] 더블 버퍼링 1
            /*/FillRect(memDC, &rtClient, (HBRUSH)(COLOR_WINDOW + 2));
            Rectangle(memDC, 100, 100, 200, 200);   // 그리기

            BitBlt(hdc, 0, 0, rtClient.right, rtClient.bottom, memDC, 0, 0, SRCCOPY);   // memDC -> hdc*/

            // [step17_ex2] 더블 버퍼링 2(타이머에 의한 원 움직이기)
            /*FillRect(memDC, &rtClient, (HBRUSH)(COLOR_WINDOW + 2));

            // draw circle
            HPEN hPen, hOldPen;
            HBRUSH hBrush, hOldBrush;

            hPen = CreatePen(PS_INSIDEFRAME, 10, RGB(255, 0, 0));
            hBrush = CreateSolidBrush(RGB(0, 0, 255));

            hOldPen = (HPEN)SelectObject(memDC, hPen);
            hOldBrush = (HBRUSH)SelectObject(memDC, hBrush);

            Ellipse(memDC, ptCircle.x - R, ptCircle.y - R, ptCircle.x + R, ptCircle.y + R);

            SelectObject(memDC, hOldPen);
            SelectObject(memDC, hOldBrush);
            DeleteObject(hPen);
            DeleteObject(hBrush);

            BitBlt(hdc, 0, 0, rtClient.right, rtClient.bottom, memDC, 0, 0, SRCCOPY); // memDC -> hdc
            */

            // [step17_ex3] 더블 버퍼링 3(마우스 사각형 선택, 이동, 크기 변경)
            FillRect(memDC, &rtClient, (HBRUSH)(COLOR_WINDOW + 2));

            if (clickLoc != OUTSIDE)  // draw rtMove
            {
                hPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
                hBrush = CreateSolidBrush(RGB(255, 255, 255));
                hOldPen = (HPEN)SelectObject(memDC, hPen);
                hOldBrush = (HBRUSH)SelectObject(memDC, hBrush);

                Rectangle(memDC, rtMove.left, rtMove.top, rtMove.right, rtMove.bottom);

                SelectObject(memDC, hOldPen);
                SelectObject(memDC, hOldBrush);
                DeleteObject(hPen);
                DeleteObject(hBrush);
            }

            // draw object
            hPen = CreatePen(PS_SOLID, 1, object.penColor);
            hBrush = CreateSolidBrush(object.brushColor);

            hOldPen = (HPEN)SelectObject(memDC, hPen);
            hOldBrush = (HBRUSH)SelectObject(memDC, hBrush);
            
            rt = object.rect;
            Rectangle(memDC, rt.left, rt.top, rt.right, rt.bottom);

            SelectObject(memDC, hOldPen);
            SelectObject(memDC, hOldBrush);
            DeleteObject(hPen);
            DeleteObject(hBrush);

            if (object.bSelected) // draw corners
            {
                hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
                hBrush = CreateSolidBrush(RGB(0, 0, 200));

                hOldPen = (HPEN)SelectObject(memDC, hPen);
                hOldBrush = (HBRUSH)SelectObject(memDC, hBrush);

                RECT rtCorners[4];
                SetCornersRect(object.rect, rtCorners);
                for (int i = 0; i < 4; i++)
                {
                    Rectangle(memDC, rtCorners[i].left, rtCorners[i].top, rtCorners[i].right, rtCorners[i].bottom);
                }

                SelectObject(memDC, hOldPen);
                SelectObject(memDC, hOldBrush);
                DeleteObject(hPen);
                DeleteObject(hBrush);
            }

            BitBlt(hdc, 0, 0, rtClient.right, rtClient.bottom, memDC, 0, 0, SRCCOPY);   // memDC -> hdc
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        // [step17_ex1] 더블 버퍼링 1
        // [step17_ex3] 더블 버퍼링 3(마우스 사각형 선택, 이동, 크기 변경)
        DeleteDC(memDC);
        DeleteObject(hBit);
        // [step17_ex2] 더블 버퍼링 2(타이머에 의한 원 움직이기)
        //KillTimer(hWnd, 1);
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
