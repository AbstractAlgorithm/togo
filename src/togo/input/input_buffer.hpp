#line 2 "togo/input/input_buffer.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.

@file
@brief InputBuffer interface.
@ingroup input
@ingroup input_buffer
*/

#pragma once

#include <togo/config.hpp>
#include <togo/io/object_buffer.hpp>
#include <togo/gfx/types.hpp>
#include <togo/input/types.hpp>
#include <togo/input/input_buffer.gen_interface>

namespace togo {

/// Construct with allocator for storage.
inline InputBuffer::InputBuffer(
	Allocator& allocator,
	u32 const init_capacity
)
	: _num_displays(0)
	, _displays()
	, _buffer(allocator, init_capacity)
{}

namespace input_buffer {

/**
	@addtogroup input_buffer
	@{
*/

/// Number of displays.
inline unsigned num_displays(InputBuffer& ib) {
	return ib._num_displays;
}

/** @} */ // end of doc-group input_buffer

} // namespace input_buffer
} // namespace togo
