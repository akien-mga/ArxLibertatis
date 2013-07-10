/*
 * Copyright 2011-2012 Arx Libertatis Team (see the AUTHORS file)
 *
 * This file is part of Arx Libertatis.
 *
 * Arx Libertatis is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Arx Libertatis is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Arx Libertatis.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ARX_PLATFORM_LOCK_H
#define ARX_PLATFORM_LOCK_H

#include "Configure.h"

#if defined(ARX_HAVE_PTHREADS)
#include <pthread.h>
#elif defined(ARX_HAVE_WINAPI)
#include <windows.h>
#else
#error "Locking not supported: need either ARX_HAVE_PTHREADS or ARX_HAVE_WINAPI"
#endif

class Lock {
	
private:
	
#if defined(ARX_HAVE_PTHREADS)
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	bool locked;
#elif defined(ARX_HAVE_WINAPI)
	HANDLE mutex;
#endif
	
public:
	
	Lock();
	~Lock();
	
	void lock();
	
	void unlock();
	
};

class Autolock {
	
private:
	
	Lock * lock;
	
public:
	
	explicit Autolock(Lock * _lock) : lock(_lock) {
		lock->lock();
	}
	
	explicit Autolock(Lock & _lock) : lock(&_lock) {
		lock->lock();
	}
	
	~Autolock() {
		lock->unlock();
	}
	
};

#endif // ARX_PLATFORM_LOCK_H
