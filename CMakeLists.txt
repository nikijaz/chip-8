cmake_minimum_required(VERSION 3.10.0)
project(chip-8 LANGUAGES C)

add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)

file(GLOB SRC_FILES src/*.c)
add_executable(chip-8 ${SRC_FILES})

target_compile_options(chip-8 PRIVATE -Wall)

target_link_libraries(chip-8 SDL3::SDL3 m)
target_include_directories(chip-8 PRIVATE ${SDL3_INCLUDE_DIRS})
