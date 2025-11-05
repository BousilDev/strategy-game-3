# was 3.11.0, newest on ubuntu 24.04 is 3.28.3
cmake_minimum_required(VERSION 3.22)
project(cpp_game VERSION 0.1.0)

enable_language(CXX)

# use C++ standard version 17
set(CMAKE_CXX_STANDARD 17)
# produce non-optimized code with debug symbols
set(CMAKE_BUILD_TYPE Debug)

message(STATUS "BUILD directory is ${CMAKE_BINARY_DIR}")
message(STATUS "SRC directory is ${CMAKE_SOURCE_DIR}")

# 2.5.1 on intel machines, 2.6.1 on amd machines (WSL)
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

# Add all .c, - files under src/ directory
file(GLOB SOURCES_C "src/*.c")
# Add all .cpp, - files under src/ directory
file(GLOB SOURCES_CPP "src/*.cpp")
# Add all .hpp, - files under include/ directory
file(GLOB SOURCES_HPP "include/*.hpp")
# Add all .h, - files under src/ directory
file(GLOB SOURCES_H "src/*.h")
set(SOURCES ${SOURCES_C} ${SOURCES_CPP} ${SOURCES_H} ${SOURCES_HPP})
message(STATUS "SRC directory source code files: \n\t-- HEADERS: ${SOURCES_H}${SOURCES_HPP}\n\t-- C SOURCES: ${SOURCES_C}\n\t-- CPP SOURCE:${SOURCES_CPP}\n\t-- ALL:${SOURCES}")
# add the executable target
add_executable(cpp_game)

# Find Box2D library
#find_package(Box2D REQUIRED)
#target_link_libraries(${EXECUTABLE_NAME} Box2D)

target_sources(cpp_game PUBLIC ${SOURCES})
target_include_directories(cpp_game PUBLIC .)
target_include_directories(cpp_game PUBLIC src)
target_include_directories(cpp_game PUBLIC include)
target_link_libraries(cpp_game PRIVATE sfml-graphics sfml-window sfml-system)

# get a lot of warnings
if(MSVC)
    target_compile_options(cpp_game PRIVATE /Wall)
else()
    # gcc and clang
    target_compile_options(cpp_game PRIVATE -Wall -Wextra -pedantic -Wno-missing-field-initializers)
endif()
