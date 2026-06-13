cmake_minimum_required(VERSION 3.16)
project(dcp06_posix_step02_link_smoke CXX)

if(NOT DEFINED ENV{CAPTIVATE_POSIX_SDK})
    message(FATAL_ERROR "CAPTIVATE_POSIX_SDK is not set. Run: source ~/.bashrc")
endif()
if(NOT DEFINED ENV{CAPTIVATE_POSIX_LIBS})
    message(FATAL_ERROR "CAPTIVATE_POSIX_LIBS is not set. Run: source ~/.bashrc")
endif()

set(CAPTIVATE_POSIX_SDK "$ENV{CAPTIVATE_POSIX_SDK}")
set(CAPTIVATE_POSIX_LIBS "$ENV{CAPTIVATE_POSIX_LIBS}")

# x86_64 Ubuntu dev/sim — matches SensorData header fallback until Leica documents Linux PLAT_* defines
set(CAPTIVATE_POSIX_COMPILE_DEFS
    PLAT_WINPC_X86
    HW_X86
)

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
    "${CAPTIVATE_POSIX_SDK}/Projects/SwxAssertHandler/include"
)

# Same link set as DCP06-VS2008 Release|Win32 (versioned .so paths — no libfoo.so symlinks in SDK)
set(CAPTIVATE_SDK_LIBS
    "${CAPTIVATE_POSIX_LIBS}/libABL_Base.so.10.00.0"
    "${CAPTIVATE_POSIX_LIBS}/libABL_Tps.so.10.00.0"
    "${CAPTIVATE_POSIX_LIBS}/libGuiPlus.so.10.00.0"
    "${CAPTIVATE_POSIX_LIBS}/libHALTools.so.10.00.0"
    "${CAPTIVATE_POSIX_LIBS}/libScanData.so.1.2.0"
    "${CAPTIVATE_POSIX_LIBS}/libSensorData.so.2.13.0"
    "${CAPTIVATE_POSIX_LIBS}/libUtilities.so.10.00.0"
    "${CAPTIVATE_POSIX_LIBS}/libGSV.so.10.00.0"
    "${CAPTIVATE_POSIX_LIBS}/libGSV_Base.so.10.00.0"
    "${CAPTIVATE_POSIX_LIBS}/libABL_Manage.so.10.00.0"
    "${CAPTIVATE_POSIX_LIBS}/libABL_Scanning.so.10.00.0"
    "${CAPTIVATE_POSIX_LIBS}/libABL_System.so.10.00.0"
    "${CAPTIVATE_POSIX_LIBS}/libTpsFacade.so.10.00.0"
    "${CAPTIVATE_POSIX_LIBS}/libCommon.so.10.00.0"
    "${CAPTIVATE_POSIX_LIBS}/libGeoMath.so.7.2.7"
    "${CAPTIVATE_POSIX_LIBS}/libMath.so.10.00.0"
    "${CAPTIVATE_POSIX_LIBS}/libSLBasics.so.10.00.0"
)

add_library(link_smoke SHARED link_smoke.cpp)
target_include_directories(link_smoke PRIVATE ${CAPTIVATE_SDK_INCLUDE_DIRS})
target_compile_definitions(link_smoke PRIVATE ${CAPTIVATE_POSIX_COMPILE_DEFS})
target_compile_features(link_smoke PRIVATE cxx_std_11)
target_compile_options(link_smoke PRIVATE -Wall -Wextra -fPIC)
target_link_options(link_smoke PRIVATE -Wl,--no-as-needed)
target_link_libraries(link_smoke PRIVATE ${CAPTIVATE_SDK_LIBS})

message(STATUS "CAPTIVATE_POSIX_SDK=${CAPTIVATE_POSIX_SDK}")
message(STATUS "CAPTIVATE_POSIX_LIBS=${CAPTIVATE_POSIX_LIBS}")
