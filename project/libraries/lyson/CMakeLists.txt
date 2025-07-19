cmake_minimum_required (VERSION 3.8)

project(Lyson)

add_library(Lyson SHARED "src/lyson.c"  "src/parser.c" "src/builder.c")


set_target_properties(Lyson PROPERTIES
    C_VISIBILITY_PRESET hidden
    DEFINE_SYMBOL "LYNIX_EXPORTS"
    OUTPUT_NAME "lyson"     
)


target_include_directories(Lyson PRIVATE
    ${CMAKE_SOURCE_DIR}/project/includes
)

install(TARGETS Lyson
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION libs
    ARCHIVE DESTINATION libs
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/project/includes/lyson
    DESTINATION includes/lyson
)