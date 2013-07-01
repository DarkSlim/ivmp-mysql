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

#include "ModuleCommon.h"
#include "SDK/SDK.h"
#include "HandleManager.h"
#include "Functions.h"

EXPORT bool InitModule(char * szModuleName)
{
	strcpy(szModuleName, MODULE_INFO_NAME);

	LogPrintf("");
	LogPrintf("  ========================================");
	LogPrintf("                  %s", MODULE_INFO_NAME);
	LogPrintf("                    %s", MODULE_VERSION_STRING);

#ifdef _DEBUG
	LogPrintf("");
	LogPrintf("                  [WARNING]               ");
	LogPrintf("        You are running a debug version   ");
	LogPrintf("                of this module!           ");
#endif

	LogPrintf("  ========================================");
	LogPrintf("");

	HandleManager::CreateInstance();

	return true;
}

EXPORT void ScriptLoad(HSQUIRRELVM vm)
{
	// Register constants
	RegisterConstant(vm, "INVALID_MYSQL_HANDLE",             -1);
	RegisterConstant(vm, "MAX_MYSQL_HANDLES",                MAX_MYSQL_HANDLES);
	RegisterConstant(vm, "IVMP_MYSQL_VERSION",               MODULE_VERSION_STRING, strlen(MODULE_VERSION_STRING));
	
	// Register functions
	RegisterFunction(vm, "mysql_connect",                    plugin_mysql_connect);
	RegisterFunction(vm, "mysql_close",                      plugin_mysql_close);
	RegisterFunction(vm, "mysql_errno",                      plugin_mysql_errno);
	RegisterFunction(vm, "mysql_error",                      plugin_mysql_error);
	RegisterFunction(vm, "mysql_ping",                       plugin_mysql_ping);
	RegisterFunction(vm, "mysql_escape_string",              plugin_mysql_escape_string);
	RegisterFunction(vm, "mysql_free_escaped_string",        plugin_mysql_free_escaped_string);
	RegisterFunction(vm, "mysql_query",                      plugin_mysql_query);
	RegisterFunction(vm, "mysql_store_result",               plugin_mysql_store_result);
	RegisterFunction(vm, "mysql_free_result",                plugin_mysql_free_result);
	RegisterFunction(vm, "mysql_num_rows",                   plugin_mysql_num_rows);
	RegisterFunction(vm, "mysql_num_fields",                 plugin_mysql_num_fields);
	//RegisterFunction(vm, "mysql_query_callback",             plugin_mysql_query_callback);
	RegisterFunction(vm, "mysql_store_result",               plugin_mysql_store_result);
	RegisterFunction(vm, "mysql_free_result",                plugin_mysql_free_result);
	RegisterFunction(vm, "mysql_num_rows",                   plugin_mysql_num_rows);
	RegisterFunction(vm, "mysql_num_fields",                 plugin_mysql_num_fields);
	RegisterFunction(vm, "mysql_fetch_row",                  plugin_mysql_fetch_row);
	RegisterFunction(vm, "mysql_fetch_field_row",            plugin_mysql_fetch_field_row);
}

EXPORT void ScriptUnload(HSQUIRRELVM vm)
{
}

EXPORT void Pulse()
{
}