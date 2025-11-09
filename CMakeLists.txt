url=https://github.com/Avalianfeng/matrix-manage/blob/main/CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(matrix-manage)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 将运行时二进制输出到 build/bin（单配置和多配置都处理）
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)

# 如果你的源文件都在 src/ 下，使用下面的方式收集源文件
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.cxx"
    "${CMAKE_SOURCE_DIR}/src/*.cc"
    "${CMAKE_SOURCE_DIR}/src/*.c"
)

# 如果没有 src/，你也可以直接在 add_executable 中写文件列表
if(PROJECT_SOURCES)
    add_executable(matrix-manage ${PROJECT_SOURCES})
else()
    # 回退：如果没有找到 src 下的文件，尝试 main.cpp 在根目录
    if(EXISTS "${CMAKE_SOURCE_DIR}/main.cpp")
        add_executable(matrix-manage ${CMAKE_SOURCE_DIR}/main.cpp)
    else()
        message(WARNING "No source files found in src/ or main.cpp. Please add sources or update CMakeLists.txt.")
    endif()
endif()

# (可选) 释出版本信息
set_target_properties(matrix-manage PROPERTIES
    OUTPUT_NAME "matrix-manage"
)
