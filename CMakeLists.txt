cmake_minimum_required(VERSION 3.8)
set (CMAKE_CXX_STANDARD 11)
project(imgurfetch)

# Find and include all source .cpp files
file(GLOB SOURCES "src/*.cpp")
include_directories(src)

add_executable(imgurfetch ${SOURCES})

# Find and use threads library
find_package(Threads)
target_link_libraries(imgurfetch ${CMAKE_THREAD_LIBS_INIT})

# Find and use Openssl library
find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIRS})
    target_link_libraries(imgurfetch ${OPENSSL_LIBRARIES})
endif(OPENSSL_FOUND)
