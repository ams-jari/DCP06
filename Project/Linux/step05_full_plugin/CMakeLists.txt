cmake_minimum_required(VERSION 3.16)
project(dcp06_posix_step05_full_plugin CXX)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/CaptivatePosixSdk.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../cmake/Dcp06Sources.cmake)

list(APPEND DCP06_SOURCES ${DCP06_VCPROJ_SOURCES})

list(LENGTH DCP06_SOURCES DCP06_SOURCE_COUNT)
message(STATUS "DCP06 source files: ${DCP06_SOURCE_COUNT}")

add_library(DCP06 SHARED ${DCP06_SOURCES})
set_target_properties(DCP06 PROPERTIES
    OUTPUT_NAME "DCP06"
    PREFIX ""
)

dcp06_apply_posix_target_settings(DCP06)
dcp06_link_captivate_sdk(DCP06)
