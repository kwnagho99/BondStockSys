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

        // ������ ID�� ���� �� �߻��ϴ� ���� ó��
        if (wcscmp(sqlState, L"23000") == 0) { // SQL ���� �ڵ� 23000�� ���Ἲ ���� ���� ������ �ǹ�
            wprintf(L"Duplicate ID error. Cannot insert duplicate IDs.\n");
        }

        // Ʈ����� �ѹ�
        if (type == SQL_HANDLE_DBC) {
            SQLEndTran(SQL_HANDLE_DBC, handle, SQL_ROLLBACK);
            wprintf(L"Transaction rolled back.\n");
        }

        exit(-1);
    }
}
