 
cmake_minimum_required(VERSION 3.31)

find_package(bgfx CONFIG REQUIRED)

set (game-client-shaders-vs
        assets/shaders/vs_font.sc
        assets/shaders/vs_ocornut_imgui.sc
        assets/shaders/vs_ocornut_imgui_image.sc
        assets/shaders/vs_sprite.sc
        assets/shaders/vs_ui_colour.sc
        assets/shaders/vs_ui_image.sc
)

set (game-client-shaders-fs
        assets/shaders/fs_font.sc
        assets/shaders/fs_ocornut_imgui.sc
        assets/shaders/fs_ocornut_imgui_image.sc
        assets/shaders/fs_sprite.sc
        assets/shaders/fs_ui_colour.sc
        assets/shaders/fs_ui_image.sc
)

bgfx_compile_shaders(
	TYPE VERTEX
	SHADERS ${game-client-shaders-vs}
# 	VARYING_DEF filename
	OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets/shaders/"
	# 	OUT_FILES_VAR variable name
	# 	INCLUDE_DIRS directories
	# 	[AS_HEADERS]
)

bgfx_compile_shaders(
		TYPE FRAGMENT
		SHADERS ${game-client-shaders-fs}
		# 	VARYING_DEF filename
		OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets/shaders/"
		# 	OUT_FILES_VAR variable name
		# 	INCLUDE_DIRS directories
		# 	[AS_HEADERS]
)

set(game-client-shaders)

if (NOT EMSCRIPTEN_BUILD)
set(game-client-shaders ${game-client-shaders-vs} ${game-client-shaders-fs})
endif()

add_executable(game-client ${game-client-shaders})

set(game-client-libs
        core-lib
        gfx-lib
        ui-lib
        input-lib
        audio-lib
        devtools-lib
        physics2d-lib
        scripting-js-lib
        scripting-lua-lib
)

target_compile_features(game-client PRIVATE cxx_std_23)
target_link_libraries(game-client PRIVATE ${game-client-libs})
target_include_directories(game-client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (EMSCRIPTEN_BUILD)
	set_target_properties(game-client PROPERTIES SUFFIX ".html")
	set_target_properties(game-client PROPERTIES LINK_FLAGS "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets@/assets/")
	set_target_properties(game-client PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/assets)
endif()

add_custom_target(game-client-assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/"
        "$<TARGET_FILE_DIR:game-client>/assets/"
    COMMENT "Copying assets"
)

add_dependencies(game-client game-client-assets)

add_subdirectory(src)
