cmake_minimum_required (VERSION 3.8)

project(Assembler)

add_library(Assembler SHARED "src/parser.c")


set_target_properties(Assembler PROPERTIES OUTPUT_NAME "lyasm")

target_link_libraries(Assembler 
    PRIVATE UTILS Compiler Lyson
)

target_include_directories(Assembler PRIVATE
    ${CMAKE_SOURCE_DIR}/project/includes
)


install(TARGETS Assembler
    RUNTIME DESTINATION bin
)

