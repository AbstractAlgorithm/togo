
local S, G, R = precore.helpers()

precore.make_config("togo.lib.window.backend.opt", {
	once = true,
}, {
{option = {
	data = {
		trigger = "togo-window-backend",
		value = "BACKEND",
		description = "select the window system backend",
		allowed = {
			{"sdl", "SDL2 backend (default)"},
			{"glfw", "GLFW backend"},
		}
	},
	init_handler = function()
		if not _OPTIONS["togo-window-backend"] then
			_OPTIONS["togo-window-backend"] = "sdl"
		end
	end
}}})

precore.make_config("togo.lib.window.enable-opengl.opt", {
	once = true,
}, {
{option = {
	data = {
		trigger = "togo-enable-opengl",
		description = "enable OpenGL support",
	},
	init_handler = function()
	end
}}})

precore.make_config("togo.lib.window.backend.dep", nil, {
{project = function(p)
	-- premake4.4-beta5 doesn't have the "opt=value" syntax for configuration
	-- selection. TODO: Fix these yucklings when there's a release that does.
	local backend = _OPTIONS["togo-window-backend"]
	configuration {}
	if backend == "sdl" then
		defines {
			"TOGO_CONFIG_WINDOW_BACKEND=TOGO_WINDOW_BACKEND_SDL",
		}
		includedirs {
			G"${DEP_PATH}/sdl/include/",
		}
		if not precore.env_project()["NO_LINK"] then
			libdirs {
				G"${DEP_PATH}/sdl/lib/",
			}
			links {
				":libSDL2.a",
			}
		end
	elseif backend == "glfw" then
		assert(_OPTIONS["togo-enable-opengl"], "GLFW backend requires --togo-enable-opengl")
		defines {
			"TOGO_CONFIG_WINDOW_BACKEND=TOGO_WINDOW_BACKEND_GLFW",
		}
		includedirs {
			G"${DEP_PATH}/glfw/include/",
		}
		if not precore.env_project()["NO_LINK"] then
			configuration {"linux"}
				linkoptions {
					"`pkg-config --static --libs " ..
					G"${DEP_PATH}/glfw/lib/pkgconfig/glfw3.pc" ..
					"`"
				}
			configuration {}
				libdirs {
					G"${DEP_PATH}/glfw/lib/",
				}
				links {
					":libglfw3.a",
				}
		end
	end

	if _OPTIONS["togo-enable-opengl"] then
		defines {
			"TOGO_CONFIG_WINDOW_ENABLE_OPENGL",
		}
		precore.apply("togo.dep.opengl")
	end
end}})

precore.make_config("togo.lib.window.dep", {
	reverse = true,
}, {
"togo.base",
"togo.lib.core.dep",
"togo.lib.window.backend.dep",
{project = function(p)
	togo.library_config("window")

	configuration {"togo-test"}
		defines {
			"TOGO_TEST_INPUT_BUFFER",
		}
end}})

precore.append_config_scoped("togo.projects", {
{global = function(_)
	togo.make_library("window", {
		"togo.lib.window.dep",
	})
end}})

precore.apply_global({
	"togo.lib.window.enable-opengl.opt",
	"togo.lib.window.backend.opt",
})
