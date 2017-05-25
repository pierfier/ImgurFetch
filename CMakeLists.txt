cmake_minimum_required(VERSION 3.8)
project(imgurfetch)

file(GLOB SOURCES "src/*.cpp")
include_directories(src)

add_executable(imgurfetch ${SOURCES})
