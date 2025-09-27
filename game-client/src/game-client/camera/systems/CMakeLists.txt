
target_sources(game-client PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        CameraControllerSystem.cppm
)

target_sources(game-client PRIVATE
        CameraControllerSystem.cpp
)
