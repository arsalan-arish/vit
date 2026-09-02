cmake_minimum_required(VERSION 4.3.1)
project(vit)

add_executable(
    ${PROJECT_NAME} 
    src/main.c
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)