#line 2 "togo/core/serialization/fixed_array.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.

@file
@brief FixedArray serialization.
@ingroup lib_core_serialization
*/

#pragma once

#include <togo/core/config.hpp>
#include <togo/core/error/assert.hpp>
#include <togo/core/collection/fixed_array.hpp>
#include <togo/core/serialization/support.hpp>

#include <limits>

namespace togo {

/**
	@addtogroup lib_core_serialization
	@{
*/

/** @cond INTERNAL */

template<class Ser, class S, class T, unsigned N>
inline void
read(serializer_tag, Ser& ser, SerCollection<S, FixedArray<T, N>>&& value) {
	static_assert(
		std::numeric_limits<S>::max() >= N,
		"S is smaller than the fixed capacity of this collection"
	);

	auto& collection = value.ref;
	S size{};
	ser % size;

	// NB: Assertion in resize() will cover invalid sizes
	fixed_array::resize(collection, size);
	ser % make_ser_sequence(
		fixed_array::begin(collection),
		fixed_array::size(collection)
	);
}

template<class Ser, class S, class T, unsigned N, bool C>
inline void
write(serializer_tag, Ser& ser, SerCollection<S, FixedArray<T, N>, C> const& value) {
	static_assert(
		std::numeric_limits<S>::max() >= N,
		"S is smaller than the fixed capacity of this collection"
	);

	auto const& collection = value.ref;
	auto const size = fixed_array::size(collection);
	TOGO_DEBUG_ASSERTE(std::numeric_limits<S>::max() >= size);
	ser
		% static_cast<S>(size)
		% make_ser_sequence(
			fixed_array::begin(collection),
			size
		)
	;
}

/** @endcond */ // INTERNAL

/** @} */ // end of doc-group lib_core_serialization

} // namespace togo
