#line 2 "togo/resource/resource_handler/test_resource.cpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.
*/

#include <togo/config.hpp>
#include <togo/memory/memory.hpp>
#include <togo/resource/types.hpp>
#include <togo/resource/resource.hpp>
#include <togo/resource/resource_handler.hpp>
#include <togo/resource/resource_manager.hpp>
#include <togo/serialization/serializer.hpp>
#include <togo/serialization/resource/test_resource.hpp>
#include <togo/serialization/binary_serializer.hpp>

namespace togo {

namespace resource_handler {
namespace test_resource {

static ResourceValue load(
	void* const type_data,
	ResourceManager& /*manager*/,
	ResourcePackage& package,
	ResourceMetadata const& metadata
) {
	TOGO_DEBUG_ASSERTE(type_data == nullptr);
	TestResource* const test_resource = TOGO_CONSTRUCT(
		memory::default_allocator(), TestResource, {0}
	);

	{// Deserialize resource
	ResourceStreamLock lock{package, metadata.id};
	BinaryInputSerializer ser{lock.stream()};
	ser % *test_resource;
	}
	return test_resource;
}

static void unload(
	void* const type_data,
	ResourceManager& /*manager*/,
	ResourceValue const resource
) {
	TOGO_DEBUG_ASSERTE(type_data == nullptr);
	auto const* test_resource = static_cast<TestResource*>(resource.pointer);
	TOGO_DESTROY(memory::default_allocator(), test_resource);
}

} // namespace test_resource
} // namespace resource_handler

void resource_handler::register_test(
	ResourceManager& rm
) {
	ResourceHandler const handler{
		RES_TYPE_TEST,
		SER_FORMAT_VERSION_TEST_RESOURCE,
		nullptr,
		resource_handler::test_resource::load,
		resource_handler::test_resource::unload
	};
	resource_manager::register_handler(rm, handler);
}

} // namespace togo
