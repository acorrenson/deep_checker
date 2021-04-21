cmake_minimum_required(VERSION 3.0)

project(deep_checker)

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_C_FLAGS "-Werror -Wall -Wextra -g -Wmissing-declarations -Wno-unused-parameter ${CMAKE_C_FLAGS}")

add_library(game
    game.c game.h
    utils.c utils.h
    vector.c vector.h
)

add_executable(simulator simulator.c)
target_link_libraries(simulator game)

add_executable(interactive interactive.c)
target_link_libraries(interactive game)

file(COPY ../statistics/utils.py DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ../statistics/knn.py DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ../statistics/knn.save DESTINATION ${CMAKE_BINARY_DIR})
