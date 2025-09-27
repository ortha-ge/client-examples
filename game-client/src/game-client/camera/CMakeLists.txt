
target_sources(game-client PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        CameraController.cppm
)

target_sources(game-client PRIVATE
)

add_subdirectory(services)
add_subdirectory(systems)
