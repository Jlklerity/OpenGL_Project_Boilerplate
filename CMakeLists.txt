cmake_minimum_required(VERSION 3.15)
project(EngineProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(LOCAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(LOCAL_LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

if(WIN32)
    set(GLFW_LIB_CANDIDATES glfw3.lib libglfw3.a)
    set(GLAD_LIB_CANDIDATES glad.lib libglad.a)
elseif(APPLE)
    set(GLFW_LIB_CANDIDATES libglfw3.a)
    set(GLAD_LIB_CANDIDATES libglad.a)
else()
    set(GLFW_LIB_CANDIDATES libglfw.so.3.3 glfw3.a)
    set(GLAD_LIB_CANDIDATES libglad.a glad.a)
endif()

set(GLFW_LIB_PATH "")
foreach(lib_name IN LISTS GLFW_LIB_CANDIDATES)
    if(EXISTS ${LOCAL_LIB_DIR}/${lib_name})
        set(GLFW_LIB_PATH ${LOCAL_LIB_DIR}/${lib_name})
        break()
    endif()
endforeach()

set(GLAD_LIB_PATH "")
foreach(lib_name IN LISTS GLAD_LIB_CANDIDATES)
    if(EXISTS ${LOCAL_LIB_DIR}/${lib_name})
        set(GLAD_LIB_PATH ${LOCAL_LIB_DIR}/${lib_name})
        break()
    endif()
endforeach()

if(NOT EXISTS ${LOCAL_INCLUDE_DIR}/GLFW/glfw3.h)
    message(FATAL_ERROR "Missing header: ${LOCAL_INCLUDE_DIR}/GLFW/glfw3.h")
endif()
if(NOT EXISTS ${LOCAL_INCLUDE_DIR}/glad/glad.h)
    message(FATAL_ERROR "Missing header: ${LOCAL_INCLUDE_DIR}/glad/glad.h")
endif()
if(NOT EXISTS ${LOCAL_INCLUDE_DIR}/glm/glm.hpp)
    message(FATAL_ERROR "Missing header: ${LOCAL_INCLUDE_DIR}/glm/glm.hpp")
endif()
if(GLFW_LIB_PATH STREQUAL "")
    message(FATAL_ERROR "Missing GLFW static library in ${LOCAL_LIB_DIR}. Expected one of: ${GLFW_LIB_CANDIDATES}")
endif()
if(GLAD_LIB_PATH STREQUAL "")
    message(FATAL_ERROR "Missing GLAD static library in ${LOCAL_LIB_DIR}. Expected one of: ${GLAD_LIB_CANDIDATES}")
endif()

message(STATUS "Using GLFW static library: ${GLFW_LIB_PATH}")
message(STATUS "Using GLAD static library: ${GLAD_LIB_PATH}")

find_package(OpenGL REQUIRED)

add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION ${GLFW_LIB_PATH}
  INTERFACE_INCLUDE_DIRECTORIES ${LOCAL_INCLUDE_DIR}
)

add_library(glad STATIC IMPORTED)
set_target_properties(glad PROPERTIES
    IMPORTED_LOCATION ${GLAD_LIB_PATH}
  INTERFACE_INCLUDE_DIRECTORIES ${LOCAL_INCLUDE_DIR}
)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${LOCAL_INCLUDE_DIR})

add_subdirectory(engine)
add_subdirectory(game)