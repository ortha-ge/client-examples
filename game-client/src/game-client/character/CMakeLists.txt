
target_sources(game-client PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        CatCharacter.cppm
        Character.cppm
        CharacterController.cppm
        CharacterSpawner.cppm
        FrogCharacter.cppm
        FrogCollector.cppm
)

target_sources(game-client PRIVATE
        CatCharacter.cpp
        Character.cpp
        CharacterSpawner.cpp
        FrogCharacter.cpp
)

add_subdirectory(services)
add_subdirectory(systems)
