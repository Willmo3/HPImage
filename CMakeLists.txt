cmake_minimum_required(VERSION 3.27)
project(HPImage)

set(CMAKE_CXX_STANDARD 14)

add_library(HPImage STATIC
        hpimage.h
        hpimage.cpp
)
