cmake_minimum_required(VERSION 3.16.1)

project(ValidateTestObjects VERSION 1.0.0.0)

configure_file(testObjectHeadersVer.h.in testObjectHeadersVer.h)

set (SRCS
    testObjectHeaders.cpp
)

set (HDRS
    ${PROJECT_BINARY_DIR}/testObjectHeadersVer.h
)

set (CMN_HDRS
    ${Common_HEADERS}NaM/identifiable.h
    ${Common_HEADERS}NaM/testUtils.h
    ${Common_HEADERS}NaM/TestObjects/testObjects.h
)

set (TESTOBJ_HDRS
    ${Common_HEADERS}NaM/TestObjects/dummyData.h
    ${Common_HEADERS}NaM/TestObjects/statBase.h
    ${Common_HEADERS}NaM/TestObjects/testPoint2D.h
)

source_group("Headers" FILES ${HDRS})
source_group("Common" FILES )
source_group("Common/Headers" FILES ${CMN_HDRS})
source_group("Common/Headers/TestObjects" FILES ${TESTOBJ_HDRS})
source_group("Sources" FILES ${SRCS})

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS} ${CMN_HDRS} ${TESTOBJ_HDRS})

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

