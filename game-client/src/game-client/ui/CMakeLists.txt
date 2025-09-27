
target_sources(game-client PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        HUD.cppm
        MainMenu.cppm
        MainMenuController.cppm
)

target_sources(game-client PRIVATE
        HUD.cpp
        MainMenu.cpp
)

add_subdirectory(services)
add_subdirectory(systems)
