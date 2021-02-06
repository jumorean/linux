cmake_minimum_required(VERSION 3.17)
project(linux C)

set(CMAKE_C_STANDARD 99)


add_subdirectory(IPC)
add_subdirectory(socket)

