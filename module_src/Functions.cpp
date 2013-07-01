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

#include "Functions.h"

#include "HandleManager.h"
#include "MySQLHandle.h"

int plugin_mysql_connect(HSQUIRRELVM vm)
{
	const char* host;
	const char* username;
	const char* password;
	const char* database;
	int port = 3306;

	MIN_PARAMS(4, "mysql_connect");
	GET_STRING(1, host);
	GET_STRING(2, username);
	GET_STRING(3, password);
	GET_STRING(4, database);

	if (GET_PARAMCOUNT() >= 5)
		GET_INTEGER(5, port);

	unsigned int id = HandleManager::GetInstance()->createHandle(host, username, password, database, false, 3306);

	if (id == INVALID_MYSQL_HANDLE)
	{
		sq_pushinteger(vm, INVALID_MYSQL_HANDLE);
		return 1;
	}


	sq_pushinteger(vm, id);
	return 1;
}

int plugin_mysql_close(HSQUIRRELVM vm)
{
	int id;

	MIN_PARAMS(1, "mysql_close");
	GET_INTEGER(1, id);

	bool success = HandleManager::GetInstance()->removeHandle(id);
	sq_pushbool(vm, success);
	
	return 1;
}

int plugin_mysql_errno(HSQUIRRELVM vm)
{
	int id;

	MIN_PARAMS(1, "mysql_errno");
	GET_INTEGER(1, id);

	GRAB_HANDLE(id);
	CHECK_HANDLE(handle);

	unsigned int err = handle->getLastError();
	RELEASE_HANDLE(id);

	sq_pushinteger(vm, err);
	return 1;
}

int plugin_mysql_ping(HSQUIRRELVM vm)
{
	int id;

	MIN_PARAMS(1, "mysql_ping");
	GET_INTEGER(1, id);

	GRAB_HANDLE(id);
	CHECK_HANDLE(handle);

	int ping = handle->ping();
	RELEASE_HANDLE(id);

	sq_pushinteger(vm, ping);
	return 1;
}

int plugin_mysql_escape_string(HSQUIRRELVM vm)
{
	int id;
	const char* string;

	MIN_PARAMS(2, "mysql_escape_string");
	GET_INTEGER(1, id);
	GET_STRING(2, string);

	GRAB_HANDLE(id);
	CHECK_HANDLE(handle);

	char* escapedString = handle->escapeString(string);
	RELEASE_HANDLE(id);

	sq_pushstring(vm, escapedString, strlen(escapedString));
	return 1;
}

int plugin_mysql_free_escaped_string(HSQUIRRELVM vm)
{
	int id;

	MIN_PARAMS(1, "mysql_free_escaped_string");
	GET_INTEGER(1, id);

	GRAB_HANDLE(id);
	CHECK_HANDLE(handle);

	handle->freeEscapedString();
	RELEASE_HANDLE(id);

	return 0;
}

int plugin_mysql_query(HSQUIRRELVM vm)
{
	int id;
	const char* query;

	MIN_PARAMS(2, "mysql_query");
	GET_INTEGER(1, id);
	GET_STRING(2, query);

	GRAB_HANDLE(id);
	CHECK_HANDLE(handle);

	int returnValue = handle->executeQuery(query);
	RELEASE_HANDLE(id);

	sq_pushinteger(vm, returnValue);
	return 1;
}

int plugin_mysql_query_callback(HSQUIRRELVM vm)
{
	int id;
	const char* query;
	SQUserPointer callback;

	MIN_PARAMS(3, "mysql_query_callback");
	GET_INTEGER(1, id);
	GET_STRING(2, query);
	GET_USERPOINTER(3, callback);

	GRAB_HANDLE(id);
	CHECK_HANDLE(handle);

	int returnValue = handle->executeQuery(query);
	RELEASE_HANDLE(id);

	sq_pushuserpointer(vm, callback);
	sq_pushinteger(vm, returnValue);
	sq_call(vm, 2, false, true);

	return 0;
}

int plugin_mysql_store_result(HSQUIRRELVM vm)
{
	int id;
	
	MIN_PARAMS(1, "mysql_store_result");
	GET_INTEGER(1, id);

	GRAB_HANDLE(id);
	CHECK_HANDLE_BOOL(handle);

	bool success = handle->storeResult();
	RELEASE_HANDLE(id);

	sq_pushbool(vm, success);
	return 1;
}

int plugin_mysql_free_result(HSQUIRRELVM vm)
{
	int id;

	MIN_PARAMS(1, "mysql_free_result");
	GET_INTEGER(1, id);

	GRAB_HANDLE(id);
	CHECK_HANDLE_BOOL(handle);

	bool success = handle->freeResult();
	RELEASE_HANDLE(id);

	sq_pushbool(vm, success);
	return 1;
}

int plugin_mysql_num_rows(HSQUIRRELVM vm)
{
	int id;

	MIN_PARAMS(1, "mysql_num_rows");
	GET_INTEGER(1, id);

	GRAB_HANDLE(id);
	CHECK_HANDLE(handle);

	int numRows = handle->getResultRowCount();
	RELEASE_HANDLE(id);

	sq_pushinteger(vm, numRows);
	return 1;
}

int plugin_mysql_num_fields(HSQUIRRELVM vm)
{
	int id;

	MIN_PARAMS(1, "mysql_num_fields");
	GET_INTEGER(1, id);

	GRAB_HANDLE(id);
	CHECK_HANDLE(handle);

	int numFields = handle->getResultFieldCount();
	RELEASE_HANDLE(id);

	sq_pushinteger(vm, numFields);
	return 1;
}

int plugin_mysql_fetch_row(HSQUIRRELVM vm)
{
	int id;

	MIN_PARAMS(1, "mysql_fetch_row");
	GET_INTEGER(1, id);

	GRAB_HANDLE(id);
	CHECK_HANDLE_BOOL(handle);

	bool success = handle->fetchResultRow();
	RELEASE_HANDLE(id);

	sq_pushbool(vm, success);
	return 1;
}

int plugin_mysql_fetch_field_row(HSQUIRRELVM vm)
{
	int id;
	int fieldIndex;

	MIN_PARAMS(2, "mysql_fetch_field_row");
	GET_INTEGER(1, id);
	GET_INTEGER(2, fieldIndex);

	GRAB_HANDLE(id);
	
	if (handle == NULL || !handle->isValid())
	{
		sq_pushinteger(vm, 0);
		return 1;
	}

	const char* data = handle->fetchFieldFromRow(fieldIndex);

	RELEASE_HANDLE(id);

	if (data == NULL)
	{
		sq_pushinteger(vm, 0);
		return 1;
	}

	sq_pushstring(vm, data, strlen(data));
	return 1;
}