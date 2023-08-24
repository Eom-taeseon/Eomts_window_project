// Eomts_window_project.cpp : 애플리케이션에 대한 진입점을 정의합니다
//

#include "framework.h"
#include "Eomts_window_project.h"

#include "struct.h"
#include "BrushNPen.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;												// 현재 인스턴스입니다.
HWND _main_hWnd;
WCHAR szTitle[MAX_LOADSTRING];						// 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];		// 기본 창 클래스 이름입니다.
RECT _drawPos = { 50, 550, 100, 350 };
TCHAR _szStringBuffer[MAX_PATH] = { 0, };
POINT _pos = { 570, 430 };

BOOL _isDraw = FALSE;

// 도형 유무
BOOL _isLine = FALSE;
BOOL _isRect = FALSE;
BOOL _isCircle = FALSE;

// 브러시 유무
BOOL _white_brush = FALSE;
BOOL _ltgray_brush = FALSE;
BOOL _gray_brush = FALSE;
BOOL _dkgray_brush = FALSE;
BOOL _black_brush = FALSE;
BOOL _null_brush = FALSE;

// 펜 유무
BOOL _white_pen = FALSE;
BOOL _black_pen = FALSE;
BOOL _null_pen = FALSE;

// 
list<_line> list_line;
list<_rect> list_rect;
list<_circle> list_circle;

list<_draw> list_draw; // 각 도형별 상태 (shape 종류, brush 종류, pen 종류)
list<_position> list_pos; // 각 도형별 위치 (left, top, right, bottom)

int _shape, _brush, _pen;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance); // 창 클래스 등록
BOOL                InitInstance(HINSTANCE, int); // 인스턴스 핸들을 저장하고 주 창을 생성
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM); // 주 창의 메시지를 처리
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM); // 정보 대화 상자의 메시지 처리기

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int		nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO : 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_EOMTSWINDOWPROJECT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return false;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EOMTSWINDOWPROJECT));

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

	return (int)msg.wParam;
}

