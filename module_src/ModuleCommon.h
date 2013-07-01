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

#ifndef IVMP_MYSQL_COMMON_H
#define IVMP_MYSQL_COMMON_H

#define PLATFORM_WINDOWS    1
#define PLATFORM_UNIX       2

#define COMPILER_MICROSOFT  1
#define COMPILER_GNU        2

// Determine platform
#if defined(_WIN64) // must be first as _WIN64 also defines _WIN32
#	define PLATFORM PLATFORM_WINDOWS
#elif defined(_WIN32) || defined(WIN32) || defined(__WIN32__)
#	define PLATFORM PLATFORM_WINDOWS
#else // maybe change to __gnu_linux__?
#	define PLATFORM PLATFORM_UNIX
#endif

// Determine compiler
#if defined(_MSC_VER)
#	define COMPILER COMPILER_MICROSOFT
#elif defined(__GNUC__)
#	define COMPILER COMPILER_GNU
#else
#	error "FATAL: Unknown compiler."
#endif

#if COMPILER == COMPILER_MICROSOFT
#	pragma warning(disable : 4267)             // conversion from 'size_t' to 'int', possible loss of data
#endif

// Module information
#define MODULE_INFO_NAME        "IVMP-MySQL"
#define MODULE_VERSION_STRING   "v0.0.1"

// Module defines
#define INVALID_MYSQL_HANDLE    -1
#define MAX_MYSQL_HANDLES       10

#endif