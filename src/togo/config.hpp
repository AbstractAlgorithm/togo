#line 2 "togo/config.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.

@file
@brief Core configuration.
@ingroup config
*/

#pragma once

// AM‽ configuration
#include <am/config_values.hpp>

/** @cond INTERNAL */
#ifdef AM_CONFIG_IMPLICIT_LINEAR_INTERFACE
	#undef AM_CONFIG_IMPLICIT_LINEAR_INTERFACE
#endif
#define AM_CONFIG_FLOAT_PRECISION AM_PRECISION_MEDIUM
#define AM_CONFIG_VECTOR_TYPES AM_FLAG_TYPE_FLOAT
#define AM_CONFIG_MATRIX_TYPES AM_FLAG_TYPE_FLOAT
/** @endcond */ // INTERNAL

#include <am/config.hpp>

namespace togo {

/**
	@addtogroup config
	@{
*/

#if defined(DOXYGEN_CONSISTS_SOLELY_OF_UNICORNS_AND_CONFETTI)
	/// Whether to use assertions
	///
	/// This is enabled if it is defined.
	#define TOGO_DISABLE_ASSERTIONS

	/// Whether to use assertions
	///
	/// This is enabled if it is defined. It is automatically enabled
	/// if DEBUG is defined or if NDEBUG is not defined.
	#define TOGO_DEBUG

	/// Whether to use stdlib constraints to ensure types are being
	/// used properly.
	///
	/// This is enabled if it is defined.
	#define TOGO_USE_CONSTRAINTS
#else
	#if !defined(TOGO_DEBUG) && (defined(DEBUG) || !defined(NDEBUG))
		#define TOGO_DEBUG
	#endif
#endif // defined(DOXYGEN_CONSISTS_SOLELY_OF_UNICORNS_AND_CONFETTI)

/** @name System detection */ /// @{

#if defined(DOXYGEN_CONSISTS_SOLELY_OF_UNICORNS_AND_CONFETTI)
	/// Defined if the target platform is Linux.
	#define TOGO_PLATFORM_LINUX
	/// Defined if the target platform is MacOS.
	#define TOGO_PLATFORM_MACOS
	/// Defined if the target platform is Windows.
	#define TOGO_PLATFORM_WINDOWS

	/// Defined if the target platform is POSIX-compliant.
	///
	/// This is defined if #TOGO_PLATFORM_LINUX or #TOGO_PLATFORM_MACOS
	/// are defined.
	#define TOGO_PLATFORM_IS_POSIX

	/// Defined if the target processor is x86.
	#define TOGO_ARCH_X86
	/// Defined if the target processor is x86_64.
	#define TOGO_ARCH_X86_64
#else
	#if defined(__linux__)
		#define TOGO_PLATFORM_LINUX
	#elif defined(macintosh) || defined(Macintosh) || \\
		(defined(__APPLE__) && defined(__MACH__))

		#define TOGO_PLATFORM_MACOS
	#elif defined(_WIN32)
		#define TOGO_PLATFORM_WINDOWS
	#else
		#error "unrecognized platform"
	#endif

	#if defined(TOGO_PLATFORM_LINUX) || defined(TOGO_PLATFORM_MACOS)
		#define TOGO_PLATFORM_IS_POSIX
	#endif

	#if defined(__amd64__) || defined(__x86_64__) || \
		defined(_M_AMD64) || defined(_M_X64)

		#define TOGO_ARCH_X86_64
	#elif defined(__i386__) || defined(_M_IX86)
		#define TOGO_ARCH_X86
	#else
		#error "unrecognized processor architecture"
	#endif
#endif // defined(DOXYGEN_CONSISTS_SOLELY_OF_UNICORNS_AND_CONFETTI)

/// @} // end of name-group System detection

/** @name Compiler detection */ /// @{

#if defined(DOXYGEN_CONSISTS_SOLELY_OF_UNICORNS_AND_CONFETTI)
	/// Defined if the compiler is Clang.
	#define TOGO_COMPILER_CLANG
	/// Defined if the compiler is GCC.
	#define TOGO_COMPILER_GCC
	/// Defined if the compiler is MSVC.
	#define TOGO_COMPILER_MSVC
#else
	#if defined(__clang__)
		#define TOGO_COMPILER_CLANG
	#elif defined(__GNUC__)
		#define TOGO_COMPILER_GCC
	#elif defined(_MSC_VER)
		#define TOGO_COMPILER_MSVC
	#else
		#error "unrecognized compiler"
	#endif
#endif // defined(DOXYGEN_CONSISTS_SOLELY_OF_UNICORNS_AND_CONFETTI)

/// @} // end of name-group Compiler detection

/** @name Graphics configuration */ /// @{

/// SDL graphics backend.
///
/// This backend can be used with the following renderers:
///
/// - #TOGO_RENDERER_OPENGL
#define TOGO_GRAPHICS_BACKEND_SDL 0x00000001

/// GLFW graphics backend.
///
/// This backend can be used with the following renderers:
///
/// - #TOGO_RENDERER_OPENGL
#define TOGO_GRAPHICS_BACKEND_GLFW 0x00000002

/// OpenGL renderer.
#define TOGO_RENDERER_OPENGL 0x00000100

#if defined(DOXYGEN_CONSISTS_SOLELY_OF_UNICORNS_AND_CONFETTI)
	/// Configure the graphics backend to use.
	///
	/// Options:
	///
	/// - #TOGO_GRAPHICS_BACKEND_SDL
	/// - #TOGO_GRAPHICS_BACKEND_GLFW
	#define TOGO_CONFIG_GRAPHICS_BACKEND

	/// Configure the renderer to use.
	///
	/// Options:
	///
	/// - #TOGO_RENDERER_OPENGL
	#define TOGO_CONFIG_RENDERER
#else
	#if !defined(TOGO_CONFIG_GRAPHICS_BACKEND)
		#error "graphics backend has not been selected"
	#endif
	#if !defined(TOGO_CONFIG_RENDERER)
		#error "renderer has not been selected"
	#endif

	#if (TOGO_CONFIG_RENDERER != TOGO_RENDERER_OPENGL)
		#error "TOGO_CONFIG_RENDERER has an invalid value"
	#endif

	#if (TOGO_CONFIG_GRAPHICS_BACKEND == TOGO_GRAPHICS_BACKEND_SDL)
		#if (TOGO_CONFIG_RENDERER != TOGO_RENDERER_OPENGL)
			#error "selected renderer not usable with SDL graphics backend"
		#endif
	#elif (TOGO_CONFIG_GRAPHICS_BACKEND == TOGO_GRAPHICS_BACKEND_GLFW)
		#if (TOGO_CONFIG_RENDERER != TOGO_RENDERER_OPENGL)
			#error "selected renderer not usable with GLFW graphics backend"
		#endif
	#else
		#error "TOGO_CONFIG_GRAPHICS_BACKEND has an invalid value"
	#endif
#endif // defined(DOXYGEN_CONSISTS_SOLELY_OF_UNICORNS_AND_CONFETTI)

/// @} // end of name-group Graphics configuration

/** @} */ // end of doc-group config

} // namespace togo
