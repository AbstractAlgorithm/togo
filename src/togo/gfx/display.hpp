#line 2 "togo/gfx/display.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.

@file gfx/display.hpp
@brief Graphics display interface.
@ingroup gfx
@ingroup gfx_display
*/

#pragma once

#include <togo/config.hpp>
#include <togo/utility.hpp>
#include <togo/gfx/types.hpp>
#include <togo/memory.hpp>

namespace togo {
namespace gfx {
namespace display {

/**
	@addtogroup gfx
	@{
*/
/**
	@addtogroup gfx_display
	@{
*/

/// Create graphics display.
///
/// An assertion will fail if the display could not be created.
/// If gfx::DisplayFlags::fullscreen is enabled, width and height are
/// ignored.
gfx::Display* create(
	char const* title,
	unsigned width,
	unsigned height,
	gfx::DisplayFlags flags,
	gfx::Config const& config,
	Allocator& allocator = memory::default_allocator()
);

/// Set title.
void set_title(gfx::Display* display, char const* title);

/// Set mouse input lock.
///
/// If this is enabled, the mouse position will be locked to the
/// display window.
void set_mouse_lock(gfx::Display* display, bool enable);

/// Make display the current target for a graphics context.
void make_current(gfx::Display* display, gfx::Context* context);

/// Swap buffers in display.
///
/// This does nothing if the display is not double-buffered.
void swap_buffers(gfx::Display* display);

/// Destroy graphics display.
void destroy(gfx::Display* display);

/** @} */ // end of doc-group gfx_display
/** @} */ // end of doc-group gfx

} // namespace display
} // namespace gfx
} // namespace togo
