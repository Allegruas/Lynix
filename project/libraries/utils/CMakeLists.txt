cmake_minimum_required (VERSION 3.8)

project(UTILS)

add_library(UTILS SHARED "src/type/wstring.c"  "src/tool/stringbuf.c" "src/exception.c" "src/tool/file.c" "src/type/string.c")


set_target_properties(UTILS PROPERTIES
    C_VISIBILITY_PRESET hidden
    DEFINE_SYMBOL "LYNIX_EXPORTS"
    OUTPUT_NAME "lyutils"     
)


target_include_directories(UTILS PRIVATE
    ${CMAKE_SOURCE_DIR}/project/includes
)

install(TARGETS UTILS
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION libs
    ARCHIVE DESTINATION libs
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/project/includes
    DESTINATION ${CMAKE_INSTALL_PREFIX}
)