#line 2 "togo/assert.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.

@file assert.hpp
@brief Assertion macros and debugging.
@ingroup debug
*/

#pragma once

#include <togo/config.hpp>

namespace togo {

/**
	@addtogroup debug
	@{
*/

/// Abort the program with contextual information.
void
error_abort(
	unsigned line,
	char const* file,
	char const* msg,
	...
);

/// Print debug message.
void
debug_print(
	unsigned line,
	char const* file,
	char const* msg,
	...
);

#if !defined(TOGO_DISABLE_ASSERTIONS) && !defined(DOXYGEN_CONSISTS_SOLELY_OF_UNICORNS_AND_CONFETTI)
	#define TOGO_ASSERT(expr, msg) \
		do { if (!(expr)) { error_abort(__LINE__, __FILE__, \
		"Assertion failed: `" #expr "`: " msg "\n"); } } while (false)
	#define TOGO_ASSERTF(expr, msg, ...) \
		do { if (!(expr)) { error_abort(__LINE__, __FILE__, \
		"Assertion failed: `" #expr "`: " msg "\n", __VA_ARGS__); } } while (false)
	#define TOGO_ASSERTE(expr) \
		do { if (!(expr)) { error_abort(__LINE__, __FILE__, \
		"Assertion failed: `" #expr "`\n"); } } while (false)
#else
	/// Assertion with an error message.
	#define TOGO_ASSERT(expr, msg) ((void)0)
	/// Assertion with a formatted error message.
	#define TOGO_ASSERTF(expr, msg, ...) ((void)0)
	/// Assertion with no message.
	#define TOGO_ASSERTE(expr) ((void)0)
#endif

#if defined(TOGO_DEBUG) && !defined(TOGO_DISABLE_ASSERTIONS)
	#define TOGO_DEBUG_ASSERT(expr, msg) TOGO_ASSERT(expr, msg)
	#define TOGO_DEBUG_ASSERTF(expr, msg, ...) TOGO_ASSERTF(expr, msg, __VA_ARGS__)
	#define TOGO_DEBUG_ASSERTE(expr) TOGO_ASSERTE(expr)
#else
	/// Debug assertion with an error message.
	#define TOGO_DEBUG_ASSERT(expr, msg) ((void)0)
	/// Debug assertion with a formatted error message.
	#define TOGO_DEBUG_ASSERTF(expr, msg, ...) ((void)0)
	/// Debug assertion with no message.
	#define TOGO_DEBUG_ASSERTE(expr) ((void)0)
#endif

#if defined(TOGO_DEBUG)
	#define TOGO_DEBUG_MSG(msg) do { debug_print(__LINE__, __FILE__, \
		msg); } while (false)
	#define TOGO_DEBUG_MSGF(msg, ...) do { debug_print(__LINE__, __FILE__, \
		msg, __VA_ARGS__); } while (false)
#else
	/// Debug message.
	#define TOGO_DEBUG_MSG(msg) ((void)0)
	/// Debug message with formatting.
	#define TOGO_DEBUG_MSGF(msg, ...) ((void)0)
#endif

/** @} */ // end of doc-group debug

} // namespace togo
