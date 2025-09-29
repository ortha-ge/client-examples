 
cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)

project(ortha-client-examples LANGUAGES CXX)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

find_package(Catch2 3 REQUIRED)

enable_testing()

add_subdirectory("${PROJECT_SOURCE_DIR}/deps/soloud/contrib")
target_include_directories(soloud PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/deps/soloud/include>
        $<INSTALL_INTERFACE:include>  # <prefix>/include/mylib
)

add_subdirectory("${PROJECT_SOURCE_DIR}/core-lib")
add_subdirectory("${PROJECT_SOURCE_DIR}/audio-lib")
add_subdirectory("${PROJECT_SOURCE_DIR}/gfx-lib")
add_subdirectory("${PROJECT_SOURCE_DIR}/ui-library")
add_subdirectory("${PROJECT_SOURCE_DIR}/input-lib")
add_subdirectory("${PROJECT_SOURCE_DIR}/physics2d-lib")
add_subdirectory("${PROJECT_SOURCE_DIR}/scripting-js-lib")
add_subdirectory("${PROJECT_SOURCE_DIR}/scripting-lua-lib")
add_subdirectory("${PROJECT_SOURCE_DIR}/devtools-lib")
add_subdirectory("${PROJECT_SOURCE_DIR}/game-client")

add_custom_target(all_tests)
add_dependencies(all_tests core-lib-tests gfx-lib-tests)
