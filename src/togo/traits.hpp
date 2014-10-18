#line 2 "togo/traits.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.

@file
@brief Type traits.
@ingroup utility
*/

#pragma once

#include <togo/config.hpp>

namespace togo {

/**
	@addtogroup utility
	@{
*/

/** @name Type traits */ /// @{

/// Typed static constexpr integral value.
///
/// T must be an integral type.
template<class T, T V>
struct integral_constant {
	static constexpr T const value = V;
};

/// Boolean constant of false.
using false_type = integral_constant<bool, false>;

/// Boolean constant of true.
using true_type = integral_constant<bool, true>;

namespace {
	template<bool, class, class>
	struct type_if_impl;

	template<class T, class E>
	struct type_if_impl<false, T, E> {
		using type = E;
	};

	template<class T, class E>
	struct type_if_impl<true, T, E> {
		using type = T;
	};
} // anonymous namespace

/// Type alias to T if C is true, or E if C is false.
template<bool C, class T, class E>
using type_if = typename type_if_impl<C, T, E>::type;

/** @cond INTERNAL */
namespace {
	template<bool, class>
	struct enable_if_impl;

	template<class T>
	struct enable_if_impl<false, T> {};

	template<class T>
	struct enable_if_impl<true, T> {
		using type = T;
	};
} // anonymous namespace
/** @endcond */ // INTERNAL

/// SFINAE enabler type alias.
template<bool C, class T = void>
using enable_if = typename enable_if_impl<C, T>::type;

namespace {

template<class T>
struct remove_ref_impl {
	using type = T;
};

template<class T>
struct remove_ref_impl<T&> {
	using type = T;
};

} // anonymous namespace

/// Remove reference qualification from type.
template<class T>
using remove_ref = typename remove_ref_impl<T>::type;

namespace {

template<class T>
struct remove_cv_impl {
	using type = T;
};

template<class T>
struct remove_cv_impl<T const> {
	using type = T;
};

template<class T>
struct remove_cv_impl<T volatile> {
	using type = T;
};

template<class T>
struct remove_cv_impl<T const volatile> {
	using type = T;
};

} // anonymous namespace

/// Remove const, volatile, and reference qualifications from type.
template<class T>
using remove_cv = typename remove_cv_impl<T>::type;

/// Remove const, volatile, and reference qualifications from type.
template<class T>
using remove_cvr = typename remove_cv<remove_ref<T>>::type;

namespace {

template<class T>
struct is_signed_impl : false_type {};

#define IMPL_SPECIALIZE(T) \
	template<> struct is_signed_impl<T> : true_type {} //

IMPL_SPECIALIZE(signed char);
IMPL_SPECIALIZE(signed short);
IMPL_SPECIALIZE(signed int);
IMPL_SPECIALIZE(signed long);
IMPL_SPECIALIZE(signed long long);

#undef IMPL_SPECIALIZE

} // anonymous namespace

/// Boolean constant of true if T is a signed integral.
template<class T>
using is_signed = is_signed_impl<remove_cv<T>>;

namespace {

template<class T>
struct is_unsigned_impl : false_type {};

#define IMPL_SPECIALIZE(T) \
	template<> struct is_unsigned_impl<T> : true_type {} //

IMPL_SPECIALIZE(unsigned char);
IMPL_SPECIALIZE(unsigned short);
IMPL_SPECIALIZE(unsigned int);
IMPL_SPECIALIZE(unsigned long);
IMPL_SPECIALIZE(unsigned long long);

#undef IMPL_SPECIALIZE

} // anonymous namespace

/// Boolean constant of true if T is an unsigned integral.
template<class T>
using is_unsigned = is_unsigned_impl<remove_cv<T>>;

namespace {

template<class T, class U>
struct is_same_impl : false_type {};

template<class T>
struct is_same_impl<T, T> : true_type {};

} // anonymous namespace

/// Boolean constant of true if T is the same as U.
template<class T, class U>
using is_same = is_same_impl<T, U>;

/// @}

/// Enum-class bitwise operator enabler.
///
/// Specialize this class deriving from true_type to enable bit-wise
/// operators for an enum-class.
template<class>
struct enable_enum_bitwise_ops : false_type {};

/** @} */ // end of doc-group utility

} // namespace togo
