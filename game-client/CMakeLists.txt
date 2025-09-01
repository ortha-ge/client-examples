 
cmake_minimum_required(VERSION 3.31)

set (game-client-src
    src/game-client/GameClient.cpp
    src/game-client/main.cpp
)

set (game-client-modules
    src/game-client/GameClient.cppm
    src/game-client/services/GameClientService.cppm
)

set (game-client-shaders-vs
	assets/shaders/vs_sprite.sc
)

set (game-client-shaders-fs
	assets/shaders/fs_sprite.sc
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

add_executable(game-client ${game-client-src} ${game-client-modules} ${game-client-shaders-vs} ${game-client-shaders-fs})

target_sources(game-client PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
    ${game-client-modules}
)

target_compile_features(game-client PRIVATE cxx_std_23)
target_link_libraries(game-client PRIVATE core-lib gfx-lib input-lib audio-lib devtools-lib scripting-js-lib scripting-lua-lib)
target_include_directories(game-client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (EMSCRIPTEN_PRESET)
	set_target_properties(game-client PROPERTIES SUFFIX ".html")
endif()

add_custom_target(game-client-assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/"
        "$<TARGET_FILE_DIR:game-client>/assets/"
    COMMENT "Copying assets"
)
add_dependencies(game-client game-client-assets)
