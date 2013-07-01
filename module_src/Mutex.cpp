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

#include "Mutex.h"

Mutex::Mutex()
{
#ifndef USE_BOOST_MUTEX
#	if PLATFORM == PLATFORM_WINDOWS
		m_nativeMutex = CreateMutex(0, FALSE, 0);
#	elif PLATFORM == PLATFORM_UNIX
		pthread_mutex_init(&m_nativeMutex, NULL);
#	endif
#endif
}

Mutex::~Mutex()
{
}

void Mutex::lock()
{
#ifndef USE_BOOST_MUTEX
#	if PLATFORM == PLATFORM_WINDOWS
		WaitForSingleObject(m_nativeMutex, INFINITE);
#	elif PLATFORM == PLATFORM_UNIX
		pthread_mutex_lock(&m_nativeMutex);
#	endif
#endif

#ifdef USE_BOOST_MUTEX
		m_boostMutex.lock();
#endif
}

void Mutex::unlock()
{
#ifndef USE_BOOST_MUTEX
#	if PLATFORM == PLATFORM_WINDOWS
		ReleaseMutex(m_nativeMutex);
#	elif PLATFORM == PLATFORM_UNIX
		pthread_mutex_unlock(&m_nativeMutex);
#	endif
#endif

#ifdef USE_BOOST_MUTEX
		m_boostMutex.unlock();
#endif
}