//
// 함수 : MyRegisterClass()
// 
// 용도 : 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EOMTSWINDOWPROJECT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_EOMTSWINDOWPROJECT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
// 함수 : InitInstance(HINSTANCE, int)
// 
// 용도  : 인스턴스 핸들을 저장하고 주 창을 만듭니다.
// 
// 주석 : 
// 
//		이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//		주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
// 함수 : WndProc(HWND, UINT, WPARAM, LPARAM)
// 
// 용도 : 주 창의 메시지를 처리합니다.
// 
// WM_COMMAND - 애플리케이션 메뉴를 처리합니다.
// WM_PAINT - 주 창을 그립니다.
// WM_DESTROY - 종료 메시지를 게시하고 반환합니다.
//  
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	// 윈도우가 만들어지면서 호출됨
	case WM_CREATE:
		//ZeroMemory(&_drawPos, sizeof(_drawPos));
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

		// 선그리기
		case IDM_DRAW_LINE:
			_shape = enum_shape::line;
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		// 사각형 그리기
		case IDM_DRAW_RECT:
			_shape = enum_shape::rect;
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		// 원 그리기
		case IDM_DRAW_CIRCLE:
			_shape = enum_shape::circle;
			InvalidateRect(hWnd, NULL, TRUE);
			break; 

		// 브러시 선택
		case IDM_BG_WHITE:
			_brush = enum_brush::white_brush;
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case IDM_BG_LTGRAY:
			_brush = enum_brush::ltgray_brush;
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case IDM_BG_GRAY:
			_brush = enum_brush::gray_brush;
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case IDM_BG_DKGRAY:
			_brush = enum_brush::dkgray_brush;
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case IDM_BG_BLACK:
			_brush = enum_brush::black_brush;
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case IDM_BG_NULL:
			_brush = enum_brush::null_brush;
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case IDM_BG_RED:
			_brush = enum_brush::red_brush;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case IDM_BG_GREEN:
			_brush = enum_brush::green_brush;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case IDM_BG_BLUE:
			_brush = enum_brush::blue_brush;
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		// 펜 선택
		case IDM_PEN_WHITE:
			_pen = enum_pen::white_pen;
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case IDM_PEN_BLACK:
			_pen = enum_pen::black_pen;
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case IDM_PEN_NULL:
			_pen = enum_pen::null_pen;
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case IDM_PEN_RED:
			_pen = enum_pen::red_pen;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case IDM_PEN_GREEN:
			_pen = enum_pen::green_pen;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case IDM_PEN_BLUE:
			_pen = enum_pen::blue_pen;
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	// 마우스 왼쪽이 눌렸을 때 발생
	case WM_LBUTTONDOWN:
		_isDraw = TRUE; // 그림 생성시 무조건 그려준다.
		_drawPos.left = LOWORD(lParam);
		_drawPos.top = HIWORD(lParam);
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	// 마우스가 움직이는 동안 위치 갱신
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON) {
			_drawPos.right = LOWORD(lParam);
			_drawPos.bottom = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

	// 마우스 왼쪽이 올라가면 발생
	case WM_LBUTTONUP:
		// 속성 저장
		if (_isDraw)
		{
			_draw draw_tmp(_shape, _brush, _pen, _drawPos.left, _drawPos.top, _drawPos.right, _drawPos.bottom);
			list_draw.push_back(draw_tmp);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		//InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_RBUTTONDOWN:
		// 모든 리스트 제거
		list_draw.clear();
		if (_isDraw)
			_isDraw = FALSE;

		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BrushNPen BnP; // BrushNPen 클래스 객체 생성

		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		swprintf_s(_szStringBuffer, MAX_PATH, L"Mouse Position : (%d %d %d %d)",
			_drawPos.left, _drawPos.top, _drawPos.right, _drawPos.bottom);
		TextOut(hdc, 50, 50, _szStringBuffer, lstrlen(_szStringBuffer)); // 위치좌표 실시간 확인

		// 브러쉬 및 펜 초기화
		HBRUSH oldBrush = BnP.Set_OldBrush(hdc);
		HPEN oldPen = BnP.Set_OldPen(hdc);

		// 드래그 중인 것 그리기
		BnP.Select_Brush(hdc, _brush); // Brush 
		BnP.Select_Pen(hdc, _pen);
		switch (_shape)
		{
			case 0: // line
				MoveToEx(hdc, _drawPos.left, _drawPos.top, nullptr);
				LineTo(hdc, _drawPos.right, _drawPos.bottom);
				break;

			case 1: // rectangle
				Rectangle(hdc, _drawPos.left, _drawPos.top, _drawPos.right, _drawPos.bottom);
				break;

			case 2: // ellipse
				Ellipse(hdc, _drawPos.left, _drawPos.top, _drawPos.right, _drawPos.bottom);
				break;

			default:
				break;
		}
		
		list<_draw>::iterator iter_draw_End = list_draw.end();
		list<_draw>::iterator iter_draw_Pos = list_draw.begin();

		for (iter_draw_Pos; iter_draw_Pos != iter_draw_End; ++iter_draw_Pos)
		{
			BnP.Select_Brush(hdc, iter_draw_Pos->brush); // Brush 
			BnP.Select_Pen(hdc, iter_draw_Pos->pen);

			switch (iter_draw_Pos->shape)
			{
			case 0: // line
				MoveToEx(hdc, iter_draw_Pos->left, iter_draw_Pos->top, nullptr);
				LineTo(hdc, iter_draw_Pos->right, iter_draw_Pos->bottom);
				break;

			case 1: // rectangle
				Rectangle(hdc, iter_draw_Pos->left, iter_draw_Pos->top, iter_draw_Pos->right, iter_draw_Pos->bottom);
				break;

			case 2: // ellipse
				Ellipse(hdc, iter_draw_Pos->left, iter_draw_Pos->top, iter_draw_Pos->right, iter_draw_Pos->bottom);
				break;

			default:
				break;
			}
		}

		// 그림을 다 그리면 원래의 브러쉬로 돌려놓는다.
		SelectObject(hdc, oldBrush);
		SelectObject(hdc, oldPen);

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