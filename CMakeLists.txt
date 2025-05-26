# CMake setup
cmake_minimum_required(VERSION 3.11)

# Project setup
project(ThreadPools VERSION 1.0)

# C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Libs and exe
add_subdirectory(External/tracy)
add_subdirectory(Source)
add_subdirectory(Source/Tests)