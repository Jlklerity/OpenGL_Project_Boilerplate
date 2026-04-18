# Group all engine sources
file(GLOB_RECURSE ENGINE_SOURCES 
    "platform/*.cpp" "platform/*.h"
    "core/*.cpp" "core/*.h"
    "scene/*.cpp" "scene/*.h"
    "assets/*.cpp" "assets/*.h"
    "render/*.cpp" "render/*.h"
)

add_library(Engine STATIC ${ENGINE_SOURCES})

# Include directories so other modules can do #include "core/Entity.h"
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link dependencies (OpenGL, GLFW)
target_link_libraries(Engine PUBLIC OpenGL::GL) # Add glfw3 here as well