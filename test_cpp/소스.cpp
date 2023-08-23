// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <list>

using namespace std;

struct _line {
	_line(int _left, int _top, int _right, int _bottom) {
		left = _left;
		top = _top;
		right = _right;
		bottom = _bottom;
	}

	int left, right, top, bottom;
};

int main()
{
	cout << "hello world" << endl;

	list<_line> test_line;

	_line line_1(1, 1, 1, 1);
	test_line.push_back(line_1);

	_line line_2(2, 2, 2, 2);
	test_line.push_back(line_2);

	_line line_3(3, 3, 3, 3);
	test_line.push_back(line_3);

	list<_line>::iterator iterEnd = test_line.end();
	list<_line>::iterator iterPos = test_line.begin();
	for (iterPos; iterPos != iterEnd; ++iterPos) {
		cout << iterPos->left << " " << iterPos->top << " " << iterPos->right << " " << iterPos->bottom << endl;
	}

	system("pause");
	return 0;
}