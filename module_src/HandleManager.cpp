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

#include "HandleManager.h"
#include "MySQLHandle.h"
#include "Mutex.h"

HandleManager* HandleManager::InstancePtr = NULL;

void HandleManager::CreateInstance()
{
	InstancePtr = new HandleManager();
}


HandleManager* HandleManager::GetInstance()
{
	return InstancePtr;
}

HandleManager::HandleManager()
{
}

HandleManager::~HandleManager()
{
	for (handleMap_t::iterator it = m_handleMap.begin(); it != m_handleMap.end(); ++it)
	{
		removeHandle(it->second->getHandleId());
	}
}

unsigned int HandleManager::createHandle(const char* host, const char* usr, const char* passwd, const char* db, bool enableMT, int port)
{
	MySQLHandle* handle = new MySQLHandle(host, usr, passwd, db, enableMT, port);

	if (!handle->isValid())
	{
		delete handle;
		return INVALID_MYSQL_HANDLE;
	}

	handle->setHandleId(1);

	m_handleMap[1] = handle;
	m_mutexMap[1] = new Mutex();

	return 1;
}

MySQLHandle* HandleManager::grabHandle(unsigned int id)
{
	if (m_handleMap[id] == NULL)
		return NULL;

	m_mutexMap[id]->lock();
	return m_handleMap[id];
}

void HandleManager::releaseHandle(unsigned int id)
{
	if (m_handleMap[id] == NULL)
		return;

	m_mutexMap[id]->unlock();
}

void HandleManager::releaseHandle(MySQLHandle* handle)
{
	if (handle == NULL || handle->getHandleId() == INVALID_MYSQL_HANDLE)
		return;

	if (m_handleMap.find(handle->getHandleId()) == m_handleMap.end())
		return; // handle isn't registered

	m_mutexMap[handle->getHandleId()]->unlock();
}

bool HandleManager::removeHandle(unsigned int id)
{
	MySQLHandle* handle = this->grabHandle(id);

	if (handle == NULL)
		return false;

	// Close the MySQL handle safely

	this->releaseHandle(id);

	// Remove MySQLHandle* from handleMap
	delete m_handleMap[id];
	m_handleMap[id] = NULL;
	
	// Remove Mutex from mutexMap
	delete m_mutexMap[id];
	m_mutexMap[id] = NULL;

	return true;
}