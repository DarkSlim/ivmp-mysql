/*
    This file is part of the open source IVMP-MySQL project.

    IVMP-MySQL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    IVMP-MySQL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with IVMP-MySQL.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef IVMP_MYSQL_FUNCTIONS_H
#define IVMP_MYSQL_FUNCTIONS_H

#include "SDK/SDK.h"

// [ Squirrel Macros ]

// These macros are convenience macros to use the Squirrel VM

/* 
 * Forces the amount of parameters (sq_gettop() - 1) to be at least 'count'
 * funcName should be the name of the function called
 */
#define MIN_PARAMS(count, funcName) \
		if (sq_gettop(vm) <= count) \
		{ \
			char errorBuf[256]; \
			sprintf(errorBuf, "Invalid syntax (expected %i params) at %s", count, funcName); \
			sq_throwerror(vm, errorBuf); \
			return -1; \
		}


#define GET_PARAMCOUNT() \
	sq_gettop(vm) - 1

#define GET_STRING(idx, to) \
	sq_getstring(vm, idx + 1, &to)

#define GET_INTEGER(idx, to) \
	sq_getinteger(vm, idx + 1, &to)

#define GET_FLOAT(idx, to) \
	sq_getfloat(vm, idx + 1, &to)

#define GET_USERPOINTER(idx, to) \
	sq_getuserpointer(vm, idx + 1, &to)

#define GRAB_HANDLE(x) \
	MySQLHandle* handle = HandleManager::GetInstance()->grabHandle(x)

#define RELEASE_HANDLE(x) \
	HandleManager::GetInstance()->releaseHandle(x)

#define CHECK_HANDLE(handle) \
	if (handle == NULL || handle->getHandleId() == INVALID_MYSQL_HANDLE) \
	{ \
		sq_pushinteger(vm, -1); \
		return 1; \
	}

#define CHECK_HANDLE_BOOL(handle) \
	if (handle == NULL || handle->getHandleId() == INVALID_MYSQL_HANDLE) \
	{ \
		sq_pushbool(vm, false); \
		return 1; \
	}
// [ Squirrel Macros ]

int plugin_mysql_connect(HSQUIRRELVM vm);

int plugin_mysql_close(HSQUIRRELVM vm);

int plugin_mysql_errno(HSQUIRRELVM vm);

int plugin_mysql_error(HSQUIRRELVM vm);

int plugin_mysql_ping(HSQUIRRELVM vm);

int plugin_mysql_escape_string(HSQUIRRELVM vm);

int plugin_mysql_free_escaped_string(HSQUIRRELVM vm);

int plugin_mysql_query(HSQUIRRELVM vm);

int plugin_mysql_query_callback(HSQUIRRELVM vm);

int plugin_mysql_store_result(HSQUIRRELVM vm);

int plugin_mysql_free_result(HSQUIRRELVM vm);

int plugin_mysql_num_rows(HSQUIRRELVM vm);

int plugin_mysql_num_fields(HSQUIRRELVM vm);

int plugin_mysql_fetch_row(HSQUIRRELVM vm);

int plugin_mysql_fetch_field_row(HSQUIRRELVM vm);

#endif