#line 2 "togo/input/input.cpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.
*/

#include <togo/config.hpp>
#include <togo/error/assert.hpp>
#include <togo/gfx/display/types.hpp>
#include <togo/input/input.hpp>

namespace togo {

/// Check if a key has been pressed for a display.
bool input::key_pressed(gfx::Display* display, KeyCode const code) {
	return display->_key_states[static_cast<unsigned>(code)];
}

/// Check if a key has been released for a display.
bool input::key_released(gfx::Display* display, KeyCode const code) {
	return display->_key_states[
		static_cast<unsigned>(code)
	] & (1 << static_cast<unsigned>(KeyAction::release));
}

/// Check if a mouse button has been pressed for a display.
bool input::mouse_button_pressed(gfx::Display* display, MouseButton const button) {
	return display->_mouse_button_states[static_cast<unsigned>(button)];
}

/// Check if a mouse button has been released for a display.
bool input::mouse_button_released(gfx::Display* display, MouseButton const button) {
	return display->_mouse_button_states[
		static_cast<unsigned>(button)
	] & (1 << static_cast<unsigned>(MouseButtonAction::release));
}

/// Get mouse position for a display.
Vec2 input::mouse_position(gfx::Display* display) {
	return Vec2{
		static_cast<float>(display->_mouse_x),
		static_cast<float>(display->_mouse_y)
	};
}

} // namespace togo
