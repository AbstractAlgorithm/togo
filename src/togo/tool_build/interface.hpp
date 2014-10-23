#line 2 "togo/tool_build/interface.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.

@file
@brief tool_build interface.
@ingroup tool_build_interface
*/

#pragma once

#include <togo/tool_build/config.hpp>
#include <togo/string_types.hpp>
#include <togo/kvs_types.hpp>
#include <togo/tool_build/types.hpp>

namespace togo {
namespace tool_build {
namespace interface {

/**
	@addtogroup tool_build_interface
	@{
*/

/// Get project path.
inline StringRef project_path(
	Interface const& interface
) {
	return {interface._project_path};
}

/// Read options.
///
/// This should only be called before init().
bool read_options(
	Interface& interface,
	KVS const& k_options
);

/// Initialize interface.
///
/// This must be called for the project path to be assigned.
/// If project_path is empty, the TOGO_PROJECT environment variable
/// will be used.
/// If there is no project path, an assertion will fail.
void init(
	Interface& interface,
	StringRef const& project_path,
	bool const register_standard_compilers
);

/// Add package by name.
void add_package(
	Interface& interface,
	StringRef const& name
);

/// Read project data.
void read_project(
	Interface& interface
);

/// Write project data.
void write_project(
	Interface& interface
);

/// Run tool with main() arguments.
bool run(
	Interface& interface,
	KVS const& k_command_options,
	KVS const& k_command
);

/// Run help command.
///
/// Prints command help information.
bool command_help(
	Interface const& interface,
	StringRef const& command_name
);

/// Run help command with KVS.
///
/// Specification: `help [command_name]`.
bool command_help(
	Interface const& interface,
	KVS const& k_command_options,
	KVS const& k_command
);

/// Run list command.
///
/// Lists packages in the project.
bool command_list(
	Interface const& interface
);

/// Run list command with KVS.
///
/// Specification: `list`.
bool command_list(
	Interface const& interface,
	KVS const& k_command_options,
	KVS const& k_command
);

/// Run create command.
///
/// Creates package.
bool command_create(
	Interface& interface,
	StringRef const& name
);

/// Run create command with KVS.
///
/// Specification: `create <name>`.
bool command_create(
	Interface& interface,
	KVS const& k_command_options,
	KVS const& k_command
);

/** @} */ // end of doc-group tool_build_interface

} // namespace interface
} // namespace tool_build
} // namespace togo
