
// [step35_ex3] 리스트-뷰 컨트롤 3.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "[step35_ex3] 리스트-뷰 컨트롤 3.h"

#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib") // for InitCommonControls

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
INT_PTR CALLBACK    DlgProc(HWND, UINT, WPARAM, LPARAM);

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
    TCHAR addr[20]; // 주소
    BOOL male;     // 성별
}STUDENT;

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

        RECT rc;
        GetClientRect(hList, &rc);
        int nWidth = rc.right - rc.left;

        // 칼럼 헤더 생성
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

        // 행 전체 선택
        ListView_SetExtendedListViewStyle(hList, LVS_EX_FULLROWSELECT);
        return true;
    }

    case WM_NOTIFY:
    {
        LPNMHDR pHDR = (LPNMHDR)lParam;
        int i;
        LVITEM row;
        ZeroMemory(&row, sizeof(LVITEM));

        switch (pHDR->idFrom)
        {
        case IDC_LIST1:
            if (pHDR->code == NM_CLICK)
            {
                i = ListView_GetNextItem(hList, -1, LVNI_SELECTED);

                if (i < 0)
                    break;

                TCHAR pszText[128];
                ListView_GetItemText(hList, i, 0, pszText, 128);
                SetDlgItemText(hDlg, IDC_EDIT1, pszText);
                ListView_GetItemText(hList, i, 1, pszText, 128);
                SetDlgItemText(hDlg, IDC_EDIT2, pszText);

                ListView_GetItemText(hList, i, 2, pszText, 128);
                SetDlgItemText(hDlg, IDC_EDIT3, pszText);

                row.iItem = i;
                row.mask = LVIF_IMAGE;
                ListView_GetItem(hList, &row);
                CheckDlgButton(hDlg, IDC_CHECK1, row.iImage ? BST_CHECKED : BST_UNCHECKED);
            }
            break;
        }
        return true;
    }

    case WM_COMMAND:
    {
        LVITEM row;
        int i;
        TCHAR szText1[128], szText2[128], szText3[128];
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON1:
            // 에디트 컨트롤 문자열 가져오기
            GetDlgItemText(hDlg, IDC_EDIT1, szText1, 128);
            GetDlgItemText(hDlg, IDC_EDIT2, szText2, 128);
            GetDlgItemText(hDlg, IDC_EDIT3, szText3, 128);

            if (lstrlen(szText1) == 0)
                break;
            i = ListView_GetItemCount(hList);   // 항목 개수

            // 행 추가
            memset(&row, 0, sizeof(LVITEM));
            row.iItem = i;
            row.iImage = IsDlgButtonChecked(hDlg, IDC_CHECK1);
            row.mask = LVIF_IMAGE;
            ListView_InsertItem(hList, &row);   // 행 추가
            ListView_SetItemText(hList, i, 0, szText1);
            ListView_SetItemText(hList, i, 1, szText2);
            ListView_SetItemText(hList, i, 2, szText3);
            break;

        case IDC_BUTTON2:
            // 행 삭제
            i = ListView_GetNextItem(hList, -1, LVNI_SELECTED);
            ListView_DeleteItem(hList, i);  // LVM_DELETEITEM
            break;
        }
        return true;
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