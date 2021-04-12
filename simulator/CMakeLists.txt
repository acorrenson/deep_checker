cmake_minimum_required(VERSION 3.0)

project(deep_checker)

set(CMAKE_C_FLAGS "-Werror -Wall -Wextra -g -Wmissing-declarations -Wno-unused-parameter ${CMAKE_C_FLAGS}")

add_executable(simulator
    simulator.c
    utils.c utils.h
    vector.c vector.h
)
