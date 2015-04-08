
#include <togo/core/log/log.hpp>
#include <togo/core/memory/memory.hpp>
#include <togo/core/memory/temp_allocator.hpp>

#include <togo/support/test.hpp>

using namespace togo;

void f() {
	{
		TempAllocator<16> a;
	}

	TOGO_LOG("\n");
	{
		TempAllocator<32> a;
		void* p = a.allocate(24);
		p = a.allocate(1);
	}

	TOGO_LOG("\n");
	{
		TempAllocator<16> a;
		a.allocate(8);
		a.allocate(4087, 1);
		a.allocate(4087, 1);
		a.allocate(4087, 1);
		a.allocate(4087, 1);
		a.allocate(4087, 1);
	}
}

signed main() {
	memory_init();
	f();
	return 0;
}
