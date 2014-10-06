#line 2 "togo/resource_types.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.

@file resource_types.hpp
@brief Resource types.
@ingroup types
@ingroup resource
*/

#pragma once

#include <togo/config.hpp>
#include <togo/types.hpp>
#include <togo/hash.hpp>
#include <togo/memory_types.hpp>
#include <togo/collection_types.hpp>
#include <togo/io_types.hpp>

namespace togo {

/**
	@addtogroup resource
	@{
*/

/// Resource type.
using ResourceType = hash32;

/// Resource name hash.
using ResourceNameHash = hash64;

/// Resource type hash literal.
inline constexpr ResourceType
operator"" _resource_type(
	char const* const data,
	std::size_t const size
) {
	return hash::calc32_ce(data, size);
}

/// Resource name hash literal.
inline constexpr ResourceNameHash
operator"" _resource_name(
	char const* const data,
	std::size_t const size
) {
	return hash::calc64_ce(data, size);
}

/// Resource types.
enum : ResourceType {
	/// Non-type.
	RESTYPE_NULL = hash::IDENTITY32,

	/// gfx::RenderConfig.
	RESTYPE_RENDER_CONFIG = "render_config"_resource_type,
};

/// Resource path parts.
struct ResourcePathParts {
	struct Tag {
		StringRef name{null_tag{}};
		hash32 hash;
	};

	ResourceType type_hash;
	ResourceNameHash name_hash;
	StringRef type{null_tag{}};
	StringRef name{null_tag{}};
	FixedArray<Tag, 8> tags;
};

// Forward declarations
struct ResourcePackage;
struct ResourceHandler;
struct ResourceManager;

/**
	@addtogroup resource_package
	@{
*/

/// Resource package.
struct ResourcePackage;

/** @} */ // end of doc-group resource_package

/**
	@addtogroup resource_handler
	@{
*/

/// Resource handler.
struct ResourceHandler {
	/// Load a resource.
	///
	/// Returns pointer to resource, or nullptr on error.
	using load_func_type = void* (
		void* /*type_data*/,
		ResourceManager& /*manager*/,
		ResourceNameHash /*name_hash*/,
		IReader& /*stream*/
	);

	/// Unload a resource.
	using unload_func_type = void (
		void* /*type_data*/,
		ResourceManager& /*manager*/,
		ResourceNameHash /*name_hash*/,
		void* /*resource*/
	);

	ResourceType type;
	load_func_type* func_load;
	unload_func_type* func_unload;
};

/** @} */ // end of doc-group resource_handler

/**
	@addtogroup resource_manager
	@{
*/

/// Resource manager.
struct ResourceManager {
	struct BoundHandler {
		ResourceHandler handler;
		void* type_data;
	};

	HashMap<ResourceType, BoundHandler> _handlers;
	HashMap<ResourceNameHash, void*> _resources;

	ResourceManager() = delete;
	ResourceManager(ResourceManager const&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager const&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

	~ResourceManager() = default;

	ResourceManager(Allocator& allocator);
};

/** @} */ // end of doc-group resource_manager

/** @} */ // end of doc-group resource

} // namespace togo
