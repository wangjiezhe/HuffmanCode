cmake_minimum_required(VERSION 3.7)
project(HuffmanCode)

set(CMAKE_CXX_STANDARD 11)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK2 REQUIRED gtk+-2.0)
include_directories (${GTK2_INCLUDE_DIRS})
link_directories (${GTK2_LIBRARY_DIRS})

set(SOURCE_FILES
        huffman.c
        huffman.h
        main.c)

add_executable(HuffmanCode ${SOURCE_FILES})

add_definitions (${GTK2_CFLAGS_OTHER})
target_link_libraries (HuffmanCode ${GTK2_LIBRARIES})
