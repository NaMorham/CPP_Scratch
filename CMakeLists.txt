cmake_minimum_required(VERSION 3.16.1)

project(CppScratch VERSION 1.2.0.1)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(Common_HEADERS ${CMAKE_SOURCE_DIR}/common/include/)
set(Common_SOURCES ${CMAKE_SOURCE_DIR}/common/src/)
set(Common_LIBS ${CMAKE_SOURCE_DIR}/common/lib/)

add_subdirectory("Projects")

