#line 2 "togo/game/gfx/generator/gen_clear.cpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.
*/

#include <togo/game/config.hpp>
#include <togo/core/error/assert.hpp>
#include <togo/core/hash/hash.hpp>
#include <togo/core/serialization/serializer.hpp>
#include <togo/core/serialization/support.hpp>
#include <togo/core/serialization/binary_serializer.hpp>
#include <togo/game/gfx/types.hpp>
#include <togo/game/gfx/generator.hpp>
#include <togo/game/gfx/render_node.hpp>
#include <togo/game/gfx/renderer.hpp>

namespace togo {
namespace game {
namespace gfx {

namespace generator {
namespace gen_clear {

union Data {
	void* ptr;
	u32 rt_index;
};

static void exec(
	gfx::GeneratorUnit const& unit,
	gfx::RenderNode& node,
	gfx::RenderObject const* const /*objects_begin*/,
	gfx::RenderObject const* const /*objects_end*/
) {
	// TODO: Command for clearing any RT instead?
	// TODO: Clear all RTs in layer?
	Data const data{unit.data};
	if (data.rt_index == ~u32{0}) {
		gfx::render_node::push(node, 0, gfx::CmdClearBackbuffer{});
	} else {
		TOGO_ASSERT(false, "clearing other than the back-buffer is not implemented");
	}
}

static void read(
	gfx::GeneratorDef const& /*def*/,
	gfx::Renderer* /*renderer*/,
	BinaryInputSerializer& ser,
	gfx::GeneratorUnit& unit
) {
	Data data;
	ser % data.rt_index;
	unit.data = data.ptr;
	unit.func_exec = exec;
}

} // namespace gen_clear
} // namespace generator

gfx::GeneratorDef const generator::clear{
	"clear"_generator_name,
	nullptr,
	nullptr,
	nullptr,
	generator::gen_clear::read
};

} // namespace gfx
} // namespace game
} // namespace togo
