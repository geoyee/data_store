cmake_minimum_required(VERSION 3.15)

project(example)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-DMSGPACK_NO_BOOST)

if (NOT MSVC)
    find_program(CCACHE_PROGRAM ccache)
    if (CCACHE_PROGRAM)
        message(STATUS "[${PROJECT_NAME}] Found CCache: ${CCACHE_PROGRAM}")
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PROGRAM})
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_PROGRAM})
    endif()
endif()

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(DEPEND_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

file(GLOB CURRENT_HEADER ${ROOT_DIR}/include/*)
message("[${PROJECT_NAME}] Current HEADER: ${CURRENT_HEADER}")

add_executable(${PROJECT_NAME} main.cpp ${CURRENT_HEADER})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${ROOT_DIR}/include
    ${DEPEND_DIR}/msgpack-cxx-611
)