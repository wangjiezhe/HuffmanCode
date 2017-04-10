cmake_minimum_required(VERSION 3.7)
project(HuffmanCode)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GLIB REQUIRED glib-2.0)
if(GLIB_FOUND)
  include_directories (${GLIB_INCLUDE_DIRS})
  link_directories (${GLIB_LIBRARY_DIRS})
  add_definitions (${GLIB_CFLAGS_OTHER})
else(GLIB_FOUND)
  message(FATAL_ERROR "glib-2.0 not found!")
endif(GLIB_FOUND)

set(SOURCE_FILES
        huffman.c
        huffman.h
        main.c)

add_executable(HuffmanCode ${SOURCE_FILES})

target_link_libraries (HuffmanCode ${GLIB_LIBRARIES})
