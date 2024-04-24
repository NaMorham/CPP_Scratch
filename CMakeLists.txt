cmake_minimum_required(VERSION 3.16.1)

project(CppScratch VERSION 1.2.0.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file (GLOB Common_HEADERS ${CMAKE_SOURCE_DIR}/common/include/*.h)
file (GLOB Common_SOURCES ${CMAKE_SOURCE_DIR}/common/src/*.cpp)
file (GLOB Common_LIBS ${CMAKE_SOURCE_DIR}/common/lib/*.cpp)

add_subdirectory("Projects")
