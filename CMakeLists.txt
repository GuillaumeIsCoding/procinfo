cmake_minimum_required(VERSION 3.31)

project(ProcInfo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

option(BUILD_SANDBOX "Enable Build sandbox" OFF)

file(GLOB_RECURSE HEADER_FILE
    "${INCLUDE_DIR}/*.hpp"
)

file(GLOB_RECURSE SOURCE_FILE
    "${SOURCE_DIR}/*.cpp"
)

add_library(ProcInfoLib STATIC ${SOURCE_FILE} ${HEADER_FILE})

target_include_directories(ProcInfoLib
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (WIN32)
    target_link_libraries(ProcInfoLib psapi.lib kernel32.lib)
endif ()

if (BUILD_SANDBOX)
    add_executable(ProcInfo sandbox/main.cpp)

    target_link_libraries(ProcInfo PRIVATE ProcInfoLib)
endif ()
