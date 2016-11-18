##
# lib/TestRunner/CMakeLists.txt
#

# Add a library that handles the actual running of tests.
add_library(TestRunner TestRunner.c++)
target_link_libraries(TestRunner PUBLIC GMock)
