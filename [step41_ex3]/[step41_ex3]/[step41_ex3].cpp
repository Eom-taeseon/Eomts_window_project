
// [step41_ex3].cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "stdafx.h""
#include "framework.h"
#include "[step41_ex3].h"

#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")    // InitCommonControls()

#include "sqlite3.h"
#pragma comment(lib, "sqlite3.lib")

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

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HWND hList;
    static sqlite3* db;

    switch (msg)
    {
    case WM_INITDIALOG:
    {
        InitCommonControls();   // 공용 컨트롤 DLL 로드 확인
        hList = GetDlgItem(hDlg, IDC_LIST1);
        RECT rc;
        GetClientRect(hList, &rc);
        int nWidth = rc.right - rc.left;
        int nW[5] = { 50, 100, 150, 100, 100 };
        nW[3] = nWidth - 400;

        sqlite3_stmt* stmt;     // SQL 쿼리 컴파일
        if (sqlite3_open16(L"test.db", &db) != SQLITE_OK)    // DB 개방
        {
            sqlite3_close(db);
            (INT_PTR)FALSE;
        }
        LPCWSTR sql = L"SELECT * FROM STUDENT;";
        if (sqlite3_prepare16(db, sql, -1, &stmt, NULL) == SQLITE_OK)
        {
            LVCOLUMN col;
            ZeroMemory(&col, sizeof(LVCOLUMN));
            col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
            int cols = sqlite3_column_count(stmt);
            for (int c = 0; c < cols; c++)  // 칼럼 헤더 생성
            {
                col.fmt = LVCFMT_CENTER;
                col.pszText =
                    (LPWSTR)sqlite3_column_name16(stmt, c);
                col.cx = nW[c];
                ListView_InsertColumn(hList, c, &col);
            }

            // 항목 추가
            LVITEM row;
            ZeroMemory(&row, sizeof(LVITEM));
            row.mask = LVIF_TEXT;
            int id;
            WCHAR sID[10];
            LPWSTR header, name, dept, address, gender;

            int k = 0;
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                id = sqlite3_column_int(stmt, 0);   // 0-칼럼
                name = (LPWSTR)sqlite3_column_text16(stmt, 1);
                dept = (LPWSTR)sqlite3_column_text16(stmt, 2);
                address = (LPWSTR)sqlite3_column_text16(stmt, 3);
                gender = (LPWSTR)sqlite3_column_text16(stmt, 4);
                wsprintf(sID, _T("%d"), id);

                row.iItem = k;  // 행 번호
                row.iSubItem = 0;
                row.pszText = sID;
                ListView_InsertItem(hList, &row);

                ListView_SetItemText(hList, k, 1, name);
                ListView_SetItemText(hList, k, 2, dept);
                ListView_SetItemText(hList, k, 3, address);
                ListView_SetItemText(hList, k, 4, gender);
                k++;
            }
        }
        // 행 전체 선택 설정
        ListView_SetExtendedListViewStyle(hList, LVS_EX_FULLROWSELECT);
        return (INT_PTR)TRUE;
    }

    case WM_CLOSE:
        sqlite3_close(db);
        DestroyWindow(hDlg);
        return true;

    case WM_DESTROY:
        return true;
    }
    return false;
}