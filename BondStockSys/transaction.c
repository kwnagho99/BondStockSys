#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <string.h>
#include "transaction.h"

void executeTransaction(SQLHDBC hDbc) {
    SQLRETURN retcode;

    // �ڵ� Ŀ�� ��带 ���ϴ�.
    retcode = SQLSetConnectAttr(hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, 0);
    if (retcode != SQL_SUCCESS) {
        printf("Failed to set auto-commit off.\n");
        return;
    }

    // ���� �۾� ���� (����: updateStockData �Լ� ȣ��)
    updateStockData(hDbc, 1, "Updated Name", "UTK", "NYSE", 200.00);

    // ���� �߻� ���θ� üũ�մϴ�.
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        // Ʈ������� Ŀ���մϴ�.
        retcode = SQLEndTran(SQL_HANDLE_DBC, hDbc, SQL_COMMIT);
        if (retcode != SQL_SUCCESS) {
            printf("Failed to commit transaction.\n");
        }
        else {
            printf("Transaction committed successfully.\n");
        }
    }
    else {
        // Ʈ������� �ѹ��մϴ�.
        retcode = SQLEndTran(SQL_HANDLE_DBC, hDbc, SQL_ROLLBACK);
        if (retcode != SQL_SUCCESS) {
            printf("Failed to rollback transaction.\n");
        }
        else {
            printf("Transaction rolled back due to an error.\n");
        }
    }

    // �ڵ� Ŀ�� ��带 �ٽ� �մϴ�.
    retcode = SQLSetConnectAttr(hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, 0);
    if (retcode != SQL_SUCCESS) {
        printf("Failed to set auto-commit on.\n");
    }
}
