#line 2 "togo/game/serialization/resource/resource_metadata.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.

@file
@brief ResourceMetadata serialization.
@ingroup serialization
*/

#pragma once

#include <togo/game/config.hpp>
#include <togo/core/serialization/support.hpp>
#include <togo/game/resource/types.hpp>

namespace togo {

/**
	@addtogroup serialization
	@{
*/

/** @cond INTERNAL */

template<class Ser>
inline void
serialize(serializer_tag, Ser& ser, ResourceMetadata& value_unsafe) {
	auto& value = serializer_cast_safe<Ser>(value_unsafe);
	ser
		% value.name_hash
		% value.tags_hash
		% value.type
		% value.data_format_version
		% value.data_offset
		% value.data_size
	;
}

/** @endcond */ // INTERNAL

/** @} */ // end of doc-group serialization

} // namespace togo