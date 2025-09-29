
target_sources(game-client PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        LevelBackground.cppm
        Level1Floor.cppm
        Level1Scene.cppm
)

target_sources(game-client PRIVATE
        LevelBackground.cpp
        Level1Floor.cpp
        Level1Scene.cpp
)
