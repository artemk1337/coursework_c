project(lem-in)

FILE(GLOB SOURCES src/*.cpp)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    sub::lib1
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ./inc
)
