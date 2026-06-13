cmake_minimum_required(VERSION 3.16)
project(dcp06_posix_step04_dcp06_logger CXX)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/CaptivatePosixSdk.cmake)

set(DCP06_LOGGER_SOURCES
    "${DCP06_ROOT}/src/core/Logger.cpp"
)

add_executable(dcp06_logger_smoke
    logger_smoke_main.cpp
    ${DCP06_LOGGER_SOURCES}
)

dcp06_apply_posix_target_settings(dcp06_logger_smoke)
