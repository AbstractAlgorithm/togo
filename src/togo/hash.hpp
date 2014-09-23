#line 2 "togo/hash.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.

@file hash.hpp
@brief Hashing utilities.
@ingroup hash
*/

#pragma once

#include <togo/config.hpp>
#include <togo/debug_constraints.hpp>
#include <togo/types.hpp>
#include <togo/string.hpp>

#include <am/hash/fnv.hpp>

namespace togo {
namespace hash {

/**
	@addtogroup hash
	@{
*/

// TODO: Build constexpr version of MurmurHash2 (both lengths) and
// use it instead of FNV-1a

/// 32-bit hash identity (hash of nothing).
static constexpr hash32 const
IDENTITY32{0};

/// 64-bit hash identity (hash of nothing).
static constexpr hash64 const
IDENTITY64{0};

namespace {
	static constexpr am::hash::HashLength const
	hash32_length = am::hash::HashLength::HL32,
	hash64_length = am::hash::HashLength::HL64;

	template<class H>
	struct hash_type_length;

	template<> struct hash_type_length<hash32> {
		static constexpr auto const value = hash32_length;
	};

	template<> struct hash_type_length<hash64> {
		static constexpr auto const value = hash64_length;
	};

	template<class H>
	struct hash_identity;

	template<> struct hash_identity<hash32> {
		static constexpr auto const value = hash::IDENTITY32;
	};

	template<> struct hash_identity<hash64> {
		static constexpr auto const value = hash::IDENTITY64;
	};
} // anonymous namespace

/** @cond INTERNAL */
TOGO_CONSTRAIN_SAME(hash32, am::detail::hash::fnv_hash_type<hash32_length>);
TOGO_CONSTRAIN_SAME(hash64, am::detail::hash::fnv_hash_type<hash64_length>);
/** @endcond */

/**
	Calculate H-bit hash.
*/
template<class H>
inline H calc_generic(
	char const* const data,
	unsigned const size
) {
	return
		size != 0
		? am::hash::fnv1a<hash_type_length<H>::value>(data, size)
		: hash_identity<H>::value
	;
}

/**
	Calculate H-bit hash from string reference.
*/
template<class H>
inline H calc_generic(StringRef const& ref) {
	return hash::calc_generic<H>(ref.data, ref.size);
}

/**
	Calculate 32-bit hash.
*/
inline hash32 calc32(
	char const* const data,
	unsigned const size
) {
	return hash::calc_generic<hash32>(data, size);
}

/**
	Calculate 32-bit hash from string reference.
*/
inline hash32 calc32(StringRef const& ref) {
	return hash::calc_generic<hash32>(ref.data, ref.size);
}

/**
	Calculate 64-bit hash.
*/
inline hash64 calc64(
	char const* const data,
	unsigned const size
) {
	return hash::calc_generic<hash64>(data, size);
}

/**
	Calculate 64-bit hash from string reference.
*/
inline hash64 calc64(StringRef const& ref) {
	return hash::calc_generic<hash64>(ref.data, ref.size);
}

/** @} */ // end of doc-group hash

} // namespace hash

/**
	32-bit hash literal.
*/
inline constexpr hash32
operator"" _hash32(
	char const* const data,
	std::size_t const size
) {
	return
		size != 0
		? am::hash::fnv1a_c<hash::hash32_length>(data, size)
		: hash::IDENTITY32
	;
}

/**
	64-bit hash literal.
*/
inline constexpr hash64
operator"" _hash64(
	char const* const data,
	std::size_t const size
) {
	return
		size != 0
		? am::hash::fnv1a_c<hash::hash64_length>(data, size)
		: hash::IDENTITY64
	;
}

} // namespace togo
