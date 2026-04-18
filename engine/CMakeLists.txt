add_library(Engine STATIC
    # Platform
    platform/Window.cpp
    platform/Window.h

    # Core
    core/Entity.h
    core/Registry.h
    core/Camera.h
    core/Camera.cpp
    core/InputSystem.h
    core/InputSystem.cpp

    # Render – frontend (backend-agnostic)
    render/frontend/Renderer.h
    render/frontend/Renderer.cpp
    render/frontend/RenderQueue.h

    # Render – OpenGL backend
    render/opengl/GLBuffer.h
    render/opengl/GLBuffer.cpp
    render/opengl/GLVertexArray.h
    render/opengl/GLVertexArray.cpp
    render/opengl/GLShader.h
    render/opengl/GLShader.cpp
    render/opengl/GLTexture.h
    render/opengl/GLTexture.cpp
)

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(APPLE)
  target_link_libraries(Engine PUBLIC
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
  )
endif()

target_link_libraries(Engine PUBLIC
  OpenGL::GL
  glfw
  glm
  glad
)
