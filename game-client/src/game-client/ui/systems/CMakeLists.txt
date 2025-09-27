
target_sources(game-client PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        HUDControllerSystem.cppm
        MainMenuControllerSystem.cppm
)

target_sources(game-client PRIVATE
        HUDControllerSystem.cpp
        MainMenuControllerSystem.cpp
)
