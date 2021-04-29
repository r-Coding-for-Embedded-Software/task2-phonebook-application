cmake_minimum_required(VERSION 3.17)
project(task2_phonebook C)

set(CMAKE_C_STANDARD 11)

add_executable(task2_phonebook main.c src/csv_helper.c libs/csv_helper.h libs/BasicTypes.h)