# 1. 指定 CMake 最低版本要求
cmake_minimum_required(VERSION 3.15)

set(target_name ToyRenderer)

# 2. 声明项目名称、版本和语言
project(${target_name} VERSION 1.0 LANGUAGES CXX)

# 3. 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 4. 查找源文件 (递归版本)
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
)
file(GLOB_RECURSE INCLUDES 
    "include/*.h" 
)

add_executable(${target_name} ${SOURCES}
						           ${INCLUDES})

target_include_directories(${target_name} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)