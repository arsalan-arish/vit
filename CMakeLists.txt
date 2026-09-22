cmake_minimum_required(VERSION 4.3.1)
project(vit LANGUAGES C)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(VIT_SUPER_OPTIMIZE "Enable very aggressive optimizations (defaults ON)" ON)

add_executable(${PROJECT_NAME} "test.c")

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Always build without asserts/with NDEBUG for optimized builds
target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)

# Aggressive, cross-platform optimization flags. Use generator expressions to
# apply compiler-specific flags. These aim for maximum runtime performance
# (LTO/whole-program, vectorization, fast-math, inlining).
target_compile_options(${PROJECT_NAME} PRIVATE
  # Only add aggressive MSVC optimization flags for non-Debug configs to avoid
  # conflicts with runtime checks (e.g. /RTC1). In Debug keep default flags.
  $<$<AND:$<C_COMPILER_ID:MSVC>,$<NOT:$<CONFIG:Debug>>>:/O2 /Ob2 /Oi /GL /arch:AVX2 /fp:fast /utf-8>
  # For non-MSVC compilers, enable -O3 and LTO only for non-Debug configs.
  $<$<AND:$<NOT:$<C_COMPILER_ID:MSVC>>,$<NOT:$<CONFIG:Debug>>>:-O3 -march=native -flto -ffast-math -funroll-loops -fomit-frame-pointer>
)

# Link-time optimization and non-incremental linking for best performance
target_link_options(${PROJECT_NAME} PRIVATE
  # Apply link-time codegen only for non-Debug configs.
  $<$<AND:$<C_COMPILER_ID:MSVC>,$<NOT:$<CONFIG:Debug>>>:/LTCG /INCREMENTAL:NO>
  $<$<AND:$<NOT:$<C_COMPILER_ID:MSVC>>,$<NOT:$<CONFIG:Debug>>>:-flto -s>
)

# Request CMake's interprocedural optimization where supported
set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION ON)

# Allow users to disable the "super optimize" bundle if needed
if(NOT VIT_SUPER_OPTIMIZE)
  message(STATUS "VIT_SUPER_OPTIMIZE=OFF: removing aggressive compile/link flags")
  # Remove the previously set target properties by clearing options
  target_compile_options(${PROJECT_NAME} PRIVATE "")
  target_link_options(${PROJECT_NAME} PRIVATE "")
  set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION OFF)
endif()
