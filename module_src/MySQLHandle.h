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

#ifndef IVMP_MYSQL_MYSQLHANDLE_H
#define IVMP_MYSQL_MYSQLHANDLE_H

struct st_mysql;
typedef st_mysql MYSQL;
struct st_mysql_res;
typedef st_mysql_res MYSQL_RES;
typedef char** MYSQL_ROW;

class MySQLHandle
{
	friend class HandleManager;

public:
	/*
		Returns true if the MySQLHandle is valid and can perform operations.
	*/
	const bool isValid();

	/*
		Returns the ID associatied with this MySQLHandle.
	*/
	const unsigned int getHandleId();

	/*
		Safely close the MySQLHandle.
	*/
	void closeConnection();

	/*
		Returns the error code for the most recently invoked function that can succeed or fail.
		If it returns 0, no error occurred.
	*/
	unsigned int getLastError();

	/*
		Returns the error message of the last error.
		If it returns 0, no error occurred.
	*/
	const char* getLastErrorString();

	/*
		Returns 0 if connection is still active, non-zero on error.
	*/
	int ping();

	/*
		Attempts to execute a query on the MySQLHandle.
		Returns 0 on success, non-zero on error.
	*/
	int executeQuery(const char* query);

	/*
		Attempts to store the result form the most recently executed query.
		Returns true on success.
	*/
	bool storeResult();

	/*
		Frees the currently stored result.
		Returns true if a result was freed.
	*/
	bool freeResult();

	/*
		Returns the amount of rows from the stored result, if one is available.
		Returns -1 on any error.
	*/
	int getResultRowCount();

	/*
		Returns the amount of fields for each row of the stored result, if one is available.
		Returns -1 on any error.
	*/
	int getResultFieldCount();

	/*
		Returns the amount of affected rows by the last query.
		Returns -1 on any error.
	*/
	int getQueryAffectedRows();

	/*
		Returns true if a row was successfully fetched from the stored result set.
		Returns false when there are no more rows to retrieve.
	*/
	bool fetchResultRow();

	/*
		Returns the data from the currently stored row at the specified field index, if one
		is available. Otherwise returns NULL.
	*/
	const char* fetchFieldFromRow(unsigned int fieldIndex);

	/*
		Escapes the passed string (mysql_real_escape_string) and returns it.
		The string will need to be freed (handle->freeEscapedString()) or the module function 
		mysql_free_escaped_string(handle)!
	*/
	char* escapeString(const char* string);

	/*
		If a string has been escaped, call this to safely free it from memory.
	*/
	void freeEscapedString();

	/*
		Returns the insert ID (mysql_insert_id) if one is available, otherwise -1.
	*/
	int getInsertId();

private:
	MySQLHandle(const char* host, const char* username, const char* password, const char* database, bool enableMT, int port);
	~MySQLHandle();

	/*
		Private method to set m_handleId. Should only be called by HandleManager!
	*/
	void setHandleId(unsigned int id);

	unsigned int m_handleId;
	MYSQL* m_conn;
	MYSQL_RES* m_storedResult;
	MYSQL_ROW m_storedRow;
	char* m_escapedString;
};

#endif