#line 2 "togo/core/threading/thread/posix.ipp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.
*/

#include <togo/core/config.hpp>
#include <togo/core/types.hpp>
#include <togo/core/error/assert.hpp>
#include <togo/core/memory/memory.hpp>
#include <togo/core/threading/types.hpp>
#include <togo/core/threading/thread.hpp>

#include <cstring>

#include <unistd.h>
#include <sys/syscall.h>
#include <sched.h>
#include <pthread.h>

namespace togo {

struct Thread {
	ThreadData data{};
	pthread_t handle{0};
};

void* posix_thread_start(void* const t_void) {
	Thread* const t = static_cast<Thread*>(t_void);
	void* exit_value = thread_runner(t->data);
	return exit_value;
}

// thread interface implementation

#define POSIX_CHECK(f) \
	if (((err) = (f))) { goto posix_error; }

bool thread::is_main() {
	return static_cast<pid_t>(syscall(SYS_gettid)) == getpid();
}

void thread::yield() {
	sched_yield();
}

Thread* thread::create(
	char const* name,
	void* call_data,
	void* (*func)(void* call_data),
	Allocator& allocator
) {
	Thread* const t = TOGO_CONSTRUCT_DEFAULT(allocator, Thread);
	thread_data_init(t->data, name, call_data, func, allocator);

	signed err = 0;
	pthread_attr_t attr;
	POSIX_CHECK(pthread_attr_init(&attr));
	POSIX_CHECK(pthread_create(&t->handle, &attr, posix_thread_start, t));
	POSIX_CHECK(pthread_attr_destroy(&attr));
	return t;

posix_error:
	TOGO_ASSERTF(false, "failed to create thread '%s': %d, %s", name, err, std::strerror(err));
}

char const* thread::name(Thread* t) {
	return t->data.name;
}

void* thread::join(Thread* t) {
	signed err = 0;
	void* exit_value = nullptr;
	POSIX_CHECK(pthread_join(t->handle, &exit_value));
	if (exit_value == PTHREAD_CANCELED) {
		exit_value = nullptr;
	}
	TOGO_DESTROY(*t->data.allocator, t);
	return exit_value;

posix_error:
	TOGO_ASSERTF(false, "failed to join thread '%s': %d, %s", t->data.name, err, std::strerror(err));
}

} // namespace togo
