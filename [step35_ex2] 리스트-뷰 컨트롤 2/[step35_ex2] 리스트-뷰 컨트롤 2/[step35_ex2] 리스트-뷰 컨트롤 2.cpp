// [step35_ex2] 리스트-뷰 컨트롤 2.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step35_ex2] 리스트-뷰 컨트롤 2.h"

#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib") // for InitCommonControls()

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
    TCHAR addr[60]; // 주소
    BOOL male;      // 성별
} STUDENT;

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hList;
    static HIMAGELIST hIL;

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
        hIL = ImageList_LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1), 48, 1, CLR_NONE);
        ListView_SetImageList(hList, hIL, LVSIL_SMALL);
        ListView_SetImageList(hList, hIL, LVSIL_NORMAL);    // LVS_ICON

        RECT rc;
        GetClientRect(hList, &rc);
        int nWidth = rc.right - rc.left;

        // 칼럼 헤더 설정
        LVCOLUMN col;
        ZeroMemory(&col, sizeof(LVCOLUMN));
        col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
        col.fmt = LVCFMT_CENTER;

        col.pszText = (LPTSTR)_T("이름");
        col.cx = 120;
        ListView_InsertColumn(hList, 0, &col);

        col.pszText = (LPTSTR)_T("학과");
        col.cx = 130;
        ListView_InsertColumn(hList, 1, &col);

        col.pszText = (LPTSTR)_T("주소");
        col.cx = nWidth - 250;
        ListView_InsertColumn(hList, 2, &col);

        // 항목 추가
        LVITEM row;
        ZeroMemory(&row, sizeof(LVITEM));
        row.mask = LVIF_TEXT | LVIF_IMAGE;
        for (int i = 0; i < 3; i++)
        {
            row.iItem = i;
            row.iSubItem = 0;
            row.pszText = students[i].name;
            row.iImage = students[i].male ? 1 : 0;
            ListView_InsertItem(hList, &row);

            ListView_SetItemText(hList, i, 1, students[i].dept);
            ListView_SetItemText(hList, i, 2, students[i].addr);
        }

        // 행 전체 선택 설정
        ListView_SetExtendedListViewStyle(hList, LVS_EX_FULLROWSELECT);

        return true;
    }

    case WM_COMMAND:
    {
        DWORD dwStyle = GetWindowLong(hList, GWL_STYLE);
        switch (LOWORD(wParam))
        {
        case IDM_VIEWSTYLE_ICON:
            dwStyle &= ~LVS_TYPEMASK;
            dwStyle |= LVS_ICON;    // 원하는 스타일 지정
            SetWindowLong(hList, GWL_STYLE, dwStyle);
            break;

        case IDM_VIEWSTYLE_SMALLICON:
            dwStyle &= ~LVS_TYPEMASK;
            dwStyle |= LVS_SMALLICON;
            SetWindowLong(hList, GWL_STYLE, dwStyle);
            break;

        case IDM_VIEWSTYLE_LIST:
            dwStyle &= ~LVS_TYPEMASK;
            dwStyle |= LVS_LIST;
            SetWindowLong(hList, GWL_STYLE, dwStyle);
            break;

        case IDM_VIEWSTYLE_REPORT:
            dwStyle &= ~LVS_TYPEMASK;
            dwStyle |= LVS_REPORT;
            SetWindowLong(hList, GWL_STYLE, dwStyle);
            break;
        }
        return (INT_PTR)TRUE;
    }

    case WM_CLOSE:
        ImageList_Destroy(hIL);
        DestroyWindow(hDlg);
        return true;

    case WM_DESTROY:
        return true;
    }
    return false;
}