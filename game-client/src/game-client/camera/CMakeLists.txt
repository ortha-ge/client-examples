
target_sources(game-client PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        Camera.cppm
        CameraController.cppm
)

target_sources(game-client PRIVATE
        Camera.cpp
)

add_subdirectory(services)
add_subdirectory(systems)
