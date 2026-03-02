cmake_minimum_required(VERSION 3.15)
project(DCP06_TestBuild LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

# Use vcpkg toolchain only if explicitly provided and file exists.
# Run: cmake -DCMAKE_TOOLCHAIN_FILE=<your-vcpkg>/scripts/buildsystems/vcpkg.cmake ..
# Or if vcpkg is integrated: cmake --preset=default (see CMakeUserPresets.json)

# Output static library - no executable, no SDK linking
add_library(dcp06_test STATIC)

# Source files (match main project)
file(GLOB_RECURSE DCP06_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp"
)
# Exclude backup/test folders if any
list(FILTER DCP06_SOURCES EXCLUDE REGEX ".*[Bb]ackup.*")
list(FILTER DCP06_SOURCES EXCLUDE REGEX ".*/test/.*")

target_sources(dcp06_test PRIVATE ${DCP06_SOURCES})

# Include directories - NO SYSTEM1500_ROOT, use stubs
target_include_directories(dcp06_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/dcp06
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/dcp06/math
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/geometry
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/core
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/sdk_fallback
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/../Text
)

# vcpkg: eigen3, boost (optional if using vcpkg toolchain)
find_package(Eigen3 CONFIG QUIET)
find_package(Boost QUIET)
if(Eigen3_FOUND)
  target_link_libraries(dcp06_test PRIVATE Eigen3::Eigen)
else()
  # Fallback: add vcpkg default include
  target_include_directories(dcp06_test PRIVATE $ENV{VCPKG_ROOT}/installed/x86-windows/include
    $ENV{VCPKG_ROOT}/installed/x64-windows/include)
endif()
if(Boost_FOUND)
  target_link_libraries(dcp06_test PRIVATE Boost::filesystem)
endif()

# Preprocessor defines (match main project, no PILING_EXPORTS - we're not a DLL)
target_compile_definitions(dcp06_test PRIVATE
    HW_X86
    OS_WINPC
    PLAT_WINPC_X86
    WIN32
    NDEBUG
    _WINDOWS
    _CRT_SECURE_NO_WARNINGS
    DCP06_NO_SDK  # Optional: for future #ifdef of SDK-only code
)

# MSVC settings
if(MSVC)
    target_compile_options(dcp06_test PRIVATE /W3)
endif()

# Win32
if(WIN32)
    target_compile_definitions(dcp06_test PRIVATE _WIN32)
endif()
