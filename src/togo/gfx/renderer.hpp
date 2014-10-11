#line 2 "togo/gfx/renderer.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.

@file
@brief Renderer interface.
@ingroup gfx
@ingroup gfx_renderer
*/

#pragma once

#include <togo/config.hpp>
#include <togo/utility.hpp>
#include <togo/gfx/types.hpp>
#include <togo/memory.hpp>

namespace togo {
namespace gfx {
namespace renderer {

/**
	@addtogroup gfx_renderer
	@{
*/

/// Create renderer.
///
/// An assertion will fail if the renderer could not be created.
gfx::Renderer* create(
	Allocator& allocator = memory::default_allocator()
);

/// Destroy renderer.
void destroy(gfx::Renderer* renderer);

/// Create vertex buffer.
///
/// An assertion will fail if the vertex buffer could not be created.
gfx::VertexBufferID create_vertex_buffer(
	gfx::Renderer* renderer,
	void const* data,
	unsigned size,
	gfx::VertexFormat const& format,
	gfx::BufferDataBinding data_binding = gfx::BufferDataBinding::fixed
);

/// Destroy vertex buffer.
void destroy_vertex_buffer(
	gfx::Renderer* renderer,
	gfx::VertexBufferID id
);

/// Create index buffer.
///
/// An assertion will fail if the index buffer could not be created.
gfx::IndexBufferID create_index_buffer(
	gfx::Renderer* renderer,
	void const* data,
	unsigned size,
	gfx::BufferDataBinding data_binding = gfx::BufferDataBinding::fixed
);

/// Destroy index buffer.
void destroy_index_buffer(
	gfx::Renderer* renderer,
	gfx::IndexBufferID id
);

/** @} */ // end of doc-group gfx_renderer

} // namespace renderer
} // namespace gfx
} // namespace togo
