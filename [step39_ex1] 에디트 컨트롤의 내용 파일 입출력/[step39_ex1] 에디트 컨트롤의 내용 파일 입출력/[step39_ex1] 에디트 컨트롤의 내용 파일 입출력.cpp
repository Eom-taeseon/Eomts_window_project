// [step39_ex1] 에디트 컨트롤의 내용 파일 입출력.cpp : 애플리케이션에 대한 진입점을 정의합니다.
// ref1 : [step23_ex1] GetOpenFileName, GetSaveFileName 대화상자
// ref2 : [step33_ex6] 멀티 라인 에디트 컨트롤
//

#include "framework.h"
#include "[step39_ex1] 에디트 컨트롤의 내용 파일 입출력.h"
#include "stdafx.h"
#include "commdlg.h"

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

LPWSTR LoadFromFile(LPTSTR fileName)    // Unicode load
{
    HANDLE hFile = CreateFile(fileName, GENERIC_READ, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return NULL;

    DWORD nFileSize = GetFileSize(hFile, NULL);

    char* pszBuf = (char*)LocalAlloc(LPTR, nFileSize);
    DWORD nReadSize;
    ReadFile(hFile, pszBuf, nFileSize, &nReadSize, NULL);
    CloseHandle(hFile);

    // BOM(Byte Order Marker) check
    BYTE b0 = (BYTE)pszBuf[0];
    BYTE b1 = (BYTE)pszBuf[1];
    BYTE b2 = (BYTE)pszBuf[2];
    LPWSTR strUnicode;
    if (b0 == 0xFF && b1 == 0xFE)    // Unicode, Little Endian
    {
        // 마지막 NULL 문자열 때문에 +2
        strUnicode = (LPWSTR)LocalAlloc(LPTR, nFileSize + 2);
        // ZeroMemory(strUnicode, nFileSize + 2);

        CopyMemory(strUnicode, pszBuf, nFileSize);
    }

    else if (b0 == 0xEF && b1 == 0xBB && b2 == 0xBF) // UTF-8
    {
        int nLen = MultiByteToWideChar(CP_UTF8, 0, pszBuf, nFileSize, NULL, 0);
        strUnicode = (LPWSTR)LocalAlloc(LPTR, (nLen + 1) * sizeof(WCHAR)); // 마지막 NULL 문자열 때문에 +1
        // ZeroMemory(strUnicode, (nLen + 1) * sizeof(WCHAR));
        MultiByteToWideChar(CP_UTF8, 0, pszBuf, strlen(pszBuf), strUnicode, nLen);
    }

    else    // ANSI -> UNICODE
    {
        int nLen = MultiByteToWideChar(CP_ACP, 0, pszBuf, nFileSize, NULL, 0);
        strUnicode = (LPWSTR)LocalAlloc(LPTR, (nLen + 1) * sizeof(WCHAR));  // 마지막 NULL 문자열 때문에 +1
        // ZeroMemory(strUnicode, (nLen+1) * sizeof(WCHAR));
        MultiByteToWideChar(CP_ACP, 0, pszBuf, strlen(pszBuf), strUnicode, nLen);
    }
    LocalFree(pszBuf);
    return strUnicode;
}

void WriteToFile(LPTSTR fileName, LPWSTR strUnicode)
{
    HANDLE hFile = CreateFile(fileName, GENERIC_WRITE, 0, NULL, 
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    DWORD bytesWritten;
    // BOM(Byte Order Memory) check
    WORD w0 = strUnicode[0];
    if (w0 != 0xFEFF)    // 에디트 컨트롤, 유니코드로 저장
    {
        w0 = 0xFEFF;    // Unicode BOM
        WriteFile(hFile, &w0, 2, &bytesWritten, NULL);
    }
    DWORD nSize;
    nSize = lstrlen(strUnicode) * sizeof(WCHAR);    // wcslen

    WriteFile(hFile, strUnicode, nSize, &bytesWritten, NULL);
    CloseHandle(hFile);
}

// 정보 대화상자의 메시지 처리기
#define MAXSIZE 512
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hEdit;
    static int cxClient, cyClient;

    static TCHAR lpstrFile[MAXSIZE] = _T("");
    static TCHAR lpstrFileTitle[MAXSIZE] = _T("");
    static OPENFILENAME ofn;

    switch (message)
    {
    case WM_INITDIALOG:
    {
        hEdit = GetDlgItem(hDlg, IDC_EDIT1);
        SetWindowText(hDlg, _T("제목 없음"));
        SetFocus(hEdit);

        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = hDlg;
        ofn.hInstance = hInst;
        ofn.lpstrFilter = _T("텍스트 파일(*.txt)\0*.txt\0")
            _T("모든 파일(*.*)\0 *.*\0\0");
        ofn.lpstrFile = lpstrFile;
        ofn.lpstrDefExt = _T("txt");
        ofn.nMaxFile = MAXSIZE;
        ofn.lpstrFileTitle = lpstrFileTitle;
        ofn.nMaxFileTitle = MAXSIZE;
        return (INT_PTR)TRUE;
    }

    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        SetWindowPos(hEdit, NULL, 0, 0, cxClient, cyClient, SWP_NOREDRAW);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDM_FILE_NEW:
            ZeroMemory(ofn.lpstrFile, sizeof(lpstrFile));
            ZeroMemory(ofn.lpstrFileTitle, sizeof(lpstrFileTitle));

            SetWindowText(hDlg, _T("제목 없음"));
            SetDlgItemText(hDlg, IDC_EDIT1, _T(""));
            EnableMenuItem(GetMenu(hDlg), IDM_FILE_SAVE, MF_DISABLED);
            break;

        case IDM_FILE_OPEN:
            ofn.Flags = OFN_FILEMUSTEXIST;
            if (GetOpenFileName(&ofn))   // FileOpen 대화상자
            {
                SetWindowText(hDlg, ofn.lpstrFileTitle);
                LPWSTR strUnicode = LoadFromFile(ofn.lpstrFile);
                SetDlgItemText(hDlg, IDC_EDIT1, strUnicode);
                LocalFree(strUnicode);
                EnableMenuItem(GetMenu(hDlg), IDM_FILE_SAVE, MF_ENABLED);
            }
            break;

        case IDM_FILE_SAVE:
        {
            if (lstrlen(ofn.lpstrFile) == 0)
            {
                SendMessage(hDlg, WM_COMMAND, IDM_FILE_SAVE_AS, 0);
                break;
            }
            DWORD nTextLimit = (DWORD)SendMessage(hEdit, EM_GETLIMITTEXT, 0, 0);
            LPWSTR strUnicode = (LPWSTR)LocalAlloc(LPTR, nTextLimit * sizeof(WCHAR));
            GetDlgItemText(hDlg, IDC_EDIT1, strUnicode, nTextLimit);
            WriteToFile(ofn.lpstrFile, strUnicode);
            LocalFree(strUnicode);
            break;
        }

        case IDM_FILE_SAVE_AS:  // UNICODE Save
            ofn.Flags = OFN_OVERWRITEPROMPT;
            if (GetSaveFileName(&ofn))
            {
                DWORD nTextLimit = (DWORD)SendMessage(hEdit, EM_GETLIMITTEXT, 0, 0);
                LPTSTR strUnicode = (LPTSTR)LocalAlloc(LPTR, nTextLimit * sizeof(TCHAR));
                GetDlgItemText(hDlg, IDC_EDIT1, strUnicode, nTextLimit);

                WriteToFile(ofn.lpstrFile, strUnicode);
                LocalFree(strUnicode);
                SetWindowText(hDlg, ofn.lpstrFileTitle);
                EnableMenuItem(GetMenu(hDlg), IDM_FILE_SAVE, MF_ENABLED);
            }
            break;

        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            break;
        }
        return (INT_PTR)TRUE;
    case WM_DESTROY:
        return (INT_PTR)TRUE;
    }
    return(INT_PTR)FALSE;
}