cmake_minimum_required(VERSION 3.10)

# Set the project name
project (ft)

FILE(GLOB SOURCES src/*.cpp)

############################################################
# Create a library
############################################################

add_library(${PROJECT_NAME} ${SOURCES})
add_library(sub::lib1 ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/includes
)

