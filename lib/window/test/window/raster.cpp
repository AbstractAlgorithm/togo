
#include <togo/core/error/assert.hpp>
#include <togo/core/math/types.hpp>
#include <togo/core/log/log.hpp>
#include <togo/core/string/string.hpp>
#include <togo/core/system/system.hpp>
#include <togo/image/pixmap/pixmap.hpp>
#include <togo/window/window/window.hpp>
#include <togo/window/input/input.hpp>
#include <togo/window/input/input_buffer.hpp>

#include <togo/support/test.hpp>

using namespace togo;

signed main() {
	memory_init();

#if (TOGO_CONFIG_WINDOW_BACKEND == TOGO_WINDOW_BACKEND_RASTER)
	window::init(0, 0);

	Window* const window = window::create_raster(
		"togo window",
		UVec2{1024, 768},
		WindowFlags::borderless |
		WindowFlags::resizable
	);
	InputBuffer ib{memory::default_allocator()};
	input_buffer::add_window(ib, window);

	bool quit = false;
	bool mouse_lock = false;
	window::set_mouse_lock(window, mouse_lock);

	UVec4 area{0, 0, 256, 256};
	pixmap::fill(window::backbuffer(window), {0, 255, 0}, area);
	window::push_backbuffer(window, array_ref(1, &area));

	InputEventType event_type{};
	InputEvent const* event = nullptr;
	while (!quit) {
		input_buffer::update(ib);
		while (input_buffer::poll(ib, event_type, event)) {
			TOGO_ASSERTE(event->window == window);
			if (event_type == InputEventType::window_close_request) {
				quit = true;
			}
		}
		if (input::key_released(window, KeyCode::escape)) {
			quit = true;
		}
		if (input::key_released(window, KeyCode::f1)) {
			mouse_lock = !mouse_lock;
			window::set_mouse_lock(window, mouse_lock);
			TOGO_LOG("mouse lock toggled\n");
		}
		system::sleep_ms(50);
	}
	input_buffer::remove_window(ib, window);
	window::destroy(window);
	window::shutdown();
#else
	TOGO_LOG("raster window not supported by backend\n");
#endif

	return 0;
}
