#line 2 "togo/debug_constraints.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.

@file debug_constraints.hpp
@brief Definitions for constraints.
*/

#pragma once

#include <togo/config.hpp>

#if defined(TOGO_USE_CONSTRAINTS)
	#include <type_traits>

	#define TOGO_CONSTRAIN_IS_POD(T)								\
		static_assert(												\
			std::is_standard_layout<T>::value,						\
			"T is not a POD type"									\
		)

	#define TOGO_CONSTRAIN_SAME(T, U)								\
		static_assert(												\
			std::is_same<T, U>::value,								\
			"T and U are not the same types"						\
		)

	#define TOGO_CONSTRAIN_INTEGRAL(T)								\
		static_assert(												\
			std::is_integral<T>::value,								\
			"T is not an integral type"								\
		)

	#define TOGO_CONSTRAIN_ARITHMETIC(T)							\
		static_assert(												\
			std::is_arithmetic<T>::value,							\
			"T is not an arithmetic type"							\
		)

	#define TOGO_CONSTRAIN_INTEGRAL_ARITHMETIC(T)					\
		static_assert(												\
			std::is_integral<T>::value &&							\
			std::is_arithmetic<T>::value,							\
			"T is not an integral arithmetic type"					\
		)

	#define TOGO_CONSTRAIN_UNSIGNED(T)								\
		static_assert(												\
			std::is_unsigned<T>::value,								\
			"T is not an unsigned arithmetic type"					\
		)

	#define TOGO_CONSTRAIN_COMPARABLE(T)							\
		static_assert(												\
			std::is_pointer<T>::value ||							\
			(														\
				std::is_integral<T>::value &&						\
				std::is_arithmetic<T>::value						\
			),														\
			"T is not a permitted built-in comparable type"			\
		)
#else
	/// Statically assert that type T is of standard layout.
	#define TOGO_CONSTRAIN_IS_POD(T)

	/// Statically assert that type T is the same as type U.
	#define TOGO_CONSTRAIN_SAME(T, U)

	/// Statically assert that type T is an integral type.
	#define TOGO_CONSTRAIN_INTEGRAL(T)

	/// Statically assert that type T is an arithmetic type.
	#define TOGO_CONSTRAIN_ARITHMETIC(T)

	/// Statically assert that type T is an integral arithmetic type.
	#define TOGO_CONSTRAIN_INTEGRAL_ARITHMETIC(T)

	/// Statically assert that type T is an unsigned arithmetic type.
	#define TOGO_CONSTRAIN_UNSIGNED(T)

	/// Statically assert that type T is a built-in comparable type.
	///
	/// Comparable types are arithmetic integrals and pointers.
	#define TOGO_CONSTRAIN_COMPARABLE(T)
#endif
