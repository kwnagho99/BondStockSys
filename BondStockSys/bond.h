#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stddef.h>

void insertBondData(SQLHDBC hDbc);
void selectBondData(SQLHDBC hDbc);
void updateBondData(SQLHDBC hDbc);
void deleteBondData(SQLHDBC hDbc);
