
target_sources(game-client PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        Character.cppm
        CharacterController.cppm
        CharacterSpawner.cppm
        FrogCollector.cppm
)

target_sources(game-client PRIVATE
        Character.cpp
)

add_subdirectory(services)
add_subdirectory(systems)
