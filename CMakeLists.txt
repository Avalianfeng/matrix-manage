cmake_minimum_required(VERSION 3.15)
project(matrix-manage)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 统一可执行文件输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)

# 收集源文件（假设全在 src/ 里，如果有别的目录请补充调整）
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.cc"
    "${CMAKE_SOURCE_DIR}/src/*.cxx"
)

if(NOT PROJECT_SOURCES)
    message(FATAL_ERROR "No source files found in src/. Please add sources or update CMakeLists.txt.")
endif()

# 添加可执行目标
add_executable(matrix-manage ${PROJECT_SOURCES})

# 关键：把 include/ 目录加到头文件搜索路径
target_include_directories(matrix-manage PRIVATE ${CMAKE_SOURCE_DIR}/include)

# 可选: 指定输出可执行文件名
set_target_properties(matrix-manage PROPERTIES OUTPUT_NAME "matrix-manage")
