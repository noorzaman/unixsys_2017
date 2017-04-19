cmake_minimum_required(VERSION 3.6)
project(smsh)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c cfprocessor.c cfprocessor.h flexstr.c flexstr.h splitline.c splitline.h varlib.c varlib.h smsh5.c smsh.h ifstack.h)
add_executable(smsh ${SOURCE_FILES})