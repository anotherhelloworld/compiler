cmake_minimum_required(VERSION 3.6)
project(Compiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Scanner.cpp Scanner.h Parser.cpp Parser.h Errors.h)
add_executable(Compiler ${SOURCE_FILES} Parser.cpp Parser.h Errors.h)