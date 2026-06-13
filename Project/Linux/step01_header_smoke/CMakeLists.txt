cmake_minimum_required(VERSION 3.16)
project(dcp06_posix_step01_header_smoke CXX)

if(NOT DEFINED ENV{CAPTIVATE_POSIX_SDK})
    message(FATAL_ERROR "CAPTIVATE_POSIX_SDK is not set. Run: source ~/.bashrc")
endif()

set(CAPTIVATE_POSIX_SDK "$ENV{CAPTIVATE_POSIX_SDK}")

# Same API_Hdr paths as DCP06-VS2008.vcproj (Win32), mapped to POSIX SDK Projects/
set(CAPTIVATE_SDK_INCLUDE_DIRS
    "${CAPTIVATE_POSIX_SDK}/Projects"
    "${CAPTIVATE_POSIX_SDK}/Projects/3rdParty"
    "${CAPTIVATE_POSIX_SDK}/Projects/ABL_Base/Source/API_Hdr"
    "${CAPTIVATE_POSIX_SDK}/Projects/ABL_Manage/Source/API_Hdr"
    "${CAPTIVATE_POSIX_SDK}/Projects/ABL_System/Source/API_Hdr"
    "${CAPTIVATE_POSIX_SDK}/Projects/ABL_Scanning/Source/API_Hdr"
    "${CAPTIVATE_POSIX_SDK}/Projects/ABL_Tps/Source/API_Hdr"
    "${CAPTIVATE_POSIX_SDK}/Projects/Common/Source/API_Hdr"
    "${CAPTIVATE_POSIX_SDK}/Projects/Database/Ext_Hdr/apisupport"
    "${CAPTIVATE_POSIX_SDK}/Projects/GeoMath/Source/API_Hdr"
    "${CAPTIVATE_POSIX_SDK}/Projects/GuiPlus/Source/API_Hdr"
    "${CAPTIVATE_POSIX_SDK}/Projects/HALTools/Source/API_Hdr"
    "${CAPTIVATE_POSIX_SDK}/Projects/ScanData/Source/API_Hdr"
    "${CAPTIVATE_POSIX_SDK}/Projects/SensorData/Source/API_Hdr"
    "${CAPTIVATE_POSIX_SDK}/Projects/TpsFacade/Source/API_Hdr"
    "${CAPTIVATE_POSIX_SDK}/Projects/Utilities/Source/API_Hdr"
    "${CAPTIVATE_POSIX_SDK}/Projects/GSV/Source/API_Hdr"
    "${CAPTIVATE_POSIX_SDK}/Projects/GSV_Base/Source/API_Hdr"
    "${CAPTIVATE_POSIX_SDK}/Projects/SLBasics/Source/API_Hdr"
)

add_executable(header_smoke test_include.cpp)
target_include_directories(header_smoke PRIVATE ${CAPTIVATE_SDK_INCLUDE_DIRS})
target_compile_features(header_smoke PRIVATE cxx_std_11)

# Match Linux plugin builds later
target_compile_options(header_smoke PRIVATE -Wall -Wextra -fPIC)

message(STATUS "CAPTIVATE_POSIX_SDK=${CAPTIVATE_POSIX_SDK}")
