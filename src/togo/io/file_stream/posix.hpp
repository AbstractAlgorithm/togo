#line 2 "togo/io/file_stream/posix.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.
*/

#pragma once

#include <togo/config.hpp>

#include <cstdio>

namespace togo {

struct PosixFileStreamData {
	std::FILE* handle{nullptr};
	IOStatus status{IOStatus::flag_none};
};

using FileStreamData = PosixFileStreamData;

} // namespace togo
