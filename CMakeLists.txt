cmake_minimum_required(VERSION 3.10)  # Minimum CMake version required
project(matrix-manage)  # Project name

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Add executable target
add_executable(main src/main.cpp src/decimalToFraction.cpp)