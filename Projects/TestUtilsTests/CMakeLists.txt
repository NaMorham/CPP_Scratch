cmake_minimum_required(VERSION 3.16.1)

project(TestUtilsTests VERSION 1.0.0.0)

configure_file(testUtilsTestsVer.h.in testUtilsTestsVer.h)

set (SRCS
    testUtilsTests.cpp
)

set (HDRS
    ${PROJECT_BINARY_DIR}/testUtilsTestsVer.h
)

set (CMN_HDRS
    ${Common_HEADERS}NaM/testUtils.h
)

source_group("Headers" FILES ${HDRS})
source_group("Common" FILES )
source_group("Common/Headers" FILES ${CMN_HDRS})
source_group("Sources" FILES ${SRCS})

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS} ${CMN_HDRS})

#message("${PROJECT_NAME}: \${Common_HEADERS} = ${Common_HEADERS}")
#message("${PROJECT_NAME}: \${Common_SOURCES} = ${Common_SOURCES}")
#message("${PROJECT_NAME}: \${Common_LIBS} = ${Common_LIBS}")

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_BINARY_DIR}
    ${Common_HEADERS}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
   VS_STARTUP_PROJECT ${PROJECT_NAME}
   CXX_STANDARD 20
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/out/NaM/bin)
