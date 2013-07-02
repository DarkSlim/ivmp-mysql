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
#include "MySQLHandle.h"

#include <my_global.h>
#include <mysql.h>

MySQLHandle::MySQLHandle(const char* host, const char* username, const char* password, const char* database, bool enableMT, int port)
{
	// Set default ID just in case any errors occur
	m_handleId = INVALID_MYSQL_HANDLE;

	m_storedResult = NULL;
	m_storedRow = NULL;
	m_escapedString = NULL;

	if (enableMT)
		return; // Module currently doesn't support multithreading!

	// Initialise MySQL and open connection
	m_conn = mysql_init(NULL);

	if (m_conn == NULL)
		return;

	m_conn = mysql_real_connect(m_conn, host, username, password, database, port, NULL, NULL);

	if (m_conn == NULL)
		return;

	// Handle should be safe to have an ID signed so give it ID 0 (blank ID)
	m_handleId = 0;
}

MySQLHandle::~MySQLHandle()
{
}

const bool MySQLHandle::isValid()
{
	// true if m_conn is not NULL and m_handleId is not INVALID_MYSQL_HANDLE
	return !(m_conn == NULL) && (m_handleId != INVALID_MYSQL_HANDLE);
}

const unsigned int MySQLHandle::getHandleId()
{
	return m_handleId;
}

void MySQLHandle::setHandleId(unsigned int id)
{
	m_handleId = id;
}

void MySQLHandle::closeConnection()
{
	if (m_conn != NULL)
	{
		mysql_close(m_conn);
		if (m_storedResult != NULL) freeResult();

		m_handleId = INVALID_MYSQL_HANDLE;
	}
}

unsigned int MySQLHandle::getLastError()
{
	if (!isValid())
		return 0;

	return mysql_errno(m_conn);
}

const char* MySQLHandle::getLastErrorString()
{
	if (!isValid())
		return NULL;

	return mysql_error(m_conn);
}

int MySQLHandle::ping()
{
	if (!isValid())
		return -1;

	return mysql_ping(m_conn);
}

int MySQLHandle::executeQuery(const char* query)
{
	if (!isValid())
		return -1;

	return mysql_query(m_conn, query);
}

bool MySQLHandle::storeResult()
{
	if (!isValid())
		return false;

	m_storedResult = mysql_store_result(m_conn);

	if (m_storedResult == NULL)
		return false;

	return true;
}

bool MySQLHandle::freeResult()
{
	if (!isValid() || m_storedResult == NULL)
		return false;

	mysql_free_result(m_storedResult);
	m_storedResult = NULL;
	m_storedRow = NULL;

	return true;
}

int MySQLHandle::getResultRowCount()
{
	if (!isValid() || m_storedResult == NULL)
		return -1;

	return static_cast<int>(mysql_num_rows(m_storedResult));
}

int MySQLHandle::getResultFieldCount()
{
	if (!isValid() || m_storedResult == NULL)
		return -1;

	return static_cast<int>(mysql_num_fields(m_storedResult));
}

int MySQLHandle::getQueryAffectedRows()
{
	if (!isValid())
		return -1;

	return static_cast<int>(mysql_affected_rows(m_conn));
}

bool MySQLHandle::fetchResultRow()
{
	if (!isValid() || m_storedResult == NULL)
		return false;

	m_storedRow = mysql_fetch_row(m_storedResult);

	if (m_storedRow == NULL)
		return false;
	else
		return true;
}

const char* MySQLHandle::fetchFieldFromRow(unsigned int fieldIndex)
{
	if (!isValid() || m_storedResult == NULL || m_storedRow == NULL)
		return NULL;

	if (fieldIndex >= mysql_num_fields(m_storedResult) || fieldIndex < 0)
		return NULL;

	return m_storedRow[fieldIndex];
}

char* MySQLHandle::escapeString(const char* string)
{
	if (!isValid())
		return NULL;

	if (m_escapedString == NULL)
		freeEscapedString(); // assume the user is potentially retarded

	m_escapedString = new char[strlen(string) * 2 + 1];
	mysql_real_escape_string(m_conn, m_escapedString, string, strlen(string));

	return m_escapedString;
}

void MySQLHandle::freeEscapedString()
{
	if (m_escapedString != NULL)
	{
		delete m_escapedString;
		m_escapedString = NULL;
	}
}

int MySQLHandle::getInsertId()
{
	if (!isValid())
		return NULL;

	return static_cast<int>(mysql_insert_id(m_conn));
}