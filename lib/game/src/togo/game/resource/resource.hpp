#line 2 "togo/game/resource/resource.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.

@file
@brief Resource interface.
@ingroup lib_game_resource
*/

#pragma once

#include <togo/game/config.hpp>
#include <togo/core/error/assert.hpp>
#include <togo/core/string/types.hpp>
#include <togo/core/hash/hash.hpp>
#include <togo/core/hash/hash_combiner.hpp>
#include <togo/game/resource/types.hpp>
#include <togo/game/resource/resource_package.hpp>
#include <togo/game/resource/resource.gen_interface>

namespace togo {
namespace game {

namespace resource {

/**
	@addtogroup lib_game_resource
	@{
*/

/// Calculate hash of resource type.
inline ResourceType hash_type(StringRef const& type) {
	return hash::calc32(type);
}

/// Calculate hash of resource name.
inline ResourceNameHash hash_name(StringRef const& name) {
	return hash::calc64(name);
}

/// Calculate hash of resource tags.
inline ResourceTagsHash hash_tags(
	StringRef const* const tags,
	unsigned const num_tags
) {
	ResourceTagsHashCombiner combiner{};
	for (unsigned i = 0; i < num_tags; ++i) {
		hash_combiner::add(combiner, tags[i]);
	}
	return hash_combiner::value(combiner);
}

/// Calculate hash of resource tags.
inline ResourceTagsHash hash_tags(
	ResourcePathParts::Tag const* const tags,
	unsigned const num_tags
) {
	ResourceTagsHashCombiner combiner{};
	for (unsigned i = 0; i < num_tags; ++i) {
		hash_combiner::add(combiner, tags[i].name);
	}
	return hash_combiner::value(combiner);
}

/// Calculate hash of resource package name.
inline ResourcePackageNameHash hash_package_name(StringRef const& name) {
	return hash::calc32(name);
}

/** @} */ // end of doc-group lib_game_resource

} // namespace resource

inline ResourceCompiledPath::ResourceCompiledPath()
	: id(0)
	, _data()
{}

inline u32 ResourceCompiledPath::size() const {
	return id == 0 ? 0 : string::size(_data);
}

inline char const* ResourceCompiledPath::data() const {
	return fixed_array::begin(_data);
}

inline ResourceCompiledPath::operator StringRef() const {
	return {data(), size()};
}

inline ResourceStreamLock::~ResourceStreamLock() {
	_stream = nullptr;
	resource_package::close_resource_stream(_package);
}

inline ResourceStreamLock::ResourceStreamLock(
	ResourcePackage& package,
	u32 const id
)
	: _package(package)
	, _stream(resource_package::open_resource_stream(_package, id))
{
	TOGO_ASSERT(_stream, "failed to open resource stream");
}

inline IReader& ResourceStreamLock::stream() {
	TOGO_DEBUG_ASSERTE(_stream);
	return *_stream;
}

} // namespace game
} // namespace togo
