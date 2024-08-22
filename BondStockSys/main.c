#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "bond.h"
#include "stock.h"
#include "dbutil.h"
#include <stddef.h>
#include <sql.h>
#include <sqlext.h>

void showMenu() {
    printf("안녕하세요, 채권 및 주식 관리 프로그램입니다.\n");
    printf("어떤 메뉴를 고르시겠습니까?\n");
    printf("1. 채권 데이터 입력\n");
    printf("2. 주식 데이터 입력\n");
    printf("3. 채권 데이터 조회\n");
    printf("4. 주식 데이터 조회\n");
    printf("5. 채권 데이터 수정\n");
    printf("6. 주식 데이터 수정\n");
    printf("7. 채권 데이터 삭제\n");
    printf("8. 주식 데이터 삭제\n");
    printf("9. 프로그램 종료\n");
    printf("선택: ");
}

int main() {
    SQLHENV hEnv = NULL;
    SQLHDBC hDbc = NULL;
    SQLRETURN retcode;
    int choice;

    // ODBC 환경 초기화
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    checkError(retcode, hEnv, SQL_HANDLE_ENV);

    retcode = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    checkError(retcode, hEnv, SQL_HANDLE_ENV);

    retcode = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    // 데이터베이스 연결
    retcode = SQLConnect(hDbc, (SQLWCHAR*)L"XE", SQL_NTS,
        (SQLWCHAR*)L"c##kwangho", SQL_NTS, (SQLWCHAR*)L"0219", SQL_NTS);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    printf("데이터베이스 연결에 성공했습니다.\n");

    do {
        showMenu();
        scanf_s("%d", &choice);
        getchar();
        switch (choice) {
        case 1:
            printf("채권 데이터 입력을 선택하셨습니다.\n");
            insertBondData(hDbc);
            break;
        case 2:
            printf("주식 데이터 입력을 선택하셨습니다.\n");
            insertStockData(hDbc);
            break;
        case 3:
            printf("채권 데이터를 조회합니다.\n");
            selectBondData(hDbc);
            break;
        case 4:
            printf("주식 데이터를 조회합니다.\n");
            selectStockData(hDbc);
            break;
        case 5:
            printf("채권 데이터를 수정합니다.\n");
            updateBondData(hDbc);
            break;
        case 6:
            printf("주식 데이터를 수정합니다.\n");
            updateStockData(hDbc);
            break;
        case 7:
            printf("채권 데이터를 삭제합니다.\n");
            deleteBondData(hDbc);
            break;
        case 8:
            printf("주식 데이터를 삭제합니다.\n");
            deleteStockData(hDbc);
            break;
        case 9:
            printf("프로그램을 종료합니다.\n");
            break;
        default:
            printf("잘못된 입력입니다. 다시 시도해 주세요.\n");
            break;
        }
    } while (choice != 9);

    // 데이터베이스 연결 종료
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    printf("데이터베이스 연결이 종료되었습니다.\n");

    return 0;
}
