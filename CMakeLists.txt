cmake_minimum_required(VERSION 3.22)
project(scan_util)

set(CMAKE_CXX_STANDARD 17)

add_executable(scan_util main.cpp AhoCorasic.cpp AhoCorasic.h)
