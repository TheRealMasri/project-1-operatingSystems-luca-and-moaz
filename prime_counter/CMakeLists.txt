cmake_minimum_required(VERSION 4.0)
project(prime_counter C)

set(CMAKE_C_STANDARD 11)

add_library(prime_counter STATIC prime_counter.c)
