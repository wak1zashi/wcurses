cmake_minimum_required(VERSION 3.10)

project(wcurses VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES src/wcurses.cc)

if(WIN32)
  list(APPEND SOURCES
    src/buffer.cc
    src/color_manager.cc
    src/cursor.cc
    src/input_manager.cc
    src/terminal.cc
  )
endif()

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(MSVC)
  set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/lib/vc17)
elseif(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/lib/mingw)
elseif(UNIX AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/lib/gcc)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}/${CMAKE_BUILD_TYPE}
    DEBUG_POSTFIX "_d"
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
endif()