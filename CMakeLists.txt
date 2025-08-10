cmake_minimum_required (VERSION 3.8)
project ("Daemon" C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MDd")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MD")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
add_subdirectory(${EXTERNAL_DIR}/SDL EXCLUDE_FROM_ALL)

include_directories(${EXTERNAL_DIR}/GLAD/include)
add_library(GLAD STATIC ${EXTERNAL_DIR}/GLAD/src/glad.c)

include_directories(${EXTERNAL_DIR})

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)

add_executable (Daemon ${SOURCES})

add_custom_command(
    TARGET Daemon POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:Daemon>/res"
)

target_link_libraries(Daemon PRIVATE SDL3::SDL3 GLAD)