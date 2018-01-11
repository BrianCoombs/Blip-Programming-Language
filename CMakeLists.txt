cmake_minimum_required(VERSION 3.8)
project(Project_8_Interpreter_A)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Input.cpp
        Parse.h main.cpp main.h Interpreter.cpp Interpreter.h String.h ExpTree.cpp ExpTree.h)

add_executable(Project_8_Interpreter_A ${SOURCE_FILES})