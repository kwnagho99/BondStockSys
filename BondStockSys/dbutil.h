#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

void checkError(SQLRETURN retcode, SQLHANDLE handle, SQLSMALLINT type);


