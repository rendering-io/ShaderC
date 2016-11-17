# Set the minimum required CMake version.
cmake_minimum_required(VERSION 3.2)

# Set global compiler options.
add_compile_options(-std=c++14)

# Add libraries subdirectory.
add_subdirectory(lib)

# Add tools subdirectory.
