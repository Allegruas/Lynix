cmake_minimum_required (VERSION 3.8)

project("Lynix")

#标准设定
set(CMAKE_C_STANDARD 23)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_subdirectory("project/libraries/utils")
add_subdirectory("project/libraries/compiler")
add_subdirectory("project/libraries/lyson")
add_subdirectory("project/lynix")

add_dependencies(Main UTILS Compiler)