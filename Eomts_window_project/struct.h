#pragma once

struct _draw {
	_draw(int _shape, int _brush, int _pen, int _left, int _top, int _right, int _bottom)
	{
		// 도형 정보
		shape = _shape;
		brush = _brush;
		pen = _pen;

		// 도형 위치
		left = _left;
		top = _top;
		right = _right;
		bottom = _bottom;
	}

	int shape, brush, pen;
	int left, right, top, bottom;
};

struct _position {
	_position(int _left, int _top, int _right, int _bottom) {
		left = _left;
		top = _top;
		right = _right;
		bottom = _bottom;
	}

	int left, right, top, bottom;
};

struct _line {
	_line(int _left, int _top, int _right, int _bottom) {
		left = _left;
		top = _top;
		right = _right;
		bottom = _bottom;
	}

	int left, right, top, bottom;
};

struct _rect {
	_rect(int _left, int _top, int _right, int _bottom) {
		left = _left;
		top = _top;
		right = _right;
		bottom = _bottom;
	}

	int left, right, top, bottom;
};

struct _circle {
	_circle(int _left, int _top, int _right, int _bottom) {
		left = _left;
		top = _top;
		right = _right;
		bottom = _bottom;
	}

	int left, right, top, bottom;
};

enum enum_shape {
	line	= 0,
	rect,
	circle
};

enum enum_brush {
	white_brush = 0,
	ltgray_brush,
	gray_brush,
	dkgray_brush,
	black_brush,
	null_brush,
	red_brush,
	green_brush,
	blue_brush,
};

enum enum_pen {
	white_pen = 0,
	black_pen,
	null_pen,
	red_pen,
	green_pen,
	blue_pen,
};