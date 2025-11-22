cmake_minimum_required(VERSION 3.16)
project(mdtg_starter VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mdtg
    src/main.cpp
    src/mdtg_node.cpp
    src/thread_pool.cpp
)

target_include_directories(mdtg PRIVATE ${CMAKE_SOURCE_DIR}/include)

find_package(Threads REQUIRED)
if (Threads_FOUND)
    target_link_libraries(mdtg PRIVATE Threads::Threads)
endif()

if (MSVC)
    target_compile_options(mdtg PRIVATE /W4 /permissive-)
else()
    target_compile_options(mdtg PRIVATE -Wall -Wextra -Wpedantic -O3)
endif()
