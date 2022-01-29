cmake_minimum_required(VERSION 3.0.0)
project(Lang VERSION 0.1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(languageName "gx")

add_executable(${languageName} ${languageName}.c)

target_include_directories(${languageName}
    PRIVATE
        src
)

target_sources(${languageName}
    PRIVATE
        src/darray.h
        src/lexer.c
        src/parser.c
        src/str.c
)