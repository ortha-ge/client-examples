
target_sources(game-client PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        CalculatePlayerScoreSystem.cppm
)

target_sources(game-client PRIVATE
        CalculatePlayerScoreSystem.cpp
)
