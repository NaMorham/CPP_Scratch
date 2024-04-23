cmake_minimum_required(VERSION 3.16.1)

project(UniqueIdTest VERSION 1.0.0)

add_executable(${PROJECT_NAME} uniqueTest.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
   VS_STARTUP_PROJECT ${PROJECT_NAME}
   CXX_STANDARD 20
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/bin)
