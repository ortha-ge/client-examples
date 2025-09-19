 
cmake_minimum_required(VERSION 3.31)

find_package(bgfx CONFIG REQUIRED)

set (game-client-src
        src/game-client/GameClient.cpp
        src/game-client/main.cpp
        src/game-client/camera/systems/CameraControllerSystem.cpp
        src/game-client/character/Character.cpp
        src/game-client/character/systems/CharacterControllerSystem.cpp
        src/game-client/character/systems/CharacterSpawnerSystem.cpp
        src/game-client/services/GameClientService.cpp
)

set (game-client-modules
        src/game-client/GameClient.cppm
        src/game-client/camera/CameraController.cppm
        src/game-client/camera/systems/CameraControllerSystem.cppm
        src/game-client/character/Character.cppm
        src/game-client/character/CharacterController.cppm
        src/game-client/character/CharacterSpawner.cppm
        src/game-client/character/systems/CharacterControllerSystem.cppm
        src/game-client/character/systems/CharacterSpawnerSystem.cppm
        src/game-client/services/CameraControllerSystemService.cppm
        src/game-client/services/CharacterControllerSystemService.cppm
        src/game-client/services/CharacterSpawnerSystemService.cppm
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

set(game-client-src ${game-client-src} ${game-client-modules})

if (NOT EMSCRIPTEN_BUILD)
set(game-client-src ${game-client-src} ${game-client-shaders-vs} ${game-client-shaders-fs})
endif()

add_executable(game-client ${game-client-src})

target_sources(game-client PUBLIC FILE_SET modules TYPE CXX_MODULES FILES ${game-client-modules})

set(game-client-libs
        core-lib
        gfx-lib
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
