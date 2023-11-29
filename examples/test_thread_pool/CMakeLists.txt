cmake_minimum_required(VERSION 3.17)

# requires C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 项目信息
project(test_thread_pool)
set(PROJECT_SOURCE_DIR "../../")

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -g")

# 导入文件
include_directories("${PROJECT_SOURCE_DIR}/src/")
include_directories("${PROJECT_SOURCE_DIR}/include/")
aux_source_directory("${PROJECT_SOURCE_DIR}/src/" SRC_LIST)

# 指定生成目标
add_executable(test_thread_pool test_thread_pool.cpp ${SRC_LIST})

# 添加依赖的公共库
target_link_libraries(test_thread_pool pthread)
