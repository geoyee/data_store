cmake_minimum_required(VERSION 3.15)

project(example)

set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_definitions(-DMSGPACK_NO_BOOST)
add_definitions(-DMSGPACK_CXX17)

if(NOT MSVC)
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
        message(STATUS "[${PROJECT_NAME}] Found CCache: ${CCACHE_PROGRAM}")
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PROGRAM})
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_PROGRAM})
    endif()
endif()

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(DEPEND_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

find_package(Threads)

file(GLOB CURRENT_HEADER ${ROOT_DIR}/include/*)
message("[${PROJECT_NAME}] Current HEADER: ${CURRENT_HEADER}")

file(GLOB SQLITE_SRC ${DEPEND_DIR}/sqlite/src/*)
message("[${PROJECT_NAME}] SQLite SRC: ${SQLITE_SRC}")

add_executable(${PROJECT_NAME} main.cpp ${SQLITE_SRC} ${CURRENT_HEADER})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${ROOT_DIR}/include
    ${DEPEND_DIR}/msgpack-c/include
    ${DEPEND_DIR}/sqlite/include
)

target_link_libraries(${PROJECT_NAME}
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)
