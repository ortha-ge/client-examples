 
cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WARN_DEPRECATED OFF)

project(ortha-client-examples LANGUAGES CXX)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

find_package(Catch2 3 REQUIRED)

enable_testing()

add_subdirectory("${PROJECT_SOURCE_DIR}/deps/soloud/contrib")
target_include_directories(soloud PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/deps/soloud/include>
        $<INSTALL_INTERFACE:include>  # <prefix>/include/mylib
)

add_subdirectory(ortha-rtti)
add_subdirectory(ortha-core)
add_subdirectory(ortha-audio)
add_subdirectory(ortha-gfx)
add_subdirectory(ortha-ui)
add_subdirectory(ortha-input)
add_subdirectory(ortha-physics2d)
add_subdirectory(ortha-scripting-js)
add_subdirectory(ortha-scripting-lua)
add_subdirectory(ortha-devtools)
add_subdirectory(game-client)

add_custom_target(all_tests)
add_dependencies(all_tests ortha-rtti-tests ortha-core-tests ortha-gfx-tests)
