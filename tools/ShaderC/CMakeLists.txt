##
# tools/ShaderC/CMakeLists.txt
#

# Add ShaderC compiler executable.
add_executable(shaderc ShaderC.c++)
target_link_libraries(shaderc PUBLIC Lexer Parser)
