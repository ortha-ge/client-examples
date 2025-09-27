
target_sources(game-client PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        CharacterControllerSystem.cppm
        CharacterSpawnerSystem.cppm
        FrogCollectorSystem.cppm
)

target_sources(game-client PRIVATE
        CharacterControllerSystem.cpp
        CharacterSpawnerSystem.cpp
        FrogCollectorSystem.cpp
)
