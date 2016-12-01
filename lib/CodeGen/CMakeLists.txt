# Build the CodeGen library.

# Find LLVM.
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(LLVM_LIBS support core irreader)

add_library(CodeGen CodeGen.c++)
target_compile_definitions(CodeGen PRIVATE ${LLVM_DEFINITIONS})
target_include_directories(CodeGen PRIVATE ${LLVM_INCLUDE_DIRS})
target_link_libraries(CodeGen PUBLIC ${LLVM_LIBS})
