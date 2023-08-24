#include "BrushNPen.h"
#include "struct.h"


HBRUSH BrushNPen::Set_OldBrush(HDC hdc)
{
	return (HBRUSH)SelectObject(hdc, hBrush_black);
}

HPEN BrushNPen::Set_OldPen(HDC hdc)
{
	return (HPEN)SelectObject(hdc, hPen_black);
}

void BrushNPen::Select_Brush(HDC hdc, int brush_num)
{
	switch (brush_num)
	{
	case 0:
		SelectObject(hdc, hBrush_white);
		break;

	case 1:
		SelectObject(hdc, hBrush_ltgray);
		break;

	case 2:
		SelectObject(hdc, hBrush_gray);
		break;

	case 3:
		SelectObject(hdc, hBrush_dkgray);
		break;

	case 4:
		SelectObject(hdc, hBrush_black);
		break;

	case 5:
		SelectObject(hdc, hBrush_null);
		break;

	case 6:
		SelectObject(hdc, hBrush_red);
		break;

	case 7:
		SelectObject(hdc, hBrush_green);
		break;

	case 8:
		SelectObject(hdc, hBrush_blue);
		break;
	default:
		break;
	}
}

void BrushNPen::Select_Pen(HDC hdc, int pen_num)
{
	switch (pen_num)
	{
	case 0:
		SelectObject(hdc, hPen_white);
		break;

	case 1:
		SelectObject(hdc, hPen_black);
		break;

	case 2:
		SelectObject(hdc, hPen_null);
		break;

	case 3:
		SelectObject(hdc, hPen_red);
		break;

	case 4:
		SelectObject(hdc, hPen_green);
		break;

	case 5:
		SelectObject(hdc, hPen_blue);
		break;

	default:
		break;
	}
}
