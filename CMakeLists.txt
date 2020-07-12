cmake_minimum_required (VERSION 3.0)

project(manifesto VERSION 0.1 LANGUAGES C CXX)

add_subdirectory (src)

enable_testing()
add_subdirectory (test)
