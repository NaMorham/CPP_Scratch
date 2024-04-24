cmake_minimum_required(VERSION 3.16.1)

project(CustomLinkedList VERSION 1.0.0.0)

configure_file(customLinkedListVer.h.in customLinkedListVer.h)

add_executable(${PROJECT_NAME} customLinkedList.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES
   VS_STARTUP_PROJECT ${PROJECT_NAME}
   CXX_STANDARD 20
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/out/bin)

