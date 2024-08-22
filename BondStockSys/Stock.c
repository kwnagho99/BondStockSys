#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "stock.h"

void insertStockData(SQLHDBC hDbc) {
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    char sqlQuery[512];
    SQLWCHAR sqlQueryW[512];
    char inputLine[512];

    // 사용자 주식 데이터 입력
    printf("Enter stock data (format: stock_id,stock_name,ticker_symbol,market,last_price):\n");

    while (fgets(inputLine, sizeof(inputLine), stdin)) {
        if (inputLine[0] == '\n') {
            break;
        }

        int stockId;
        char stockName[50];
        char tickerSymbol[10];
        char market[50];
        double lastPrice;

        if (sscanf_s(inputLine, "%d,%49[^,],%9[^,],%49[^,],%lf", &stockId, stockName, (unsigned)_countof(stockName), tickerSymbol, (unsigned)_countof(tickerSymbol), market, (unsigned)_countof(market), &lastPrice) == 5) {
            sprintf_s(sqlQuery, sizeof(sqlQuery), "INSERT INTO STOCKS (stock_id, stock_name, ticker_symbol, market, last_price) VALUES (%d, '%s', '%s', '%s', %f)", stockId, stockName, tickerSymbol, market, lastPrice);
            MultiByteToWideChar(CP_ACP, 0, sqlQuery, -1, sqlQueryW, 512);

            retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
            checkError(retcode, hDbc, SQL_HANDLE_DBC);

            retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
            checkError(retcode, hStmt, SQL_HANDLE_STMT);

            printf("Stock data inserted successfully.\n");

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        }
        else {
            printf("Invalid input format. Please try again.\n");
        }

        printf("Enter next stock data (or press Enter to finish):\n");
    }

    // 삽입 후 데이터 조회 호출
    selectStockData(hDbc);
}

void selectStockData(SQLHDBC hDbc) {
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    SQLWCHAR sqlQueryW[] = L"SELECT stock_id, stock_name, ticker_symbol, market, last_price FROM STOCKS";

    SQLINTEGER stockId = 0;
    SQLWCHAR stockName[50] = { 0 };
    SQLWCHAR tickerSymbol[10] = { 0 };
    SQLWCHAR market[50] = { 0 };
    SQLDOUBLE lastPrice = 0.0;

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
    checkError(retcode, hStmt, SQL_HANDLE_STMT);

    retcode = SQLBindCol(hStmt, 1, SQL_C_SLONG, &stockId, 0, NULL);
    retcode = SQLBindCol(hStmt, 2, SQL_C_WCHAR, stockName, sizeof(stockName), NULL);
    retcode = SQLBindCol(hStmt, 3, SQL_C_WCHAR, tickerSymbol, sizeof(tickerSymbol), NULL);
    retcode = SQLBindCol(hStmt, 4, SQL_C_WCHAR, market, sizeof(market), NULL);
    retcode = SQLBindCol(hStmt, 5, SQL_C_DOUBLE, &lastPrice, 0, NULL);

    printf("----------------------------------------\n");
    printf("Stock Data:\n");
    while (SQLFetch(hStmt) != SQL_NO_DATA) {
        wprintf(L"ID: %d, Name: %ls, Ticker: %ls, Market: %ls, Last Price: %.2f\n",
            stockId, stockName, tickerSymbol, market, lastPrice);
    }
    printf("----------------------------------------\n\n");
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void updateStockData(SQLHDBC hDbc) {
    int stockId;
    char stockName[50];
    char tickerSymbol[10];
    char market[50];
    double lastPrice;

    printf("Enter Stock ID to update: ");
    scanf_s("%d", &stockId);
    printf("Enter new Stock Name: ");
    scanf_s("%s", stockName, (unsigned)_countof(stockName));
    printf("Enter new Ticker Symbol: ");
    scanf_s("%s", tickerSymbol, (unsigned)_countof(tickerSymbol));
    printf("Enter new Market: ");
    scanf_s("%s", market, (unsigned)_countof(market));
    printf("Enter new Last Price: ");
    scanf_s("%lf", &lastPrice);

    SQLHSTMT hStmt;
    SQLRETURN retcode;
    char sqlQuery[512];
    SQLWCHAR sqlQueryW[512];

    sprintf_s(sqlQuery, sizeof(sqlQuery), "UPDATE STOCKS SET stock_name = '%s', ticker_symbol = '%s', market = '%s', last_price = %f WHERE stock_id = %d",
        stockName, tickerSymbol, market, lastPrice, stockId);

    MultiByteToWideChar(CP_ACP, 0, sqlQuery, -1, sqlQueryW, 512);

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
    checkError(retcode, hStmt, SQL_HANDLE_STMT);

    printf("Stock data updated successfully.\n");

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void deleteStockData(SQLHDBC hDbc) {
    int stockId;
    printf("Enter Stock ID to delete: ");
    scanf_s("%d", &stockId);

    SQLHSTMT hStmt;
    SQLRETURN retcode;
    char sqlQuery[256];
    SQLWCHAR sqlQueryW[256];

    sprintf_s(sqlQuery, sizeof(sqlQuery), "DELETE FROM STOCKS WHERE stock_id = %d", stockId);

    MultiByteToWideChar(CP_ACP, 0, sqlQuery, -1, sqlQueryW, 256);

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
    checkError(retcode, hStmt, SQL_HANDLE_STMT);

    printf("Stock data deleted successfully.\n");

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}
