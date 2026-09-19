cmake_minimum_required(VERSION 4.3.1)
project(vit LANGUAGES C)
set(CMAKE_C_STANDARD 23)            
set(CMAKE_C_STANDARD_REQUIRED ON)    
set(CMAKE_C_EXTENSIONS OFF)          

add_executable(
    ${PROJECT_NAME} 
#    "src/main.c"
#    "src/cli_parser.c"
#    "src/commands/init.c"
    "test.c"
  )

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)