#line 2 "togo/resource/resource.cpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.
*/

#include <togo/config.hpp>
#include <togo/utility/utility.hpp>
#include <togo/collection/fixed_array.hpp>
#include <togo/algorithm/sort.hpp>
#include <togo/string/string.hpp>
#include <togo/resource/resource.hpp>

namespace togo {

struct TagLessThan {
	inline bool operator()(
		ResourcePathParts::Tag const& x,
		ResourcePathParts::Tag const& y
	) const noexcept {
		return x.hash < y.hash;
	}
};

bool resource::parse_path(
	StringRef const& path,
	ResourcePathParts& pp
) {
	enum : unsigned {
		NOT_FOUND = ~0u,
	};

	pp.type_hash = RES_TYPE_NULL;
	pp.name_hash = RES_NAME_NULL;
	pp.type = StringRef{};
	pp.name = StringRef{};
	fixed_array::clear(pp.tags);

	unsigned ext_i = NOT_FOUND;
	unsigned tag_i = NOT_FOUND;
	unsigned tag_first = NOT_FOUND;
	ResourcePathParts::Tag tag;
	for (unsigned i = 0; i < path.size; ++i) {
		if (path.data[i] == '.') {
			if (ext_i == NOT_FOUND) {
				ext_i = i;
			} else {
				return false;
			}
		} else if (path.data[i] == '#') {
			if (tag_i == NOT_FOUND) {
				tag_first = i;
			} else if (tag_i == i || fixed_array::space(pp.tags) == 0) {
				return false;
			} else {
				tag.name = {
					path.data + tag_i,
					i - tag_i
				};
				tag.hash = hash::calc32(tag.name);
				fixed_array::push_back(pp.tags, tag);
			}
			tag_i = i + 1;
		}
	}

	unsigned const last_index = path.size - 1;
	unsigned const type_first = ext_i + 1;
	unsigned const type_last = min(tag_first, path.size);
	if (
		ext_i == NOT_FOUND || ext_i == 0 || ext_i == last_index ||
		ext_i > tag_first  || type_last - type_first == 0 ||
		tag_i == path.size ||
		path.data[ext_i - 1] == '/' ||
		path.data[ext_i - 1] == '\\'
	) {
		return false;
	}
	pp.type = StringRef{path.data + type_first, type_last - type_first};
	pp.name = StringRef{path.data, ext_i};
	pp.type_hash = resource::hash_type(pp.type);
	pp.name_hash = resource::hash_name(pp.name);
	if (tag_i < path.size) {
		if (fixed_array::space(pp.tags) == 0) {
			return false;
		}
		tag.name = {
			path.data + tag_i,
			path.size - tag_i
		};
		tag.hash = hash::calc32(tag.name);
		fixed_array::push_back(pp.tags, tag);
	}
	sort_insertion(begin(pp.tags), end(pp.tags), TagLessThan{});
	pp.tags_hash = resource::hash_tags(
		begin(pp.tags),
		fixed_array::size(pp.tags)
	);
	return true;
}

void resource::compiled_path(
	ResourceCompiledPath& path,
	u32 const id
) {
	if (path.id == 0) {
		string::copy(path._data, ".compiled/");
	}
	path.id = id;
	unsigned const size = string::size(".compiled/");
	// TODO: string::append_integer()
	signed const id_size = std::snprintf(
		fixed_array::begin(path._data) + size,
		fixed_array::capacity(path._data) - size,
		"%u", id
	);
	TOGO_ASSERTE(id_size > 0);
	fixed_array::resize(path._data, size + id_size + 1);
	// Don't Trust the Standard Library: A Pessimist's Guide
	fixed_array::back(path._data) = '\0';
}

} // namespace togo
