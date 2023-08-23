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
	// �귯�� ���� �ٲٱ�
	HGDIOBJ hBrush_white = GetStockObject(WHITE_BRUSH);		// �Ͼ��
	HGDIOBJ hBrush_ltgray = GetStockObject(LTGRAY_BRUSH);	// ��ȸ��
	HGDIOBJ hBrush_gray = GetStockObject(GRAY_BRUSH);		// ȸ��
	HGDIOBJ hBrush_dkgray = GetStockObject(DKGRAY_BRUSH);	// ��ȸ��
	HGDIOBJ hBrush_black = GetStockObject(BLACK_BRUSH);	// ������
	HGDIOBJ hBrush_null = GetStockObject(NULL_BRUSH);		// ����

	// �� ���� �ٲٱ�
	HGDIOBJ hPen_white = GetStockObject(WHITE_PEN);			// �Ͼ���
	HGDIOBJ hPen_black = GetStockObject(BLACK_PEN);			// ������
	HGDIOBJ hPen_null = GetStockObject(NULL_PEN);			// ������

};

