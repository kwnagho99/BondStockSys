#pragma once
#define WIN32_LEAN_AND_MEAN
#include <sql.h>
#include <sqlext.h>

void executeTransaction(SQLHDBC hDbc);
