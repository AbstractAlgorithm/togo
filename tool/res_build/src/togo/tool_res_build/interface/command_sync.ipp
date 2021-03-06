#line 2 "togo/tool_res_build/interface/command_sync.ipp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.
*/

namespace togo {
namespace tool_res_build {

static bool sync_package(
	Interface& interface,
	PackageCompiler& pkg
) {
	StringRef const pkg_name = package_compiler::name(pkg);
	StringRef const pkg_path = package_compiler::path(pkg);
	TOGO_LOGF("syncing package '%.*s':\n", pkg_name.size, pkg_name.data);

	WorkingDirScope wd_scope{pkg_path};

	// Status:
	//   N: no change
	//   D: resource removed
	//   A: resource added
	//   I: ignored (path-parse failed or no resource handler for type)
	bool no_status_logs = true;
	#define TOGO_LOG_STATUS_(path, status, post)					\
		TOGO_LOGF(													\
			" %c  %.*s / %.*s " post "\n",							\
			status,													\
			pkg_name.size, pkg_name.data,							\
			string::size(path), begin(path)							\
		);															\
		no_status_logs = false

	{// Remove resources that have been deleted
	for (auto const& metadata : package_compiler::manifest(pkg)) {
		if (metadata.id == 0) {
			// Skip holes
			continue;
		}

		if (filesystem::is_file(metadata.path)) {
			TOGO_LOG_STATUS_(metadata.path, 'N', "");
		} else {
			TOGO_LOG_STATUS_(metadata.path, 'D', "");
			package_compiler::remove_resource(pkg, metadata.id);
		}
	}}

	{// Read new files in the package tree
	DirectoryReader dir_reader{};
	if (!directory_reader::open(dir_reader, ".", false, true, true)) {
		TOGO_LOG_ERRORF(
			"failed to open directory for package '%.*s': '%.*s'\n",
			pkg_name.size, pkg_name.data,
			pkg_path.size, pkg_path.data
		);
		return false;
	}

	ResourcePathParts pp;
	DirectoryEntry entry;
	while (directory_reader::read(dir_reader, entry, DirectoryEntry::Type::file)) {
		if (!resource::parse_path(entry.path, pp)) {
			TOGO_LOG_STATUS_(entry.path, 'I', "(path parse failed)");
		} else if (!compiler_manager::has_compiler(interface._manager, pp.type_hash)) {
			TOGO_LOG_STATUS_(entry.path, 'I', "(no compiler for type)");
		} else if (package_compiler::find_resource_id(pkg, pp, false) == 0) {
			TOGO_LOG_STATUS_(entry.path, 'A', "");
			package_compiler::add_resource(pkg, entry.path, pp);
		}
	}
	directory_reader::close(dir_reader);
	}
	#undef TOGO_LOG_STATUS_

	if (no_status_logs) {
		TOGO_LOG(" (no changes)\n\n");
	} else {
		TOGO_LOG("\n");
	}

	return package_compiler::write(pkg);
}

/// Run sync command.
///
/// Updates package manifests to match filesystem.
/// If no packages are specified, all packages are synced.
bool interface::command_sync(
	Interface& interface,
	ArrayRef<StringRef const> const package_names
) {
	auto& packages = compiler_manager::packages(interface._manager);
	if (package_names.size() > 0) {
		PackageCompiler* pkg;
		for (auto const& pkg_name : package_names) {
			pkg = compiler_manager::find_package(
				const_cast<CompilerManager&>(interface._manager),
				resource::hash_package_name(pkg_name)
			);
			if (pkg) {
				if (!sync_package(interface, *pkg)) {
					return false;
				}
			} else {
				TOGO_LOG_ERRORF(
					"package not found: '%.*s'\n",
					pkg_name.size, pkg_name.data
				);
				return false;
			}
		}
	} else if (array::any(packages)) {
		for (auto* pkg : packages) {
			if (!sync_package(interface, *pkg)) {
				return false;
			}
		}
	} else {
		TOGO_LOG("no packages to sync\n");
	}
	return true;
}

/// Run sync command with KVS.
///
/// Specification:
/// @verbatim sync [<package_name> ...] @endverbatim
bool interface::command_sync(
	Interface& interface,
	KVS const& k_command_options,
	KVS const& k_command
) {
	if (kvs::any(k_command_options)) {
		TOGO_LOG("error: options unexpected\n");
		return false;
	}
	Array<StringRef> package_names{memory::scratch_allocator()};
	for (KVS const& k_pkg_name : k_command) {
		if (!kvs::is_string(k_pkg_name) || kvs::string_size(k_pkg_name) == 0) {
			TOGO_LOG("error: expected non-empty string argument\n");
			return false;
		}
		array::push_back(package_names, kvs::string_ref(k_pkg_name));
	}
	return interface::command_sync(interface, package_names);
}

} // namespace tool_res_build
} // namespace togo
