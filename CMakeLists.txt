cmake_minimum_required(VERSION 3.0)
project(adventofcode C)

set(CMAKE_C_STANDARD 23)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_executable(day1_step1 day1/step1.c)
add_executable(day1_step2 day1/step2.c)
add_executable(day2_step1 day2/step1.c)

# Copy all .txt files in inputs/ to the build directory after build
file(GLOB INPUTS "inputs/*.txt")
file(COPY ${INPUTS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})