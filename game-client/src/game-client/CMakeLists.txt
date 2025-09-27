
target_sources(game-client PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        GameClient.cppm
)

target_sources(game-client PRIVATE
        GameClient.cpp
        main.cpp
)

add_subdirectory(camera)
add_subdirectory(character)
add_subdirectory(services)
add_subdirectory(ui)
