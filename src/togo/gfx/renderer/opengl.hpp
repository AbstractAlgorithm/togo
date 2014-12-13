#line 2 "togo/gfx/renderer/opengl.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.
*/

#pragma once

#include <togo/config.hpp>
#include <togo/utility/utility.hpp>
#include <togo/log/log.hpp>
#include <togo/gfx/types.hpp>

#include <GL/glew.h>

namespace togo {
namespace gfx {

char const* gl_get_error();

#define TOGO_GLCE() \
	do { while (char const* gl_error__ = gfx::gl_get_error()) { \
		TOGO_LOG_ERRORF("OpenGL error: %s\n", gl_error__); \
	} } while (false)

#define TOGO_GLCE_X(expr_) do { (expr_); TOGO_GLCE(); } while (false)

enum : GLuint {
	BUFFER_HANDLE_NULL = 0,
	VERTEX_ARRAY_HANDLE_NULL = 0,
	TEXTURE_HANDLE_NULL = 0,
	PROGRAM_HANDLE_NULL = 0,
};

enum : GLint {
	UNIFORM_HANDLE_NULL = -1,
};

static constexpr GLenum const g_gl_buffer_data_binding[]{
	// fixed
	GL_STATIC_DRAW,
	// dynamic
	GL_DYNAMIC_DRAW,
};
static_assert(
	array_extent(g_gl_buffer_data_binding)
	== unsigned_cast(gfx::BufferDataBinding::NUM),
	""
);

static constexpr GLenum const g_gl_vertex_attrib_type[]{
	GL_FLOAT, GL_DOUBLE,
	GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT,
};
static_assert(
	array_extent(g_gl_vertex_attrib_type)
	== unsigned_cast(gfx::VertexAttribType::NUM),
	""
);

static constexpr GLenum const g_gl_index_data_type[]{
	GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT,
};
static_assert(
	array_extent(g_gl_index_data_type)
	== unsigned_cast(gfx::IndexDataType::NUM),
	""
);

static constexpr unsigned const g_gl_index_data_size[]{
	1, 2, 4
};
static_assert(
	array_extent(g_gl_index_data_size)
	== unsigned_cast(gfx::IndexDataType::NUM),
	""
);

static constexpr GLenum const g_gl_shader_stage_type[]{
	GL_VERTEX_SHADER,
	GL_FRAGMENT_SHADER,
};
static_assert(
	array_extent(g_gl_shader_stage_type)
	== unsigned_cast(gfx::ShaderStage::Type::NUM),
	""
);

enum class GLBufferFlags : unsigned {
	none = 0,
	dynamic = 1 << 0,
};

struct Buffer {
	gfx::BufferID id;
	GLuint handle;
	gfx::GLBufferFlags flags;
};

struct BufferBinding {
	enum : u32 {
		F_NONE = 0,
		F_INDEXED = 1 << 0,

		// 3 bits (IndexDataType currently takes 2 at most)
		F_SHIFT_INDEX_TYPE = 29,
	};

	gfx::BufferBindingID id;
	GLuint va_handle;
	u32 base_vertex;
	u32 num_vertices;
	u32 flags;
};

struct Texture {
	gfx::TextureID id;
	GLuint handle;
};

struct Uniform {
	gfx::UniformID id;
	GLint handle;
};

struct Shader {
	gfx::ShaderID id;
	GLuint handle;
};

struct OpenGLRendererImpl {};

using RendererImpl = OpenGLRendererImpl;

} // namespace gfx
} // namespace togo
