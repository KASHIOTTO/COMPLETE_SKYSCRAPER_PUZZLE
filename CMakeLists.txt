cmake_minimum_required(VERSION 3.10)
project(hw1 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
include_directories(include)

add_executable(hw1_game src/hw1_game.c src/hw1.c)
target_compile_options(hw1_game PUBLIC -g -Wall -Wextra -Wshadow -Wpedantic -Wdouble-promotion -Wformat=2 -Wundef -Werror)
target_link_libraries(hw1_game PUBLIC m)

add_executable(hw1_solver src/hw1_solver.c src/hw1.c)
target_compile_options(hw1_solver PUBLIC -g -Wall -Wextra -Wshadow -Wpedantic -Wdouble-promotion -Wformat=2 -Wundef -Werror)
target_link_libraries(hw1_solver PUBLIC m)

add_executable(hw1_solver_tests tests/unit_tests.cpp src/hw1.c)
target_compile_options(hw1_solver_tests PUBLIC -g -Wall -Wextra -Wshadow -Wpedantic -Wdouble-promotion -Wformat=2 -Wundef -Werror)
target_include_directories(hw1_solver_tests PUBLIC include)
target_link_libraries(hw1_solver_tests PUBLIC gtest gtest_main pthread m)
