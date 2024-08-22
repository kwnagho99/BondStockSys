#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "dbutil.h"
#include "bond.h"

void insertBondData(SQLHDBC hDbc) {
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    char sqlQuery[512];
    SQLWCHAR sqlQueryW[512];
    char inputLine[512];

    // 사용자 채권 데이터 입력
    printf("Enter bond data (format: bond_id,bond_name,issue_date,maturity_date,interest_rate,face_value):\n");

    // 사용자 입력 예시: 1,Government Bond,2024-01-01,2029-01-01,3.5,1000000
    while (fgets(inputLine, sizeof(inputLine), stdin)) {
        // 입력된 줄이 빈 줄이면 루프 종료
        if (inputLine[0] == '\n') {
            break;
        }

        int bondId;
        char bondName[50];
        char issueDate[20], maturityDate[20];
        double interestRate, faceValue;

        if (sscanf_s(inputLine, "%d,%49[^,],%19[^,],%19[^,],%lf,%lf", &bondId, bondName, (unsigned)_countof(bondName), issueDate, (unsigned)_countof(issueDate), maturityDate, (unsigned)_countof(maturityDate), &interestRate, &faceValue) == 6) {
            // SQL 쿼리 작성
            sprintf_s(sqlQuery, sizeof(sqlQuery), "INSERT INTO BONDS (bond_id, bond_name, issue_date, maturity_date, interest_rate, face_value) VALUES (%d, '%s', '%s', '%s', %f, %f)", bondId, bondName, issueDate, maturityDate, interestRate, faceValue);

            // 쿼리 문자열을 와이드 문자열로 변환
            MultiByteToWideChar(CP_ACP, 0, sqlQuery, -1, sqlQueryW, 512);

            retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
            checkError(retcode, hDbc, SQL_HANDLE_DBC);

            // 쿼리 실행
            retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
            checkError(retcode, hStmt, SQL_HANDLE_STMT);

            printf("Bond data inserted successfully.\n");

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        }
        else {
            printf("Invalid input format. Please try again.\n");
        }

        printf("Enter next bond data (or press Enter to finish):\n");
    }

    // 삽입 후 데이터 조회 호출
    selectBondData(hDbc);
}

void selectBondData(SQLHDBC hDbc) {
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    SQLWCHAR sqlQueryW[] = L"SELECT bond_id, bond_name, issue_date, maturity_date, interest_rate, face_value FROM BONDS";
    SQLINTEGER bondId = 0;
    SQLWCHAR bondName[50] = { 0 };
    SQLWCHAR issueDate[20] = { 0 }, maturityDate[20] = { 0 };
    SQLDOUBLE interestRate = 0.0, faceValue = 0.0;

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
    checkError(retcode, hStmt, SQL_HANDLE_STMT);

    retcode = SQLBindCol(hStmt, 1, SQL_C_SLONG, &bondId, 0, NULL);
    retcode = SQLBindCol(hStmt, 2, SQL_C_WCHAR, bondName, sizeof(bondName), NULL);
    retcode = SQLBindCol(hStmt, 3, SQL_C_WCHAR, issueDate, sizeof(issueDate), NULL);
    retcode = SQLBindCol(hStmt, 4, SQL_C_WCHAR, maturityDate, sizeof(maturityDate), NULL);
    retcode = SQLBindCol(hStmt, 5, SQL_C_DOUBLE, &interestRate, 0, NULL);
    retcode = SQLBindCol(hStmt, 6, SQL_C_DOUBLE, &faceValue, 0, NULL);

    printf("----------------------------------------\n");
    printf("Bond Data:\n");
    while (SQLFetch(hStmt) != SQL_NO_DATA) {
        wprintf(L"ID: %d, Name: %ls, Issue Date: %ls, Maturity Date: %ls, Interest Rate: %.2f, Face Value: %.2f\n",
            bondId, bondName, issueDate, maturityDate, interestRate, faceValue);
    }
    printf("----------------------------------------\n\n");
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void updateBondData(SQLHDBC hDbc) {
    int bondId;
    char bondName[50];
    char issueDate[20], maturityDate[20];
    double interestRate, faceValue;

    printf("Enter Bond ID to update: ");
    scanf_s("%d", &bondId);
    printf("Enter new Bond Name: ");
    scanf_s("%s", bondName, (unsigned)_countof(bondName));
    printf("Enter new Issue Date (YYYY-MM-DD): ");
    scanf_s("%s", issueDate, (unsigned)_countof(issueDate));
    printf("Enter new Maturity Date (YYYY-MM-DD): ");
    scanf_s("%s", maturityDate, (unsigned)_countof(maturityDate));
    printf("Enter new Interest Rate: ");
    scanf_s("%lf", &interestRate);
    printf("Enter new Face Value: ");
    scanf_s("%lf", &faceValue);

    SQLHSTMT hStmt;
    SQLRETURN retcode;
    char sqlQuery[512];
    SQLWCHAR sqlQueryW[512];

    sprintf_s(sqlQuery, sizeof(sqlQuery), "UPDATE BONDS SET bond_name = '%s', issue_date = '%s', maturity_date = '%s', interest_rate = %f, face_value = %f WHERE bond_id = %d",
        bondName, issueDate, maturityDate, interestRate, faceValue, bondId);

    MultiByteToWideChar(CP_ACP, 0, sqlQuery, -1, sqlQueryW, 512);

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
    checkError(retcode, hStmt, SQL_HANDLE_STMT);

    printf("Bond data updated successfully.\n");

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void deleteBondData(SQLHDBC hDbc) {
    int bondId;
    printf("Enter Bond ID to delete: ");
    scanf_s("%d", &bondId);

    SQLHSTMT hStmt;
    SQLRETURN retcode;
    char sqlQuery[256];
    SQLWCHAR sqlQueryW[256];

    sprintf_s(sqlQuery, sizeof(sqlQuery), "DELETE FROM BONDS WHERE bond_id = %d", bondId);

    MultiByteToWideChar(CP_ACP, 0, sqlQuery, -1, sqlQueryW, 256);

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
    checkError(retcode, hStmt, SQL_HANDLE_STMT);

    printf("Bond data deleted successfully.\n");

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}
