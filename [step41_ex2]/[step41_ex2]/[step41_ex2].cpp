// [step41_ex2].cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// ref: https://www.sqlite.org/cintro.html
//

#include "pch.h"
#include <iostream>
#include <iomanip>      // setw
#include <windows.h>    // for DeleteFile
#include <locale.h>     // setlocale
#include "sqlite3.h"
#pragma comment(lib, "sqlite3.lib")
using namespace std;

typedef struct {
    WCHAR name[20];     // 이름
    WCHAR dept[20];     // 학과
    WCHAR address[60];  // 주소
    BOOL male;          // 성별
} STUDENT;

STUDENT students[] = {
    {L"김철수", L"컴퓨터공학", L"서울시", true},
    {L"박인수", L"국문학", L"부산시", true},
    {L"이영희", L"영문학", L"대전시", false}
};

int main()
{
    // ::DeleteFile(L"test.db"); // 파일 삭제

    // #1: DB 열기
    sqlite3* db;
    if (sqlite3_open16(L"test.db", &db) != SQLITE_OK)
    {
        cerr << "Error : " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 1;
    }

    // #2. 테이블 생성
    LPCWSTR sql =
        L"CREATE TABLE IF NOT EXISTS STUDENT\
        (id INTEGER PRIMARY KEY, name TEXT,\
        dept TEXT, address TEXT, gender TEXT);";
    sqlite3_stmt* stmt; // SQL 쿼리 컴파일
    if (sqlite3_prepare16(db, sql, -1, &stmt, NULL) == SQLITE_OK)
        sqlite3_step(stmt); // 실행
    sqlite3_finalize(stmt); // stmt 문장 삭제

    // #3. 레코드 삽입
    int err;
    sql =
        L"INSERT INTO STUDENT VALUES(?, ?, ?, ?, ?);";
    // 쿼리 컴파일
    if (sqlite3_prepare16(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        for (int i = 0; i < 3; i++)
        {
            sqlite3_reset(stmt);
            sqlite3_bind_int(stmt, 1, i + 1);   // 1: 첫 인자
            sqlite3_bind_text16(stmt, 2, students[i].name, -1, SQLITE_STATIC);
            sqlite3_bind_text16(stmt, 3, students[i].dept, -1, SQLITE_STATIC);
            sqlite3_bind_text16(stmt, 4, students[i].address, -1, SQLITE_STATIC);
            sqlite3_bind_text16(stmt, 5, students[i].male ? L"남자" : L"여자", -1, SQLITE_STATIC);

            err = sqlite3_step(stmt);
            if (err != SQLITE_DONE)
                wprintf(L"에러\n");
        }
    }
    sqlite3_finalize(stmt); //stmt 문장 삭제

    // #4. 레코드 조회, 출력
    setlocale(LC_ALL, "");  // 유니코드 한글 출력을 위해
    int id;
    LPWSTR name, dept, address, gender;
    sql =
        L"SELECT * FROM STUDENT;";
    if (sqlite3_prepare16(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        int cols = sqlite3_column_count(stmt);
        for (int col = 0; col < cols; col++)
        {
            if (col == 0)
                wcout << setw(4);
            else
                wcout << setw(10);
            wcout << (LPWSTR)sqlite3_column_name16(stmt, col);
        }
        cout << endl;
        wprintf(L"---------------------------------------------\n");
        
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            id = sqlite3_column_int(stmt, 0);   // 0-칼럼
            name = (LPWSTR)sqlite3_column_text16(stmt, 1);
            dept = (LPWSTR)sqlite3_column_text16(stmt, 2);
            address = (LPWSTR)sqlite3_column_text16(stmt, 3);
            gender = (LPWSTR)sqlite3_column_text16(stmt, 4);
            
            wcout << setw(4) << id;
            wcout << setw(8) << name;
            wcout << setw(8) << dept;
            wcout << setw(6) << address;
            wcout << setw(4) << gender << endl;
        }
    }

    // #5. DB 닫기
    sqlite3_close(db);
    return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
