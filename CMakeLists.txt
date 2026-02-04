cmake_minimum_required(VERSION 3.15)
project(raytracinginaweekend CXX)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(stb REQUIRED)

set(CMAKE_SKIP_RPATH TRUE)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} fmt::fmt spdlog::spdlog stb::stb)
