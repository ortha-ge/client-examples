
target_sources(game-client PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        HUDControllerSystemService.cppm
        MainMenuControllerSystemService.cppm
)

target_sources(game-client PRIVATE
)
