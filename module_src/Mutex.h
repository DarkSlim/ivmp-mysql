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

#ifndef IVMP_MYSQL_MUTEX_H
#define IVMP_MYSQL_MUTEX_H

#include "ModuleCommon.h"

/*
	Why you should use boost mutexes:
		BECAUSE IT'S BOOST

		But honestly, boost is just much easier to use for me. I think I may
		have implemented Windows mutexes wrong anyhow. If you'd rather use the
		crappy native mutex implementation that I wrote, be my guest and just comment
		out the below define.

		Oh yeah, and another reason to sue boost mutexes is that we're going to use
		boost threads for async queries so it would make sense to keep all the eggs
		in one basket :)
*/
#define USE_BOOST_MUTEX

// Headers
#ifndef USE_BOOST_MUTEX
#	if PLATFORM == PLATFORM_WINDOWS
#	include <WinSock2.h>
#	include <process.h>
#	elif PLATFORM == PLATFORM_UNIX
#	include <pthread.h>
#	endif
#endif

#ifdef USE_BOOST_MUTEX
#	include <boost/thread/mutex.hpp>
#endif

// Data types
#ifndef USE_BOOST_MUTEX
#	if PLATFORM == PLATFORM_WINDOWS
#	define native_mutex_t HANDLE
#	elif PLATFORM == PLATFORM_UNIX
#	define native_mutex_t pthread_mutex_t
#	endif
#endif

class Mutex
{
public:
	Mutex();
	~Mutex();

	void lock();
	void unlock();

private:
#ifndef USE_BOOST_MUTEX
	native_mutex_t m_nativeMutex;
#else
	boost::mutex m_boostMutex;
#endif
};

#endif