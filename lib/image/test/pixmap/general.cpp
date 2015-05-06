
#include <togo/image/config.hpp>
#include <togo/core/error/assert.hpp>
#include <togo/core/utility/utility.hpp>
#include <togo/image/pixmap/pixmap.hpp>

#include <togo/support/test.hpp>

#include <cstring>

using namespace togo;

signed main() {
	memory_init();

	Color c_init{0x01, 0x02, 0x03, 0x04};
	Color c_fill1{0xAA, 0xBB, 0xCC, 0xDD};
	Color c_fill2{0xDD, 0xCC, 0xBB, 0xAA};
	auto cp_init = c_init.packed();
	auto cp_fill1 = c_fill1.packed();
	// auto cp_fill2 = c_fill2.packed();

	Pixmap p{PixelFormatID::rgba8};
	pixmap::resize(p, UVec2{1, 1}, c_init);
	TOGO_ASSERTE(p.data && p.data_size == p.data_capacity && p.data_size == 1*1 * 4);
	auto data = reinterpret_cast<u32*>(p.data);
	TOGO_ASSERTE(data[0] == cp_init);

	pixmap::resize(p, UVec2{4, 4}, c_fill1);
	TOGO_ASSERTE(p.data && p.data_size == p.data_capacity && p.data_size == 4*4 * 4);
	data = reinterpret_cast<u32*>(p.data);
	TOGO_ASSERTE(data[0] == cp_init);
	{auto end = pointer_add(data, p.data_size);
	for (auto check = data + 1; check != end; ++check) {
		TOGO_ASSERTE(*check == cp_fill1);
	}}

	{Pixmap copy{p.format};
	pixmap::copy(copy, p);
	TOGO_ASSERTE(
		copy.data_size == p.data_size &&
		std::memcmp(copy.data, p.data, p.data_size) == 0
	);}

	{Pixmap blit{p.format};
	pixmap::resize(blit, UVec2{4, 4}, c_fill2);
	pixmap::blit(blit, p, UVec2{0, 0});
	TOGO_ASSERTE(
		blit.data_size == p.data_size &&
		std::memcmp(blit.data, p.data, p.data_size) == 0
	);}

	return 0;
}
