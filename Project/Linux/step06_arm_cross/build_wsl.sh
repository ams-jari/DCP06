#!/usr/bin/env bash
set -euo pipefail

if [[ -f "${HOME}/.bashrc" ]]; then
    # shellcheck disable=SC1090
    source "${HOME}/.bashrc"
fi

YOCTO_ENV="/usr/local/leicasdk-x86_64/environment-setup-cortexa53-crypto-leicageo-linux"
TOOLCHAIN_FILE="/usr/local/leicasdk-x86_64/sysroots/x86_64-leicasdk-linux/usr/share/cmake/cortexa53-crypto-leicageo-linux-toolchain.cmake"

if [[ ! -f "${YOCTO_ENV}" ]]; then
    echo "Yocto SDK not installed. Run Stage 2 in Docs/DCP06_POSIX_WSL_Setup.md first." >&2
    exit 1
fi

# shellcheck disable=SC1090
source "${YOCTO_ENV}"

: "${DCP06_ROOT:=/mnt/c/Users/dell/Desktop/AMS/Development/DCP06}"
: "${CAPTIVATE_POSIX_SDK:=/mnt/c/Users/dell/Desktop/AMS/Development/Captivate_PluginSDK_POSIX_v10.0.0-rc.309/Captivate_PluginSDK_POSIX_v10.0.0-rc.309}"
: "${CAPTIVATE_POSIX_ARM_LIBS:=${CAPTIVATE_POSIX_SDK}/Binary/armv8-leicageo_linux_5.0-gcc13/libs}"
export DCP06_ROOT CAPTIVATE_POSIX_SDK CAPTIVATE_POSIX_ARM_LIBS

BUILD_DIR="${HOME}/build/dcp06-step06"
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"
rm -rf ./*

cmake \
    -DCMAKE_TOOLCHAIN_FILE="${TOOLCHAIN_FILE}" \
    "${DCP06_ROOT}/Project/Linux/step06_arm_cross"
cmake --build . -j"$(nproc)"

file DCP06.so
aarch64-leicageo-linux-nm -D DCP06.so | grep Start15751

echo "Step 6 ARM cross-build OK"
