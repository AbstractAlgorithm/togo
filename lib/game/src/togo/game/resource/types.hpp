#line 2 "togo/game/resource/types.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.

@file
@brief Resource types.
@ingroup lib_game_types
@ingroup lib_game_resource
*/

#pragma once

#include <togo/game/config.hpp>
#include <togo/core/types.hpp>
#include <togo/core/utility/traits.hpp>
#include <togo/core/memory/types.hpp>
#include <togo/core/collection/types.hpp>
#include <togo/core/string/types.hpp>
#include <togo/core/hash/types.hpp>
#include <togo/core/hash/hash.hpp>
#include <togo/core/io/types.hpp>
#include <togo/core/io/file_stream.hpp>

namespace togo {
namespace game {

/**
	@addtogroup lib_game_resource
	@{
*/

/// Format versions.
enum : u32 {
	/// ResourcePackage manifest format version.
	SER_FORMAT_VERSION_PKG_MANIFEST = 3,
};

/// Resource type.
using ResourceType = hash32;

/// Resource name hash.
using ResourceNameHash = hash64;

/// Combined resource tags hash.
using ResourceTagsHash = hash64;

/// Combined resource tags hash combiner.
using ResourceTagsHashCombiner = HashCombiner64;

/// Package name hash.
using ResourcePackageNameHash = hash32;

/** @cond INTERNAL */
static_assert(
	is_same<ResourceType, hash32>::value,
	"changed ResourceType type breaks binary formats,"
	" hash functions, and likely other things"
);
static_assert(
	is_same<ResourceNameHash, hash64>::value,
	"changed ResourceNameHash type breaks binary formats,"
	" hash functions, and likely other things"
);
static_assert(
	is_same<ResourceTagsHash, hash64>::value,
	"changed ResourceTagsHash type breaks binary formats,"
	" hash functions, and likely other things"
);
/** @endcond */ // INTERNAL

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

/// Combined resource tags hash literal.
///
/// Note that this only takes a single string. Tags should be sorted
/// and separator-less for the return value of this literal to be
/// compatible with the runtime combiner-based hash function.
inline constexpr ResourceTagsHash
operator"" _resource_tags(
	char const* const data,
	std::size_t const size
) {
	return hash::calc64_ce(data, size);
}

/// Package name hash literal.
inline constexpr ResourcePackageNameHash
operator"" _resource_package_name(
	char const* const data,
	std::size_t const size
) {
	return hash::calc32_ce(data, size);
}

/// Resource types.
enum : ResourceType {
	/// Non-type.
	RES_TYPE_NULL = ""_resource_type,

	/// TestResource.
	RES_TYPE_TEST = "test"_resource_type,

	/// gfx::ShaderDef.
	RES_TYPE_SHADER_PRELUDE = "shader_prelude"_resource_type,

	/// gfx::ShaderID.
	RES_TYPE_SHADER = "shader"_resource_type,

	/// gfx::RenderConfig.
	RES_TYPE_RENDER_CONFIG = "render_config"_resource_type,
};

/// Resource names.
enum : ResourceNameHash {
	/// Null name.
	RES_NAME_NULL = ""_resource_name,

	/// Shared shader configuration shader_prelude.
	RES_NAME_SHADER_CONFIG = "togo/gfx/shader-config"_resource_name,
};

/// Combined resource tags.
enum : ResourceTagsHash {
	/// Null tags.
	RES_TAGS_NULL = ""_resource_tags,
};

/// Resource package names.
enum : ResourcePackageNameHash {
	/// Null name.
	PKG_NAME_NULL = ""_resource_package_name,
};

/// Resource path parts.
struct ResourcePathParts {
	struct Tag {
		StringRef name{};
		hash32 hash;
	};

	ResourceType type_hash;
	ResourceNameHash name_hash;
	ResourceTagsHash tags_hash;
	StringRef type{};
	StringRef name{};
	FixedArray<Tag, 8> tags;
};

/// Path to compiled resource.
struct ResourceCompiledPath {
	u32 id;
	FixedArray<char, 24> _data{};

	~ResourceCompiledPath() = default;

	ResourceCompiledPath(ResourceCompiledPath const&) = delete;
	ResourceCompiledPath(ResourceCompiledPath&&) = delete;
	ResourceCompiledPath& operator=(ResourceCompiledPath const&) = delete;
	ResourceCompiledPath& operator=(ResourceCompiledPath&&) = delete;

	ResourceCompiledPath();

	operator StringRef() const;

