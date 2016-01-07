#line 2 "togo/core/lua/lua.cpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.
*/

#include <togo/core/config.hpp>
#include <togo/core/error/assert.hpp>
#include <togo/core/utility/utility.hpp>
#include <togo/core/memory/memory.hpp>
#include <togo/core/string/string.hpp>
#include <togo/core/lua/lua.hpp>

#include <cstring>

namespace togo {

static void* alloc_func(void* ud, void* ptr, size_t osize, size_t nsize) {
	auto a = static_cast<Allocator*>(ud);
	if (nsize == 0) {
		a->deallocate(ptr);
		return nullptr;
	}

	void* new_ptr = a->allocate(nsize);
	// TODO: does Lua do resize blocks a lot? maybe this should be a nop
	// (allocators don't do reallocs)
	if (osize > 0) {
		std::memcpy(new_ptr, ptr, nsize);
	}
	a->deallocate(ptr);
	return new_ptr;
}

/// Create a new state with the given allocator.
///
/// allocator must exist as long as the state.
lua_State* lua::new_state(Allocator& allocator) {
	return lua_newstate(alloc_func, &allocator);
}

/// Create a new state with Lua's allocator.
lua_State* lua::new_state() {
	return luaL_newstate();
}

/// lua_pcall() error message handler.
///
/// Returns a traceback as a string.
signed lua::pcall_error_message_handler(lua_State* L) {
	if (lua_isstring(L, 1)) {
		luaL_traceback(L, L, lua_tostring(L, 1), 1);
	}
	return 1;
}

namespace {

static signed package_preloader(lua_State* L) {
	StringRef specified_chunk_name = lua::get_string(L, lua_upvalueindex(1));
	StringRef source = lua::get_string(L, lua_upvalueindex(2));
	auto funcs = array_cref(
		unsigned_cast(lua::get_integer(L, lua_upvalueindex(3))),
		lua::get_lightuserdata_typed<LuaModuleFunction const>(L, lua_upvalueindex(4))
	);
	StringRef modname = lua::get_string(L, 1);
	FixedArray<char, 128> chunk_name{};

	string::append(chunk_name, "@");
	if (specified_chunk_name.any()) {
		string::append(chunk_name, specified_chunk_name);
	} else {
		string::append(chunk_name, modname);
	}

	lua::push_value(L, lua::pcall_error_message_handler);
	// -> chunk
	if (luaL_loadbufferx(L, source.data, source.size, begin(chunk_name), "t")) {
		return lua_error(L);
	}
	// -> result
	if (lua_pcall(L, 0, 1, -2)) {
		return lua_error(L);
	}

	if (lua_type(L, -1) != LUA_TTABLE) {
		luaL_error(L, "wanted a table return value from module, but got a %s", luaL_typename(L, -1));
	}
	if (begin(funcs)) {
		for (auto const& mf : funcs) {
			lua::table_set(L, mf.name, mf.func);
		}
	}

	lua::table_get(L, "__module_init__");
	if (lua_type(L, -1) == LUA_TFUNCTION) {
		lua_pushvalue(L, -2);
		if (lua_pcall(L, 1, 0, -3)) {
			return lua_error(L);
		}
	} else {
		lua_pop(L, 1);
	}

	// remove preloader
	lua::table_get_raw(L, LUA_REGISTRYINDEX, "_PRELOAD");
	lua::table_set_raw(L, modname, null_tag{});
	lua_pop(L, 1); // _PRELOAD

	lua_remove(L, -2); // pcall_message_handler
	return 1; // result
}

} // anonymous namespace

/// Put module in package.preload[modname].
///
/// source must be valid as long as the state is alive.
void lua::preload_module(lua_State* L, LuaModuleRef const& module) {
	lua::table_get_raw(L, LUA_REGISTRYINDEX, "_PRELOAD");
	lua::push_value(L, module.name);
	lua::push_value(L, module.chunk_name);
	lua::push_value(L, module.source);
	lua::push_value(L, module.funcs.size());
	lua::push_lightuserdata(L, const_cast<LuaModuleFunction*>(begin(module.funcs)));
	lua_pushcclosure(L, package_preloader, 4);
	lua_rawset(L, -3); // _PRELOAD[module.name] = closure
	lua_pop(L, 1); // _PRELOAD
}

/// Register basic interfaces.
void lua::register_core(lua_State* L) {
	lua_createtable(L, 0, 32);
	lua::table_set_copy_raw(L, LUA_REGISTRYINDEX, "togo_class", -1);
	lua_pop(L, 1);

	utility::register_lua_interface(L);
}

} // namespace togo
