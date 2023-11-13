cmake_minimum_required(VERSION 3.26)
project(5600_vvm)

set(CMAKE_CXX_STANDARD 17)

add_executable(5600_vvm main.cpp)
add_executable(PTETests test/PTETests.cpp)
add_executable(pageTableTests test/PageTableTests.cpp)
add_executable(memoryTests test/testMemory.cpp)
