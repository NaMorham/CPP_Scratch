cmake_minimum_required(VERSION 3.16.1)

project(CustomStackWithPointers VERSION 1.0.0.0)

configure_file(customStackVer.h.in customStackVer.h)

set (SRCS customStack.cpp)
set (HDRS ${PROJECT_BINARY_DIR}/customStackVer.h)

source_group("Headers" FILES ${HDRS})
source_group("Sources" FILES ${SRCS})

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS})

#message("${PROJECT_NAME}: \${Common_HEADERS} = ${Common_HEADERS}")
#message("${PROJECT_NAME}: \${Common_SOURCES} = ${Common_SOURCES}")
#message("${PROJECT_NAME}: \${Common_LIBS} = ${Common_LIBS}")

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES
   VS_STARTUP_PROJECT ${PROJECT_NAME}
   CXX_STANDARD 20
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/out/bin)
