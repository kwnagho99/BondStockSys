#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <string.h>
#include "transaction.h"

void executeTransaction(SQLHDBC hDbc) {
    SQLRETURN retcode;

    // 자동 커밋 모드를 끕니다.
    retcode = SQLSetConnectAttr(hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, 0);
    if (retcode != SQL_SUCCESS) {
        printf("Failed to set auto-commit off.\n");
        return;
    }

    // 수정 작업 수행 (예시: updateStockData 함수 호출)
    updateStockData(hDbc, 1, "Updated Name", "UTK", "NYSE", 200.00);

    // 에러 발생 여부를 체크합니다.
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        // 트랜잭션을 커밋합니다.
        retcode = SQLEndTran(SQL_HANDLE_DBC, hDbc, SQL_COMMIT);
        if (retcode != SQL_SUCCESS) {
            printf("Failed to commit transaction.\n");
        }
        else {
            printf("Transaction committed successfully.\n");
        }
    }
    else {
        // 트랜잭션을 롤백합니다.
        retcode = SQLEndTran(SQL_HANDLE_DBC, hDbc, SQL_ROLLBACK);
        if (retcode != SQL_SUCCESS) {
            printf("Failed to rollback transaction.\n");
        }
        else {
            printf("Transaction rolled back due to an error.\n");
        }
    }

    // 자동 커밋 모드를 다시 켭니다.
    retcode = SQLSetConnectAttr(hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, 0);
    if (retcode != SQL_SUCCESS) {
        printf("Failed to set auto-commit on.\n");
    }
}
