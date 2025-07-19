cmake_minimum_required (VERSION 3.8)

project(Compiler)

add_library(Compiler SHARED "src/tokenizer.c" "src/parser.c")


set_target_properties(Compiler PROPERTIES
    C_VISIBILITY_PRESET hidden
    DEFINE_SYMBOL "LYNIX_EXPORTS"
    OUTPUT_NAME "lycompiler" 
)


target_link_libraries(Compiler
    PRIVATE UTILS Lyson
)

target_include_directories(Compiler PRIVATE
    ${CMAKE_SOURCE_DIR}/project/includes
)


install(TARGETS Compiler
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION libs
    ARCHIVE DESTINATION libs
)

