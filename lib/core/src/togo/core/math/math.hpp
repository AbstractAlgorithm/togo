#line 2 "togo/core/math/math.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.

@file
@brief Math interface.
@ingroup lib_core_math
*/

#pragma once

#include <togo/core/config.hpp>
#include <togo/core/math/types.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#include <am/linear/vector_interface.hpp>
#include <am/linear/vector_operations.hpp>
#include <am/linear/matrix_interface.hpp>
#include <am/linear/matrix_operations.hpp>
#pragma GCC diagnostic pop

namespace togo {

/**
	@addtogroup lib_core_math
	@{
*/

using am::linear::length;
using am::linear::distance;
using am::linear::dot;
using am::linear::cross;
using am::linear::normalize;
using am::linear::faceforward;
using am::linear::reflect;
using am::linear::refract;

using am::linear::transpose;
using am::linear::determinant;
using am::linear::inverse;

/** Mathematical constant π (pi). */
static constexpr float const MC_PI = 3.1415926535897932384626f;
/** Mathematical constant π/2 (pi/2). */
static constexpr float const MC_PI_HALF = 1.5707963267948966192313f;
/** Mathematical constant τ (tau, 2 * MC_PI). */
static constexpr float const MC_TAU = 6.2831853071795864769252f;

/** @} */ // end of doc-group lib_core_math

} // namespace togo
