##
# test/Parser/CMakeLists.txt
#

# Add a unit test executable for testing Parser library.
add_executable(ParserTests ParserTests.c++)
target_link_libraries(ParserTests PUBLIC Parser TestRunner)

# Register the test executable with ctest.
add_test(NAME Parser COMMAND ParserTests)
