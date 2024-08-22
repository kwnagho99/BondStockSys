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
    printf("�ȳ��ϼ���, ä�� �� �ֽ� ���� ���α׷��Դϴ�.\n");
    printf("� �޴��� ���ðڽ��ϱ�?\n");
    printf("1. ä�� ������ �Է�\n");
    printf("2. �ֽ� ������ �Է�\n");
    printf("3. ä�� ������ ��ȸ\n");
    printf("4. �ֽ� ������ ��ȸ\n");
    printf("5. ä�� ������ ����\n");
    printf("6. �ֽ� ������ ����\n");
    printf("7. ä�� ������ ����\n");
    printf("8. �ֽ� ������ ����\n");
    printf("9. ���α׷� ����\n");
    printf("����: ");
}

int main() {
    SQLHENV hEnv = NULL;
    SQLHDBC hDbc = NULL;
    SQLRETURN retcode;
    int choice;

    // ODBC ȯ�� �ʱ�ȭ
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    checkError(retcode, hEnv, SQL_HANDLE_ENV);

    retcode = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    checkError(retcode, hEnv, SQL_HANDLE_ENV);

    retcode = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    // �����ͺ��̽� ����
    retcode = SQLConnect(hDbc, (SQLWCHAR*)L"XE", SQL_NTS,
        (SQLWCHAR*)L"c##kwangho", SQL_NTS, (SQLWCHAR*)L"0219", SQL_NTS);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    printf("�����ͺ��̽� ���ῡ �����߽��ϴ�.\n");

    do {
        showMenu();
        scanf_s("%d", &choice);
        getchar();
        switch (choice) {
        case 1:
            printf("ä�� ������ �Է��� �����ϼ̽��ϴ�.\n");
            insertBondData(hDbc);
            break;
        case 2:
            printf("�ֽ� ������ �Է��� �����ϼ̽��ϴ�.\n");
            insertStockData(hDbc);
            break;
        case 3:
            printf("ä�� �����͸� ��ȸ�մϴ�.\n");
            selectBondData(hDbc);
            break;
        case 4:
            printf("�ֽ� �����͸� ��ȸ�մϴ�.\n");
            selectStockData(hDbc);
            break;
        case 5:
            printf("ä�� �����͸� �����մϴ�.\n");
            updateBondData(hDbc);
            break;
        case 6:
            printf("�ֽ� �����͸� �����մϴ�.\n");
            updateStockData(hDbc);
            break;
        case 7:
            printf("ä�� �����͸� �����մϴ�.\n");
            deleteBondData(hDbc);
            break;
        case 8:
            printf("�ֽ� �����͸� �����մϴ�.\n");
            deleteStockData(hDbc);
            break;
        case 9:
            printf("���α׷��� �����մϴ�.\n");
            break;
        default:
            printf("�߸��� �Է��Դϴ�. �ٽ� �õ��� �ּ���.\n");
            break;
        }
    } while (choice != 9);

    // �����ͺ��̽� ���� ����
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    printf("�����ͺ��̽� ������ ����Ǿ����ϴ�.\n");

    return 0;
}
