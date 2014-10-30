#line 2 "togo/log/log.cpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.
*/

#include <togo/config.hpp>
#include <togo/log/log.hpp>
#include <togo/threading/mutex.hpp>

#include <cstdarg>
#include <cstdlib>
#include <cstdio>

// TODO: Print into buffer, then flush buffer to streams

namespace togo {

namespace {
	static Mutex _log_mutex{};
} // anonymous namespace

void log::printf(char const* const msg, ...) {
	MutexLock l{_log_mutex};
	va_list va;
	va_start(va, msg);
	std::vprintf(msg, va);
	va_end(va);
}

} // namespace togo
