// [step41_ex1].cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// ref1: https://www.sqlite.org/cintro.html
// ref2: https://dcravey.wordpress.com/2011/03/21/using-sqlite-in-a-visual-c-application/
//

#include "pch.h"
#include <iostream>
#include <iomanip> // setw
#include <Windows.h> // for DeleteFile
#include "sqlite3.h"
#pragma comment(lib, "sqlite3.lib")
using namespace std;

int main()
{
    ::DeleteFile(L"test.db");   // DB 파일 삭제

    // #1: DB 파일 개방
    sqlite3* db;
    // if (sqlite3_open(":memory:", &db) != SQLITE_OK)  // 인 메모리 DB
    if (sqlite3_open("test.db", &db) != SQLITE_OK)     // DB 개방 + 파일이 없으면 새로 생성
    {
        cerr << "Error: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 1;
    }

    // #2: 테이블 생성
    char* error;
    const char* sql =   // SQL 문장 초기화
        "CREATE TABLE IF NOT EXISTS STUDENT\
            (id INTEGER PRIMARY KEY, name TEXT,\
            dept TEXT, address TEXT, gender INTEGER);";
    if (sqlite3_exec(db, sql, NULL, NULL, &error) != SQLITE_OK) // 
    {
        cerr << "ERROR: " << sqlite3_errmsg(db) << endl;
        sqlite3_free(error);
    }

    // #3: 레코드 삽입
    sql = // SQL 문장 저장
        "INSERT INTO STUDENT\
            VALUES(1, '김철수', '컴퓨터공학', '서울시', TRUE);";
    if (sqlite3_exec(db, sql, NULL, NULL, &error) != SQLITE_OK) // SQL 문장 실행
    {
        cerr << "ERROR: " << sqlite3_errmsg(db) << endl;
        sqlite3_free(error);
    }
    sqlite3_exec(db, "INSERT INTO STUDENT\
                        VALUES(2, '박인수', '국문학', '부산시', TRUE);",
        NULL, NULL, &error);
    sqlite3_exec(db, "INSERT INTO STUDENT\
                        VALUES(3, '이영희', '영문학', '대전시', FALSE);",
        NULL, NULL, &error);

    // #4: 레코드 조회, 출력
    char **results = NULL;
    int rows, columns;
    sql = "SELECT * FROM STUDENT;"; // SQL 문장 저장
    if (sqlite3_get_table(db, sql, &results, &rows, &columns, &error) != SQLITE_OK) // sqlite3_get_table 함수를 사용해 검색 결과 테이블을 results에 저장
    {
        cerr << "ERROR: " << sqlite3_errmsg(db) << endl;
        sqlite3_free(error);
    }

    else
    {
        // cout << "rows: " << rows << endl;
        wcout << "rows: " << rows << endl;
        wcout << "columns: " << columns << endl;
        for (int r = 0; r <= rows; r++)  // 테이블 출력
        {
            for (int c = 0; c < columns; c++)
            {
                int k = (r * columns) + c;  // 셀 위치
                wcout << setw(11) << results[k];    // 셀 출력
            }
            wcout << endl;  // 줄 변경

            if (r == 0)    // 헤더 구분선 출력
            {
                for (int c = 0; c < columns; c++)
                    wcout << setw(12) << "----------";
                wcout << endl;  // 줄 변경
            }
        }
    }

    // #5. DB 파일 닫기
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
