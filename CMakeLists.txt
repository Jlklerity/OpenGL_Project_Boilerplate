cmake_minimum_required(VERSION 3.15)
project(EngineProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(LOCAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(LOCAL_LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

if(NOT EXISTS ${LOCAL_INCLUDE_DIR}/GLFW/glfw3.h)
    message(FATAL_ERROR "Missing header: ${LOCAL_INCLUDE_DIR}/GLFW/glfw3.h")
endif()
if(NOT EXISTS ${LOCAL_INCLUDE_DIR}/glad/glad.h)
    message(FATAL_ERROR "Missing header: ${LOCAL_INCLUDE_DIR}/glad/glad.h")
endif()
if(NOT EXISTS ${LOCAL_INCLUDE_DIR}/glm/glm.hpp)
    message(FATAL_ERROR "Missing header: ${LOCAL_INCLUDE_DIR}/glm/glm.hpp")
endif()
if(NOT EXISTS ${LOCAL_LIB_DIR}/libglfw3.a)
    message(FATAL_ERROR "Missing static library: ${LOCAL_LIB_DIR}/libglfw3.a")
endif()
if(NOT EXISTS ${LOCAL_LIB_DIR}/libglad.a)
    message(FATAL_ERROR "Missing static library: ${LOCAL_LIB_DIR}/libglad.a")
endif()

find_package(OpenGL REQUIRED)

add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
  IMPORTED_LOCATION ${LOCAL_LIB_DIR}/libglfw3.a
  INTERFACE_INCLUDE_DIRECTORIES ${LOCAL_INCLUDE_DIR}
)

add_library(glad STATIC IMPORTED)
set_target_properties(glad PROPERTIES
  IMPORTED_LOCATION ${LOCAL_LIB_DIR}/libglad.a
  INTERFACE_INCLUDE_DIRECTORIES ${LOCAL_INCLUDE_DIR}
)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${LOCAL_INCLUDE_DIR})

add_subdirectory(engine)
add_subdirectory(game)