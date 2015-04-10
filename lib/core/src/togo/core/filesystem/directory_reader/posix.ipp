#line 2 "togo/core/filesystem/directory_reader/posix.ipp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.
*/

#include <togo/core/config.hpp>
#include <togo/core/error/assert.hpp>
#include <togo/core/utility/utility.hpp>
#include <togo/core/log/log.hpp>
#include <togo/core/collection/fixed_array.hpp>
#include <togo/core/string/string.hpp>
#include <togo/core/filesystem/filesystem.hpp>
#include <togo/core/filesystem/directory_reader/private.hpp>

#include <cerrno>
#include <cstring>

#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>

namespace togo {

PosixDirectoryReaderImpl::PosixDirectoryReaderImpl()
	: current_size(0)
	, nodes()
	, path()
{}

bool directory_reader::open(
	DirectoryReader& reader,
	StringRef const& path,
	bool const recursive,
	bool const ignore_dotfiles
) {
	auto& impl = reader._impl;
	TOGO_ASSERT(
		fixed_array::empty(impl.nodes),
		"the reader is already open"
	);

	directory_reader::set_options(reader, recursive, ignore_dotfiles);
	string::copy(impl.path, path);
	string::trim_trailing_slashes(impl.path);
	fixed_array::back(impl.path) = '/';
	fixed_array::push_back(impl.path, '\0');

	DIR* const handle = ::opendir(fixed_array::begin(impl.path));
	if (handle) {
		fixed_array::push_back(impl.nodes, {handle, 0});
		impl.current_size = string::size(impl.path);
		return true;
	} else {
		TOGO_LOG_DEBUGF(
			"directory_reader::open: errno = %d, %s\n",
			errno, std::strerror(errno)
		);
		return false;
	}
}

void directory_reader::close(
	DirectoryReader& reader
) {
	auto& impl = reader._impl;
	if (fixed_array::any(impl.nodes)) {
		while (fixed_array::any(impl.nodes)) {
			auto const& node = fixed_array::back(impl.nodes);
			TOGO_DEBUG_ASSERTE(::closedir(node.handle));
		}
		fixed_array::clear(impl.nodes);
		fixed_array::clear(impl.path);
	}
}

bool directory_reader::read(
	DirectoryReader& reader,
	DirectoryEntry& entry,
	DirectoryEntry::Type const type_mask
) {
	TOGO_DEBUG_ASSERTE(type_mask != static_cast<DirectoryEntry::Type>(0));

	auto& impl = reader._impl;
	TOGO_ASSERT(
		fixed_array::any(impl.nodes),
		"the reader is not open"
	);

	// If last entry was a file or not traversed, reset path to the
	// node level
	if (impl.current_size != string::size(impl.path)) {
		fixed_array::resize(impl.path, impl.current_size + 1);
		fixed_array::back(impl.path) = '\0';
	}

	auto const* node = &fixed_array::back(impl.nodes);
	struct ::dirent const* ent;
	DIR* recurse_handle;
	while (node) {
		ent = ::readdir(node->handle);
		if (!ent) {
			// Ascend
			fixed_array::resize(impl.path, node->size + 1);
			fixed_array::back(impl.path) = '\0';
			fixed_array::pop_back(impl.nodes);
			node
				= fixed_array::any(impl.nodes)
				? &fixed_array::back(impl.nodes)
				: nullptr
			;
			impl.current_size = string::size(impl.path);
			continue;
		}

		StringRef const entry_name{ent->d_name, cstr_tag{}};
		if (
			directory_reader::option_ignore_dotfiles(reader) &&
			entry_name.data[0] == '.'
		) {
			continue;
		}

		switch (ent->d_type) {
		case DT_DIR:
			// Who would ever want these!?
			if (
				string::compare_equal(entry_name, ".") ||
				string::compare_equal(entry_name, "..")
			) {
				continue;
			}

			// Descend
			string::append(impl.path, entry_name);
			fixed_array::back(impl.path) = '/';
			fixed_array::push_back(impl.path, '\0');

			// Path is corrected to node level if descent is skipped
			if (!directory_reader::option_recursive(reader)) {
				// Skip descent
			} else if ((recurse_handle = ::opendir(fixed_array::begin(impl.path)))) {
				node = &fixed_array::push_back(
					impl.nodes, {recurse_handle, impl.current_size}
				);
				impl.current_size = string::size(impl.path);
			} else {
				// Skip descent
				TOGO_LOG_DEBUGF(
					"directory_reader::read: opendir() failed; ignoring entry;"
					" errno = %d, %s\n",
					errno, std::strerror(errno)
				);
			}
			if (enum_bool(type_mask & DirectoryEntry::Type::dir)) {
				entry.type = DirectoryEntry::Type::dir;
				entry.path = impl.path;
				return true;
			}
			break;

		case DT_REG:
			if (enum_bool(type_mask & DirectoryEntry::Type::file)) {
				string::append(impl.path, entry_name);
				entry.type = DirectoryEntry::Type::file;
				entry.path = impl.path;
				return true;
			}
			break;

		default:
			break;
		}
	}
	return false;
}

} // namespace togo
