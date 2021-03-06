#line 2 "togo/core/filesystem/directory_reader/posix.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.
*/

#pragma once

#include <togo/core/config.hpp>
#include <togo/core/collection/types.hpp>

#include <sys/types.h>
#include <dirent.h>

namespace togo {

struct PosixDirectoryReaderImpl {
	struct Node {
		DIR* handle;
		u32 size;
	};

	u32 base_size;
	u32 current_size;
	FixedArray<Node, 32> nodes;
	FixedArray<char, 384> path;

	PosixDirectoryReaderImpl();
};

using DirectoryReaderImpl = PosixDirectoryReaderImpl;

} // namespace togo
