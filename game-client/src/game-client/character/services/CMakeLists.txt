
target_sources(game-client PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        CharacterControllerSystemService.cppm
        CharacterSpawnerSystemService.cppm
)

target_sources(game-client PRIVATE
)
