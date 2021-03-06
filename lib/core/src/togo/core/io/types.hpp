#line 2 "togo/core/io/types.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.

@file
@brief IO types.
@ingroup lib_core_types
@ingroup lib_core_io
*/

#pragma once

#include <togo/core/config.hpp>

namespace togo {

/**
	@addtogroup lib_core_io
	@{
*/

// Forward declarations
class IStreamBase;
class IStreamSeekable;
class IReader;
class IWriter;

/// IO stream status.
struct IOStatus {
	enum : unsigned {
		/// No flags.
		flag_none = 0,
		/// Fail flag.
		flag_fail = 1 << 0,
		/// End-of-stream flag.
		flag_eof  = 1 << 1,
	};

	unsigned _value;

	/// Set status and return self.
	IOStatus& assign(bool const fail, bool const eof) {
		_value
			= fail ? flag_fail : flag_none
			| eof  ? flag_eof  : flag_none
		;
		return *this;
	}

	/// Clear status and return self.
	IOStatus& clear() {
		_value = flag_none;
		return *this;
	}

	/// Whether there are no errors.
	constexpr bool ok() const noexcept {
		return !_value;
	}

	/// Whether an operation failed.
	constexpr bool fail() const noexcept {
		return _value & flag_fail;
	}

	/// Whether an operation failed and reached the end of the stream.
	///
	/// Writers that have a bound stream size (i.e., non-growing) may
	/// set this flag.
	constexpr bool eof() const noexcept {
		return _value & flag_eof;
	}

	/// Whether there are no errors.
	constexpr operator bool() const noexcept {
		return ok();
	}
};

/** @} */ // end of doc-group lib_core_io

} // namespace togo
