#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "dbutil.h"

void checkError(SQLRETURN retcode, SQLHANDLE handle, SQLSMALLINT type) {
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        SQLWCHAR sqlState[6], message[256];
        SQLINTEGER nativeError;
        SQLSMALLINT textLength;
        SQLGetDiagRec(type, handle, 1, sqlState, &nativeError, message, sizeof(message), &textLength);
        wprintf(L"Error: %ls\n", message);

        // 동일한 ID로 삽입 시 발생하는 오류 처리
        if (wcscmp(sqlState, L"23000") == 0) { // SQL 상태 코드 23000은 무결성 제약 조건 위반을 의미
            wprintf(L"Duplicate ID error. Cannot insert duplicate IDs.\n");
        }

        // 트랜잭션 롤백
        if (type == SQL_HANDLE_DBC) {
            SQLEndTran(SQL_HANDLE_DBC, handle, SQL_ROLLBACK);
            wprintf(L"Transaction rolled back.\n");
        }

        exit(-1);
    }
}
