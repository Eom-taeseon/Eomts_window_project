// [step35_ex1] 리스트-뷰 컨트롤 1; Report 뷰.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step35_ex1] 리스트-뷰 컨트롤 1; Report 뷰.h"
#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")    // for InitCommonControls()

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    hInst = hInstance;
    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
    return 0;
}

typedef struct {
    TCHAR name[20]; // 이름
    TCHAR dept[20]; // 학과
    TCHAR address[60];  // 주소
    BOOL male;  // 성별
}STUDENT;

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hList;
    STUDENT students[] = {
        { _T("김철수"), _T("컴퓨터공학"), _T("서울시"), TRUE },
        { _T("박인수"), _T("국문학"), _T("부산시"), TRUE },
        { _T("이영희"), _T("영문학"), _T("대전시"), FALSE } 
    };

    switch (message)
    {
    case WM_INITDIALOG:
    {
        InitCommonControls();   // 공용 컨트롤 DLL 로드 확인
        hList = GetDlgItem(hDlg, IDC_LIST1);

        RECT rc;
        GetClientRect(hList, &rc);
        int nWidth = rc.right - rc.left;

        // 칼럼 헤더 생성
        LVCOLUMN col;
        ZeroMemory(&col, sizeof(LVCOLUMN));
        col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
        col.fmt = LVCFMT_CENTER;

        col.pszText = (LPTSTR)_T("성별");
        col.cx = 50;
        ListView_InsertColumn(hList, 0, &col);

        col.pszText = (LPTSTR)_T("이름");
        col.cx = 120;
        ListView_InsertColumn(hList, 1, &col);

        col.pszText = (LPTSTR)_T("학과");
        col.cx = 130;
        ListView_InsertColumn(hList, 2, &col);

        col.pszText = (LPTSTR)_T("주소");
        col.cx = nWidth - 300;
        ListView_InsertColumn(hList, 3, &col);

        // 항목 추가
        LVITEM row;
        ZeroMemory(&row, sizeof(LVITEM));
        row.mask = LVIF_TEXT;
        for (int i = 0; i < 3; i++)
        {
            row.iItem = i;
            row.iSubItem = 0;
            row.pszText = (LPTSTR)(students[i].male? _T("남자") : _T("여자"));
            ListView_InsertItem(hList, &row);

            ListView_SetItemText(hList, i, 1, students[i].name);
            ListView_SetItemText(hList, i, 2, students[i].dept);
            ListView_SetItemText(hList, i, 3, students[i].address);
        }

        // 행 전체 설택 설정
        ListView_SetExtendedListViewStyle(hList, LVS_EX_FULLROWSELECT);
        return (INT_PTR)TRUE;
    }

    case WM_CLOSE:
        DestroyWindow(hDlg);
        return true;

    case WM_DESTROY:
        return true;
    }
    return false;
}