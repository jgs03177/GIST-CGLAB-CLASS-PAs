cmake_minimum_required(VERSION 3.13)

project(neon)

# set cmake global variables
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED YES)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

# import external libraries
add_subdirectory(extern)

set(NEON_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/src)

include_directories(src)
add_subdirectory(src)


# under construction
# include(CTest)
# enable_testing()
# add_subdirectory(test)
