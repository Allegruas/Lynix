cmake_minimum_required (VERSION 3.8)

project(Main)

add_executable(Main "src/main.c" "src/main.h")


set_target_properties(Main PROPERTIES OUTPUT_NAME "Lynix")

target_link_libraries(Main 
    PRIVATE UTILS Compiler Lyson
)

target_include_directories(Main PRIVATE
    ${CMAKE_SOURCE_DIR}/project/includes
)


install(TARGETS Main
    RUNTIME DESTINATION bin
)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/project/lynix/module
    DESTINATION modules
)

