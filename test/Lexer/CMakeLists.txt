##
# test/Lexer/CMakeLists.txt
#

# Add a unit test executable for testing Lexer library.
add_executable(LexerTests LexerTests.c++)
target_link_libraries(LexerTests PUBLIC Lexer TestRunner)

# Register the test executable with ctest.
add_test(NAME Lexer COMMAND LexerTests)