cmake_minimum_required(VERSION 3.8)
set (CMAKE_CXX_STANDARD 11)
find_package(OpenSSL)
project(imgurfetch)

file(GLOB SOURCES "src/*.cpp")
include_directories(src)

add_executable(imgurfetch ${SOURCES})
