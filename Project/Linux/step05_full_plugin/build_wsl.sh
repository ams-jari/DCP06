#!/usr/bin/env bash
set -euo pipefail

if [[ -f "${HOME}/.bashrc" ]]; then
    # shellcheck disable=SC1090
    source "${HOME}/.bashrc"
fi

: "${DCP06_ROOT:=/mnt/c/Users/dell/Desktop/AMS/Development/DCP06}"
: "${CAPTIVATE_POSIX_SDK:=/mnt/c/Users/dell/Desktop/AMS/Development/Captivate_PluginSDK_POSIX_v10.0.0-rc.309/Captivate_PluginSDK_POSIX_v10.0.0-rc.309}"
: "${CAPTIVATE_POSIX_LIBS:=${CAPTIVATE_POSIX_SDK}/Binary/x86_64-ubuntu_22.04-gcc11/libs}"
export DCP06_ROOT CAPTIVATE_POSIX_SDK CAPTIVATE_POSIX_LIBS

BUILD_DIR="${HOME}/build/dcp06-step05"
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"
rm -rf ./*

cmake "${DCP06_ROOT}/Project/Linux/step05_full_plugin"
cmake --build . -j"$(nproc)"

file DCP06.so
nm -D DCP06.so | grep Start15751

echo "Step 5 build OK"
