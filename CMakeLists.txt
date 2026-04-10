cmake_minimum_required(VERSION 3.16)
project(server_project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -Wunused-result)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(server 
    main.cpp
    ${SOURCES}
)

target_include_directories(server 
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)