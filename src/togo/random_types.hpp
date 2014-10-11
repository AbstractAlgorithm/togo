#line 2 "togo/random_types.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.

@file
@brief Random types.
@ingroup types
@ingroup random
*/

#pragma once

#include <togo/config.hpp>
#include <togo/types.hpp>
#include <togo/debug_constraints.hpp>

namespace togo {

/**
	@addtogroup random
	@{
*/

enum : u64 {
	/// Minimum value generated by RNGs.
	RNG_MIN = 0,
	/// Maximum value generated by RNGs.
	RNG_MAX = ~u64{0u},
};

enum : s64 {
	// Minimum value generated by RNGs (signed).
	RNG_MIN_S = 0,
	// Maximum value generated by RNGs (signed).
	RNG_MAX_S = static_cast<s64>((~u64{0u} << 1) >> 1),
};

/**
	xorshift64* state.
*/
struct XS64M {
	u64 _v;

	~XS64M() = default;
	XS64M(XS64M&&) = default;
	XS64M(XS64M const&) = default;
	XS64M& operator=(XS64M&&) = default;
	XS64M& operator=(XS64M const&) = default;

	XS64M() = delete;

	XS64M(u64 const seed);
};

/**
	xorshift128+ state.
*/
struct XS128A {
	u64 _v[2];

	~XS128A() = default;
	XS128A(XS128A&&) = default;
	XS128A(XS128A const&) = default;
	XS128A& operator=(XS128A&&) = default;
	XS128A& operator=(XS128A const&) = default;

	XS128A() = delete;

	XS128A(u64 const seed);
};

/**
	xorshift1024* state.
*/
struct XS1024M {
	u64 _v[16];
	unsigned _i;

	~XS1024M() = default;
	XS1024M(XS1024M&&) = default;
	XS1024M(XS1024M const&) = default;
	XS1024M& operator=(XS1024M&&) = default;
	XS1024M& operator=(XS1024M const&) = default;

	XS1024M() = delete;

	XS1024M(u64 const seed);
};

/**
	Uniform integer distribution [min, max].
*/
template<class T>
struct IntUDist {
	TOGO_CONSTRAIN_INTEGRAL_ARITHMETIC(T);

	using value_type = T;

	value_type _base;
	u64 _range;
	u64 _ratio;
	u64 _bound;

	~IntUDist() = default;
	IntUDist(IntUDist&&) = default;
	IntUDist(IntUDist const&) = default;
	IntUDist& operator=(IntUDist&&) = default;
	IntUDist& operator=(IntUDist const&) = default;

	IntUDist() = delete;

	constexpr IntUDist(value_type const min, value_type const max);
};

/**
	Uniform real distribution [min, max].
*/
template<class T>
struct RealUDist {
	TOGO_CONSTRAIN_FLOATING_POINT(T);

	using value_type = T;

	value_type _base;
	value_type _range;

	~RealUDist() = default;
	RealUDist(RealUDist&&) = default;
	RealUDist(RealUDist const&) = default;
	RealUDist& operator=(RealUDist&&) = default;
	RealUDist& operator=(RealUDist const&) = default;

	RealUDist() = delete;

	constexpr RealUDist(value_type const min, value_type const max);
};

/** @} */ // end of doc-group random

} // namespace togo
