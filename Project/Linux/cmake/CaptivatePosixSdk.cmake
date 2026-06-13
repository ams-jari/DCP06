# Captivate POSIX SDK paths and libraries for DCP06 Linux builds.
# Include from Project/Linux/step*/CMakeLists.txt after setting CAPTIVATE_POSIX_* env vars.

if(NOT DEFINED ENV{CAPTIVATE_POSIX_SDK})
    message(FATAL_ERROR "CAPTIVATE_POSIX_SDK is not set. Run: source ~/.bashrc")
endif()
if(DEFINED ENV{CAPTIVATE_POSIX_ARM_LIBS})
    set(DCP06_POSIX_TARGET "arm")
    set(CAPTIVATE_POSIX_LIBS "$ENV{CAPTIVATE_POSIX_ARM_LIBS}")
elseif(DEFINED ENV{CAPTIVATE_POSIX_LIBS})
    set(DCP06_POSIX_TARGET "x86")
    set(CAPTIVATE_POSIX_LIBS "$ENV{CAPTIVATE_POSIX_LIBS}")
else()
    message(FATAL_ERROR "CAPTIVATE_POSIX_LIBS or CAPTIVATE_POSIX_ARM_LIBS must be set. Run: source ~/.bashrc")
endif()

set(CAPTIVATE_POSIX_SDK "$ENV{CAPTIVATE_POSIX_SDK}")

if(NOT DEFINED ENV{DCP06_ROOT})
    get_filename_component(DCP06_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../.." ABSOLUTE)
else()
    set(DCP06_ROOT "$ENV{DCP06_ROOT}")
endif()

if(DCP06_POSIX_TARGET STREQUAL "arm")
    # TS20 device ABI — matches DCP06 WinCE ARM config without Windows CE macros
    set(CAPTIVATE_POSIX_COMPILE_DEFS
        HW_ARM
        PLAT_WINCE_ARM
        NDEBUG
        DCP_USE_JSON_DATABASE=1
        DCP_SKIP_FILE_TYPE_MENU
    )
else()
    set(CAPTIVATE_POSIX_COMPILE_DEFS
        PLAT_WINPC_X86
        HW_X86
        NDEBUG
        DCP_USE_JSON_DATABASE=1
        DCP_SKIP_FILE_TYPE_MENU
    )
endif()

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
    "${CAPTIVATE_POSIX_SDK}/Projects/CameraControl/Source/API_Hdr"
    "${CAPTIVATE_POSIX_SDK}/Projects/Converter/Source/API_Hdr"
    "${CAPTIVATE_POSIX_SDK}/Projects/Geometry/Source/API_Hdr"
)

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
    "${CAPTIVATE_POSIX_LIBS}/libCaptivateRoot.so.10.00.0"
    "${CAPTIVATE_POSIX_LIBS}/libSwxAssertHandler.so"
    "${CAPTIVATE_POSIX_LIBS}/libboost_filesystem.so.1.81.0"
    "${CAPTIVATE_POSIX_LIBS}/libboost_system.so.1.81.0"
)

set(DCP06_COMMON_INCLUDE_DIRS
    "${DCP06_ROOT}/include"
    "${DCP06_ROOT}/include/dcp06/math"
    "${DCP06_ROOT}/src/core"
    "${DCP06_ROOT}/Text"
    "${DCP06_ROOT}/3rdparty/jsoncpp_0107"
    "${DCP06_ROOT}/3rdparty/eigen-3.2.10"
)

function(dcp06_apply_posix_target_settings target)
    target_include_directories(${target} PRIVATE
        ${CAPTIVATE_SDK_INCLUDE_DIRS}
        ${DCP06_COMMON_INCLUDE_DIRS}
    )
    target_compile_definitions(${target} PRIVATE ${CAPTIVATE_POSIX_COMPILE_DEFS})
    target_compile_features(${target} PRIVATE cxx_std_11)
    target_compile_options(${target} PRIVATE -Wall -Wno-deprecated-declarations -fPIC)
endfunction()

function(dcp06_link_captivate_sdk target)
    target_link_options(${target} PRIVATE -Wl,--no-as-needed)
    target_link_libraries(${target} PRIVATE ${CAPTIVATE_SDK_LIBS})
endfunction()

message(STATUS "DCP06_ROOT=${DCP06_ROOT}")
message(STATUS "CAPTIVATE_POSIX_SDK=${CAPTIVATE_POSIX_SDK}")
message(STATUS "DCP06_POSIX_TARGET=${DCP06_POSIX_TARGET}")
message(STATUS "CAPTIVATE_POSIX_LIBS=${CAPTIVATE_POSIX_LIBS}")
