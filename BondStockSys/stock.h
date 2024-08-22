#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stddef.h>
#include "dbutil.h"


void insertStockData(SQLHDBC hDbc);
void selectStockData(SQLHDBC hDbc);
void updateStockData(SQLHDBC hDbc);
void deleteStockData(SQLHDBC hDbc);

