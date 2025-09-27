
target_sources(game-client PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        GameClientService.cppm
)

target_sources(game-client PRIVATE
        GameClientService.cpp
)