	u32 size() const;
	char const* data() const;
};

/// Resource metadata.
struct ResourceMetadata {
	u32 id;

	// Serial
	ResourceNameHash name_hash;
	ResourceTagsHash tags_hash;
	ResourceType type;
	u32 data_format_version;
	u32 data_offset;
	u32 data_size;
};

/// Resource value.
union ResourceValue {
	void* pointer;
	u32 uinteger;

	~ResourceValue() = default;
	ResourceValue() = default;
	ResourceValue(ResourceValue const&) = default;
	ResourceValue(ResourceValue&&) = default;
	ResourceValue& operator=(ResourceValue const&) = default;
	ResourceValue& operator=(ResourceValue&&) = default;

	/// Construct with pointer.
	ResourceValue(void* const pointer) : pointer(pointer) {}

	/// Construct with 32-bit unsigned integer.
	ResourceValue(u32 const uinteger) : uinteger(uinteger) {}

	/// Whether pointer is valid.
	bool valid() const {
		return pointer != nullptr;
	}
};

/// Test resource.
struct TestResource {
	s64 x;
};

// Forward declarations
struct ResourceHandler;
struct ResourcePackage;
struct ResourceManager;

/// Resource stream lock.
///
/// This class opens a resource stream from a package on
/// initialization and closes it on deinitialization.
struct ResourceStreamLock {
	ResourcePackage& _package;
	IReader* _stream;

	ResourceStreamLock() = delete;
	ResourceStreamLock(ResourceStreamLock const&) = delete;
	ResourceStreamLock(ResourceStreamLock&&) = delete;
	ResourceStreamLock& operator=(ResourceStreamLock const&) = delete;
	ResourceStreamLock& operator=(ResourceStreamLock&&) = delete;

	/// Close resource stream.
	~ResourceStreamLock();

	/// Open resource stream.
	ResourceStreamLock(
		ResourcePackage& package,
		u32 id
	);

	/// Resource stream.
	IReader& stream();
};

/**
	@addtogroup lib_game_resource_handler
	@{
*/

/// Resource handler.
struct ResourceHandler {
	/// Load a resource.
	///
	/// Returns pointer to resource, or nullptr on error.
	using load_func_type = ResourceValue (
		void* const type_data,
		ResourceManager& manager,
		ResourcePackage& package,
		ResourceMetadata const& metadata
	);

	/// Unload a resource.
	using unload_func_type = void (
		void* const type_data,
		ResourceManager& manager,
		ResourceValue const resource
	);

	ResourceType type;
	u32 format_version;
	void* type_data;
	load_func_type* func_load;
	unload_func_type* func_unload;
};

/** @} */ // end of doc-group lib_game_resource_handler

/**
	@addtogroup lib_game_resource_package
	@{
*/

/// Resource package.
struct ResourcePackage {
	using LookupNode = HashMapNode<ResourceNameHash, u32>;

	ResourcePackageNameHash _name_hash;
	u32 _open_resource_id;
	FileReader _stream;
	HashMap<ResourceNameHash, u32> _lookup;
	Array<ResourceMetadata> _manifest;
	FixedArray<char, 48> _name;
	FixedArray<char, 256> _path;

	ResourcePackage() = delete;
	ResourcePackage(ResourcePackage const&) = delete;
	ResourcePackage(ResourcePackage&&) = delete;
	ResourcePackage& operator=(ResourcePackage const&) = delete;
	ResourcePackage& operator=(ResourcePackage&&) = delete;

	~ResourcePackage() = default;

	ResourcePackage(
		StringRef const& name,
		StringRef const& path,
		Allocator& allocator
	);
};

/** @} */ // end of doc-group lib_game_resource_package

/**
	@addtogroup lib_game_resource_manager
	@{
*/

/// Resource manager.
struct ResourceManager {
	struct TypedResourceValue {
		ResourceValue value;
		ResourceType type;
	};

	using ActiveNode = HashMapNode<ResourceNameHash, TypedResourceValue>;

	HashMap<ResourceType, ResourceHandler> _handlers;
	HashMap<ResourceNameHash, TypedResourceValue> _resources;
	Array<ResourcePackage*> _packages;
	FixedArray<char, 128> _base_path;

	ResourceManager() = delete;
	ResourceManager(ResourceManager const&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager const&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

	~ResourceManager();
	ResourceManager(
		StringRef const base_path,
		Allocator& allocator
	);
};

/** @} */ // end of doc-group lib_game_resource_manager

/** @} */ // end of doc-group lib_game_resource

} // namespace game
} // namespace togo
