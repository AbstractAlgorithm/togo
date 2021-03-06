#line 2 "togo/core/threading/mutex/posix.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.
*/

#pragma once

#include <togo/core/config.hpp>

#include <pthread.h>

namespace togo {

// Forward declarations
struct Mutex;

struct PosixMutexImpl {
	pthread_mutex_t handle;

	PosixMutexImpl(Mutex& m);
	~PosixMutexImpl();
};

using MutexImpl = PosixMutexImpl;

} // namespace togo
