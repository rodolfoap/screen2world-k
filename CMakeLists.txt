cmake_minimum_required(VERSION 3.10)
project(screen2world)

# Force C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Equivalent to (CFLAGS) -g
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "Binary app will go to: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# Equivalent to (CFLAGS) -g
set(CMAKE_C_FLAGS "-g")

# OPENCV ####################################
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(locator locator.cpp screen2worldK.cpp)
target_compile_definitions(locator PRIVATE DEBUG=false)
target_link_libraries(locator ${OpenCV_LIBS} stdc++fs)

add_executable(gen-extrinsic gen-extrinsic.cpp screen2worldK.cpp)
target_compile_definitions(gen-extrinsic PRIVATE DEBUG=false)
target_link_libraries(gen-extrinsic ${OpenCV_LIBS} stdc++fs)
