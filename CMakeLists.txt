cmake_minimum_required(VERSION 3.15)
project(EngineProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Handle output directories so binaries go to a clean folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find OpenGL (Handles Mac Framework vs Windows Lib automatically)
find_package(OpenGL REQUIRED)

# Assuming you place GLFW in third_party or install it system-wide
# find_package(glfw3 REQUIRED)

# Add subdirectories
add_subdirectory(engine)
add_subdirectory(game)