#line 2 "togo/game/gfx/display/glfw.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.
*/

#pragma once

#include <togo/game/config.hpp>
#include <togo/game/input/types.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace togo {
namespace gfx {

struct GLFWDisplayImpl {
	GLFWwindow* handle;
};

using DisplayImpl = GLFWDisplayImpl;

} // namespace gfx
} // namespace togo
