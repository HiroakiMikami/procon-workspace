cmake_minimum_required(VERSION 3.7)
project(procon_workspace)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -D_GLIBCXX_DEBUG -g -D__LOCAL__")
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(./procon-utils)
add_subdirectory(./src)
