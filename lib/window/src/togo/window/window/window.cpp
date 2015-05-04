#line 2 "togo/window/window/window.cpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.
*/

#include <togo/window/config.hpp>
#include <togo/core/error/assert.hpp>
#include <togo/core/math/types.hpp>
#include <togo/window/window/types.hpp>
#include <togo/window/window/window.hpp>
#include <togo/window/window/impl/types.hpp>
#include <togo/window/window/impl/private.hpp>
#include <togo/window/input/input_buffer.hpp>

#if defined(TOGO_CONFIG_WINDOW_ENABLE_OPENGL)
	#include <togo/window/window/impl/opengl.ipp>
#endif

#if (TOGO_CONFIG_WINDOW_BACKEND == TOGO_WINDOW_BACKEND_SDL)
	#include <togo/window/window/impl/sdl.ipp>
#elif (TOGO_CONFIG_WINDOW_BACKEND == TOGO_WINDOW_BACKEND_GLFW)
	#include <togo/window/window/impl/glfw.ipp>
#endif

namespace togo {

namespace window {

Globals _globals{false, false, false, 0, 0};

} // namespace window

/// Initialize the window backend.
///
/// context_major and context_minor are OpenGL context version numbers.
/// If these are 0 and 0, OpenGL is not initialized, and OpenGL contexts cannot
/// be created.
/// If these are otherwise less than 3 and 3, an assertion will fail. The core
/// profile is forced.
void window::init(
	unsigned context_major,
	unsigned context_minor
) {
	TOGO_ASSERT(!_globals.initialized, "window backend has already been initialized");

	_globals.with_gl = !(context_major == 0 && context_minor == 0);
	TOGO_ASSERT(
		!_globals.with_gl || (context_major >= 3 && context_minor >= 3),
		"OpenGL context version below 3.3 is not supported"
	);

	_globals.context_major = context_major;
	_globals.context_minor = context_minor;
	window::init_impl();

	_globals.initialized = true;
	return;
}

/// Shutdown the window backend.
void window::shutdown() {
	TOGO_ASSERT(_globals.initialized, "window backend has not been initialized");

	shutdown_impl();
	_globals.context_major = 0;
	_globals.context_minor = 0;
	_globals.initialized = false;
}

Window::~Window() {
	if (window::is_attached_to_input_buffer(this)) {
		input_buffer::remove_window(*_input_buffer, this);
	}
}

/// Size.
UVec2 window::size(Window const* window) {
	return window->_size;
}

/// Width.
unsigned window::width(Window const* window) {
	return window->_size.x;
}

/// Height.
unsigned window::height(Window const* window) {
	return window->_size.y;
}

#if !defined(TOGO_CONFIG_WINDOW_ENABLE_OPENGL)
Window* window::create_opengl(
	StringRef /*title*/,
	UVec2 /*size*/,
	WindowFlags /*flags*/,
	WindowOpenGLConfig const& /*config*/,
	Window* /*share_with*/,
	Allocator& /*allocator*/
) {
	TOGO_ASSERT(false, "OpenGL disabled in lib/window");
}

void window::bind_context(Window* /*window*/) {
	TOGO_ASSERT(false, "OpenGL disabled in lib/window");
}

void window::unbind_context() {
	TOGO_ASSERT(false, "OpenGL disabled in lib/window");
}

void window::swap_buffers(Window* /*window*/) {
	TOGO_ASSERT(false, "OpenGL disabled in lib/window");
}
#endif

// private

bool window::is_attached_to_input_buffer(Window const* window) {
	return window->_input_buffer;
}

void window::attach_to_input_buffer(Window* window, InputBuffer& ib) {
	TOGO_ASSERT(
		!window::is_attached_to_input_buffer(window),
		"window is already attached to an input buffer"
	);
	window->_input_buffer = &ib;
	attach_to_input_buffer_impl(window);
}

void window::detach_from_input_buffer(Window* window) {
	TOGO_ASSERT(
		window::is_attached_to_input_buffer(window),
		"window is not attached to an input buffer"
	);
	detach_from_input_buffer_impl(window);
	window->_input_buffer = nullptr;
}

} // namespace togo
