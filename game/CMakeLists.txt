file(GLOB_RECURSE GAME_SOURCES "*.cpp" "*.h")

add_executable(GameApp ${GAME_SOURCES})

# Link the game to the engine
target_link_libraries(GameApp PRIVATE Engine)