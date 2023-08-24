#pragma once

#include "framework.h"
#include "Eomts_window_project.h"
class BrushNPen
{
public:
	HBRUSH Set_OldBrush(HDC hdc);
	HPEN Set_OldPen(HDC hdc);

	void Select_Brush(HDC hdc, int brush_num);
	void Select_Pen(HDC hdc, int pen_num);

private:
	// 브러시 색깔 바꾸기
	HGDIOBJ hBrush_white = GetStockObject(WHITE_BRUSH);		// 하얀색
	HGDIOBJ hBrush_ltgray = GetStockObject(LTGRAY_BRUSH);	// 연회색
	HGDIOBJ hBrush_gray = GetStockObject(GRAY_BRUSH);		// 회색
	HGDIOBJ hBrush_dkgray = GetStockObject(DKGRAY_BRUSH);	// 진회색
	HGDIOBJ hBrush_black = GetStockObject(BLACK_BRUSH);	// 검은색
	HGDIOBJ hBrush_null = GetStockObject(NULL_BRUSH);		// 투명

	HBRUSH hBrush_red = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH hBrush_green = CreateSolidBrush(RGB(0, 255, 0));
	HBRUSH hBrush_blue = CreateSolidBrush(RGB(0, 0, 255));

	// 펜 색깔 바꾸기
	HGDIOBJ hPen_white = GetStockObject(WHITE_PEN);			// 하얀펜
	HGDIOBJ hPen_black = GetStockObject(BLACK_PEN);			// 검정펜
	HGDIOBJ hPen_null = GetStockObject(NULL_PEN);			// 검정펜

	HPEN hPen_red = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN hPen_green = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HPEN hPen_blue = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
};

