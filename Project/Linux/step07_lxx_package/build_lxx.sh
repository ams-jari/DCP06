#!/usr/bin/env bash
# Step 7 — stage DCP06 plugin assets, run MkEdit, produce DCP06.lxx (dongle) or dev-install to Linux Captivate.
#
# Usage:
#   build_lxx.sh              # stage + try .lxx + dev install
#   build_lxx.sh package      # stage + MkEdit -C only (.lxx; needs Leica dongle)
#   build_lxx.sh install      # stage + MkEdit -I dev install (no dongle for PObs)
set -euo pipefail

MODE="${1:-all}"

if [[ -f "${HOME}/.bashrc" ]]; then
    # shellcheck disable=SC1090
    source "${HOME}/.bashrc"
fi

: "${DCP06_ROOT:=/mnt/c/Users/dell/Desktop/AMS/Development/DCP06}"
: "${STEP05_SO:=${HOME}/build/dcp06-step05/DCP06.so}"
: "${MKEDIT:=/mnt/c/Users/dell/Desktop/AMS/Development/Captivate_PluginSDK_x86_64_v10.00_RC/SWXIII_PluginDevKit_x64/Tools/MKTools/MkEdit.exe}"
: "${CAPTIVATE_LINUX_INSTALL_ROOT:=/home/gui-app/captivate/internal-storage/System}"

PROJECT_DIR="${DCP06_ROOT}/Project"
STAGING="${PROJECT_DIR}/Linux/step07_lxx_package/out/RelWithDebInfo"
MKEDIT_XML_REL="x86_64-ubuntu_22.04-gcc11\\Config\\DCP06_RelWithDebInfo.xml"
LEN_FILE="${DCP06_ROOT}/Text/Languages/en/DCP06.LEN"
PLUGIN_DIR="${CAPTIVATE_LINUX_INSTALL_ROOT}/Plugin/DCP06"

stage_assets() {
    echo "=== Step 7: staging assets ==="
    echo "DCP06_ROOT: ${DCP06_ROOT}"
    echo "Staging:    ${STAGING}"

    if [[ ! -f "${STEP05_SO}" ]]; then
        echo "ERROR: ${STEP05_SO} not found. Run step05 first:"
        echo "  bash \"${DCP06_ROOT}/Project/Linux/step05_full_plugin/build_wsl.sh\""
        exit 1
    fi

    if [[ ! -f "${LEN_FILE}" ]]; then
        echo "DCP06.LEN missing — running build_lang.bat (TextTool on Windows)..."
        cmd.exe /c "cd /d C:\\Users\\dell\\Desktop\\AMS\\Development\\DCP06\\scripts && build_lang.bat"
        if [[ ! -f "${LEN_FILE}" ]]; then
            echo "ERROR: DCP06.LEN still missing after build_lang.bat"
            exit 1
        fi
    fi

    mkdir -p "${STAGING}/SWXRes"
    rm -f "${STAGING}/libDCP06.so" "${STAGING}/DCP06.lxx" "${STAGING}/"*.pob

    cp -f "${STEP05_SO}" "${STAGING}/libDCP06.so"
    cp -f "${DCP06_ROOT}/SWXRes/DCP06_logo_1X.png" "${STAGING}/SWXRes/DCP06_logo_1X.png"
    cp -f "${DCP06_ROOT}/SWXRes/DCP06_LOGO_2X.png" "${STAGING}/SWXRes/DCP06_logo_2X.png"

    echo "Staged libDCP06.so ($(file -b "${STAGING}/libDCP06.so"))"
}

run_mkedit() {
    local extra_args="$1"
    local MKEDIT_EXE_WIN
    local PROJECT_DIR_WIN
    local INSTALL_WIN=""

    MKEDIT_EXE_WIN=$(wslpath -w "${MKEDIT}")
    PROJECT_DIR_WIN=$(wslpath -w "${PROJECT_DIR}")

    if [[ "${extra_args}" == *"-I:"* ]]; then
        INSTALL_WIN=$(wslpath -w "${CAPTIVATE_LINUX_INSTALL_ROOT}")
        extra_args="-I:${INSTALL_WIN}"
    fi

    echo "=== MkEdit ${extra_args} ==="
    cmd.exe /C "cd /d ${PROJECT_DIR_WIN} && ${MKEDIT_EXE_WIN} -M:mk -F:${MKEDIT_XML_REL} ${extra_args} -C"
}

post_install_fixups() {
    echo "=== Post-install fixups (resources + language + .app rename) ==="
    mkdir -p "${PLUGIN_DIR}/EN" "${PLUGIN_DIR}/SWXRes"

    if [[ -f "${PLUGIN_DIR}/libDCP06.app" ]]; then
        if [[ -f "${PLUGIN_DIR}/DCP06.app" ]]; then
            rm -f "${PLUGIN_DIR}/libDCP06.app"
        else
            mv "${PLUGIN_DIR}/libDCP06.app" "${PLUGIN_DIR}/DCP06.app"
        fi
    fi

    cp -f "${LEN_FILE}" "${PLUGIN_DIR}/EN/DCP06.LEN"
    cp -f "${STAGING}/SWXRes/"*.png "${PLUGIN_DIR}/SWXRes/"

    echo "Installed plugin tree:"
    ls -laR "${PLUGIN_DIR}/"
    echo ""
    echo "Restart Captivate (Ctrl+C the running session, then run 'captivate' again) to pick up DCP06."
}

try_package() {
    echo "=== MkEdit package (.lxx) — requires Leica dongle ==="
    if run_mkedit ""; then
        if [[ -f "${STAGING}/DCP06.lxx" ]]; then
            echo "DCP06.lxx created: ${STAGING}/DCP06.lxx"
            ls -la "${STAGING}/DCP06.lxx"
            return 0
        fi
    fi
    echo "NOTE: .lxx packaging failed (expected without dongle). PObs in staging are still valid."
    echo "      Re-run on a dongled PC, or ask Pasi to run MkEdit -C with the same staged files."
    return 1
}

try_install() {
    echo "=== MkEdit dev install (no dongle for PObs) ==="
    echo "WARNING: Dev install is experimental — a broken plugin under Plugin/DCP06/"
    echo "         will crash Captivate on startup. See Docs/DCP06_POSIX_Linux_Captivate_Sim.md"
    run_mkedit "-I:placeholder" || true
    post_install_fixups
}

stage_assets

case "${MODE}" in
    package)
        try_package
        ;;
    install)
        try_install
        ;;
    all)
        try_package || true
        try_install
        ;;
    *)
        echo "Usage: $0 [all|package|install]"
        exit 1
        ;;
esac

echo "=== Step 7 finished ==="
