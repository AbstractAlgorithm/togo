
#include <togo/core/error/assert.hpp>
#include <togo/core/utility/utility.hpp>
#include <togo/core/log/log.hpp>
#include <togo/core/collection/fixed_array.hpp>
#include <togo/core/string/string.hpp>
#include <togo/core/hash/hash.hpp>
#include <togo/game/resource/resource.hpp>

#include <initializer_list>

using namespace togo;
using namespace togo::game;

void test_path(
	ResourcePathParts& pp,
	StringRef const path,
	bool const expect_valid,
	StringRef const name,
	StringRef const type,
	std::initializer_list<StringRef> tags
) {
	bool const valid = resource::parse_path(path, pp);
	TOGO_LOGF("%s: '%.*s'\n", valid ? "valid  " : "invalid", path.size, path.data);
	if (valid) {
		TOGO_LOGF("   name: %016lx '%.*s'\n", pp.name_hash, pp.name.size, pp.name.data);
		TOGO_LOGF("   type: %08x%8s '%.*s'\n", pp.type_hash, "", pp.type.size, pp.type.data);
		TOGO_LOGF("   tags: %016lx [", pp.tags_hash);
		for (unsigned i = 0; i < fixed_array::size(pp.tags); ++i) {
			TOGO_LOGF(
				"%08x '%.*s'%.*s",
				pp.tags[i].hash,
				pp.tags[i].name.size,
				pp.tags[i].name.data,
				fixed_array::size(pp.tags) == i + 1 ? 0 : 2, ", "
			);
		}
		TOGO_LOG("]\n\n");

		auto const type_hash = resource::hash_type(type);
		auto const name_hash = resource::hash_name(name);
		TOGO_ASSERTE(type_hash == pp.type_hash);
		TOGO_ASSERTE(string::compare_equal(type, pp.type));
		TOGO_ASSERTE(name_hash == pp.name_hash);
		TOGO_ASSERTE(string::compare_equal(name, pp.name));
		TOGO_ASSERTE(
			fixed_array::empty(pp.tags) ==
			(pp.tags_hash == RES_TAGS_NULL)
		);

		for (unsigned i = 0; i < fixed_array::size(pp.tags); ++i) {
			auto const name = *(begin(tags) + i);
			auto const hash = hash::calc_generic<
				decltype(ResourcePathParts::Tag::hash)
			>(name);
			TOGO_ASSERTE(hash == pp.tags[i].hash);
			TOGO_ASSERTE(string::compare_equal(name, pp.tags[i].name));
		}
		TOGO_ASSERTE(fixed_array::size(pp.tags) == tags.size());
	}
	TOGO_ASSERTE(valid == expect_valid);
}

signed main() {
	ResourcePathParts pp;

	test_path(pp, "", false, "", "", {});
	test_path(pp, ".", false, "", "", {});
	test_path(pp, "/.", false, "", "", {});
	test_path(pp, "\\.", false, "", "", {});
	test_path(pp, ".c", false, "", "", {});
	test_path(pp, "b.", false, "", "", {});
	test_path(pp, "a", false, "", "", {});
	test_path(pp, "a/", false, "", "", {});
	test_path(pp, "a/.", false, "", "", {});
	test_path(pp, "a/b.", false, "", "", {});
	test_path(pp, "a/b.c.", false, "", "", {});
	test_path(pp, "a/b.c#.1", false, "", "", {});
	test_path(pp, "a#1", false, "", "", {});
	test_path(pp, ".#1", false, "", "", {});
	test_path(pp, "/.#1", false, "", "", {});
	test_path(pp, "\\.#1", false, "", "", {});
	test_path(pp, "b.#1", false, "", "", {});
	test_path(pp, ".c#1", false, "", "", {});
	test_path(pp, ".c#1", false, "", "", {});
	test_path(pp, "#", false, "", "", {});
	test_path(pp, "##", false, "", "", {});
	test_path(pp, "b.c#", false, "", "", {});
	test_path(pp, "b.c#1#", false, "", "", {});
	test_path(pp, "b.c##", false, "", "", {});
	test_path(pp, "b.c##1", false, "", "", {});
	test_path(pp, "a.b/c.d", false, "", "", {});
	test_path(pp, "b.c#1#2#3#4#5#6#7#8#9", false, "", "", {});

	TOGO_LOG("\n");
	test_path(pp, "b.c", true, "b", "c", {});
	test_path(pp, "a/b.c", true, "a/b", "c", {});
	test_path(pp, "b.c#1", true, "b", "c", {"1"});
	test_path(pp, "b.c#1#2", true, "b", "c", {"1", "2"});

	test_path(
		pp, "core/renderer/default_rc.render_config#linux",
		true,
		"core/renderer/default_rc",
		"render_config",
		{"linux"}
	);

	return 0;
}
