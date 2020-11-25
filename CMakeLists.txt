cmake_minimum_required (VERSION 3.10)

project (Lem-in VERSION 1.0)

include_directories (include)

file(GLOB SOURCES "src/*.cpp")
 
add_executable(lem-in ${SOURCES})

# add_subdirectory (src)
# add_executable (lem-in main.cpp)
