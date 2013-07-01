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

#ifndef IVMP_MYSQL_HANDLEMANAGER_H
#define IVMP_MYSQL_HANDLEMANAGER_H

#include "ModuleCommon.h"
#include <map>

class Mutex;
class MySQLHandle;

class HandleManager
{
public:
	static void CreateInstance();
	static HandleManager* GetInstance();

	/* 
		Creates a MySQLHandle with the given parameters and returns its ID on success or INVALID_MYSQL_HANDLE on failure.
		Adds the created handle to the handlemap.
	*/
	unsigned int createHandle(const char* host, const char* usr, const char* passwd, const char* db, bool enableMT, int port);

	/*
		If a handle with the passed id exists, it will be grabbed as soon as the associatied mutex can be locked and then
		the handle is returned.

		When you are finished with the MySQLHandle, remember to call HandleManager::GetInstance()->releaseHandle(id)
	*/
	MySQLHandle* grabHandle(unsigned int id);

	/*
		If a handle with the passed id exists and has been grabbed, the associated mutex will be unlocked and
		it can be grabbed again.
	*/
	void releaseHandle(unsigned int id);

	/*
		If the passed handle has been grabbed, the associated mutex will be unlocked and it can be grabbed again.
	*/
	void releaseHandle(MySQLHandle* handle);

	/*
		Removes the handle associatied with the given id if it exists.
	*/
	bool removeHandle(unsigned int id);


	/*
		Returns the amount of active MySQLHandle instances.
	*/
	unsigned int getCount();

private:
	static HandleManager* InstancePtr;

	HandleManager();
	~HandleManager();

	typedef std::map<unsigned int, MySQLHandle*> handleMap_t;
	typedef std::map<unsigned int, Mutex*> mutexMap_t;

	handleMap_t m_handleMap;
	mutexMap_t m_mutexMap;
};

#endif