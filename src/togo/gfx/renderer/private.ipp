#line 2 "togo/gfx/renderer/private.ipp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.
*/

#pragma once

#include <togo/config.hpp>
#include <togo/gfx/types.hpp>
#include <togo/gfx/renderer/types.hpp>
#include <togo/gfx/renderer/private.hpp>

namespace togo {
namespace gfx {

#define TOGO_GFX_RENDERER_TEARDOWN_RA_(ra, func_destroy)		\
	if (renderer->ra._num > 0) {								\
		for (auto const& slot : renderer->ra._slots) {			\
			if (slot.id._value != ID_VALUE_NULL) {				\
				renderer::func_destroy(renderer, slot.id);		\
			}													\
		}														\
	}

void renderer::teardown_base(gfx::Renderer* const renderer) {
	TOGO_GFX_RENDERER_TEARDOWN_RA_(_vertex_buffers, destroy_vertex_buffer);
	TOGO_GFX_RENDERER_TEARDOWN_RA_(_index_buffers, destroy_index_buffer);
	TOGO_GFX_RENDERER_TEARDOWN_RA_(_buffer_bindings, destroy_buffer_binding);
	//TOGO_GFX_RENDERER_TEARDOWN_RA_(_textures, destroy_texture);
	//TOGO_GFX_RENDERER_TEARDOWN_RA_(_uniforms, destroy_uniform);
	//TOGO_GFX_RENDERER_TEARDOWN_RA_(_shaders, destroy_shader);
}

} // namespace gfx
} // namespace togo
