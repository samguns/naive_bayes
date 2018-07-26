cmake_minimum_required(VERSION 3.12)
project(naive_bayes)

set(CMAKE_CXX_STANDARD 11)

add_executable(naive_bayes main.cpp classifier.cpp classifier.h)