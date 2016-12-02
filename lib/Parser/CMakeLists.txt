##
# lib/Parser/CMakeLists.txt
#
add_library(Parser Parser.c++)
target_link_libraries(Parser PUBLIC AST)