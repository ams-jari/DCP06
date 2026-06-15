# DCP06 — POSIX Linux Build (Step by Step)

**Date:** 2026-06-13  
**Environment:** WSL2 + Ubuntu 22.04 (see [DCP06_POSIX_WSL_Setup.md](DCP06_POSIX_WSL_Setup.md))  
**SDK:** `Captivate_PluginSDK_POSIX_v10.0.0-rc.309`

### Why these steps exist

This is a **reconnaissance build path** in DCP06 to learn the Captivate POSIX SDK and surface Linux porting issues. **Production TS20 delivery is DCP05** (owned by Pasi Ojaniemi). Results here are meant to **de-risk and accelerate** that port — not to ship DCP06 to resellers. Background: [DCP06_POSIX_SDK_Analysis.md — Context & goals](DCP06_POSIX_SDK_Analysis.md#context--goals).

Work through these steps **in order**. Do not skip ahead until the current step passes.

---

## Roadmap

| Step | Folder | Goal | Status |
|------|--------|------|--------|
| **1** | `Project/Linux/step01_header_smoke/` | Compile a tiny program that includes Leica SDK headers | done |
| **2** | `Project/Linux/step02_link_smoke/` | Link a test `.so` against core SDK libraries | done |
| **3** | `Project/Linux/step03_min_plugin/` | Minimal plugin stub exporting `Start15751` | done |
| **4** | `Project/Linux/step04_dcp06_logger/` | Compile first real DCP06 source (`Logger.cpp`) | done |
| **5** | `Project/Linux/step05_full_plugin/` | Full `DCP06.so` (110 vcproj sources + SDK link) | done |
| **6** | `Project/Linux/step06_arm_cross/` | Cross-compile `DCP06.so` for TS20 ARM | done |
| **7** | `Project/Linux/step07_lxx_package/` | Package / dev-install DCP06 for Linux Captivate sim | tooling done; install blocked |

---

## Prerequisites (already done)

- [x] WSL2 + Ubuntu 22.04
- [x] `CAPTIVATE_POSIX_SDK`, `DCP06_ROOT`, `CAPTIVATE_POSIX_LIBS` in `~/.bashrc`
- [x] `build-essential`, `cmake`, `ninja-build` installed
- [x] SDK libs and `lm --help` verified
- [x] Yocto cross-SDK at `/usr/local/leicasdk-x86_64` (Stage 2 in [WSL Setup §6](DCP06_POSIX_WSL_Setup.md#6-stage-2--yocto-cross-sdk-ts20-device-builds))
- [x] `CAPTIVATE_POSIX_ARM_LIBS` available (set in build script or `~/.bashrc`)

---

## Step 1 — Header smoke test

**Purpose:** Confirm `g++` finds the POSIX SDK include paths and core headers compile on Linux.

### Run (in Ubuntu)

Build **out of tree** under your Linux home directory (CMake on `/mnt/c/...` often fails with “Operation not permitted”):

```bash
source ~/.bashrc

mkdir -p ~/build/dcp06-step01
cd ~/build/dcp06-step01
rm -rf ./*
cmake "$DCP06_ROOT/Project/Linux/step01_header_smoke"
cmake --build .
./header_smoke
echo "Exit code: $?"
```

### Expected result

- CMake configures without errors
- Build succeeds (harmless `#pragma warning` messages from Leica headers are OK)
- `./header_smoke` prints: `POSIX SDK header smoke test OK (IMO_TS20=50)`
- Exit code `0`

### If it fails

- `CAPTIVATE_POSIX_SDK: unbound variable` → run `source ~/.bashrc`
- CMake `Operation not permitted` on `/mnt/c/...` → use `~/build/dcp06-step01` as above (not `Project/Linux/.../build`)
- Missing header → note the path; we may need an extra include dir in `CMakeLists.txt`
- Errors in Leica headers → paste full compiler output

---

## Step 2 — Link smoke test (shared library)

**Purpose:** Build `liblink_smoke.so` linked against the same core Captivate libraries as DCP06 Win32.

### Run (in Ubuntu)

```bash
source ~/.bashrc

mkdir -p ~/build/dcp06-step02
cd ~/build/dcp06-step02
rm -rf ./*
cmake "$DCP06_ROOT/Project/Linux/step02_link_smoke"
cmake --build .
file liblink_smoke.so
nm -D liblink_smoke.so | grep dcp06_posix_link_smoke
echo "Exit code: $?"
```

### Expected result

- Build completes without linker errors
- `file liblink_smoke.so` shows `ELF 64-bit LSB shared object, x86-64`
- `nm` shows exported symbol `dcp06_posix_link_smoke`
- Exit code `0`

### Notes

- Defines `PLAT_WINPC_X86` and `HW_X86` for x86 Ubuntu dev (matches SDK header fallback until Leica documents Linux-specific `PLAT_*` values).
- Links versioned `.so` files directly (`libCommon.so.10.00.0`, etc.) because the SDK ships no `libCommon.so` symlinks.

### If it fails

- `cannot find -l...` or missing `.so` → paste full linker output; we may need extra libs (Boost, Qt, ACE) or `lm --lock` on symlinks.
- Undefined reference errors → paste symbols; we add transitive libraries.

When Step 2 passes, continue to Step 3.

---

## Step 3 — Minimal plugin stub (`Start15751`)

**Purpose:** Build a tiny Captivate plugin `.so` with `APP_ENTRY_POINT` → **`Start15751`** (same App.Id as `Project/DCP06.sys`). Proves plugin entry symbol and GUI framework link. Not yet deployable to Captivate without packaging/localization.

### Run (in Ubuntu)

```bash
source ~/.bashrc

mkdir -p ~/build/dcp06-step03
cd ~/build/dcp06-step03
rm -rf ./*
cmake "$DCP06_ROOT/Project/Linux/step03_min_plugin"
cmake --build .
file libmin_plugin.so
nm -D libmin_plugin.so | grep Start15751
echo "Exit code: $?"
```

### Expected result

- Build completes (warnings from Leica headers are OK)
- `file libmin_plugin.so` → `ELF 64-bit LSB shared object, x86-64`
- `nm` shows **`Start15751`**
- Exit code `0`

### What this builds

- Minimal application: empty dialog, F1 = SET/close
- App.Id **15751** (matches DCP06)
- Local stub tokens in `min_plugin_tok.hpp` (no `.LEN` yet)

### If it fails

- `SwxAssertHandler/SysAssertHandler.hpp: No such file` → CMakeLists must include `Projects/SwxAssertHandler/include` (in repo)
- Link errors → paste full output

When Step 3 passes, continue to Step 4 (DCP06 source subset).

---

## Step 4 — DCP06 `Logger.cpp` on Linux

**Purpose:** Compile the first real DCP06 source file on Linux and verify `DCP::Logger` writes a log file using POSIX `localtime_r`.

### What changed in the main tree

- `src/core/stdafx.h` — added `#elif defined(__linux__)` branch (avoids `#include <vxworks.h>` on Linux)

### Shared CMake

- `Project/Linux/cmake/CaptivatePosixSdk.cmake` — SDK include paths, compile defs, and `dcp06_apply_posix_target_settings()` helper (reused by later steps)

### Run (in Ubuntu)

```bash
source ~/.bashrc

mkdir -p ~/build/dcp06-step04
cd ~/build/dcp06-step04
rm -rf ./*
cmake "$DCP06_ROOT/Project/Linux/step04_dcp06_logger"
cmake --build .
./dcp06_logger_smoke
cat /tmp/dcp06_step04.log
echo "Exit code: $?"
```

### Expected result

- Build succeeds (no SDK `.so` link needed — Logger is standalone)
- `./dcp06_logger_smoke` prints: `DCP06 Logger smoke test OK (/tmp/dcp06_step04.log)`
- Log file contains a line like: `[2026-06-13 ...] [INF] DCP06 Logger POSIX step 4 smoke test`
- Exit code `0`

### If it fails

- `vxworks.h: No such file` → ensure `src/core/stdafx.h` has the `__linux__` branch
- Missing `DCP06_TOK.HPP` or `Onboard_Tok.hpp` → check `CaptivatePosixSdk.cmake` include dirs and `Text/` path
- CMake on `/mnt/c/...` → use `~/build/dcp06-step04` as above

When Step 4 passes, continue to Step 5 (full plugin).

---

## Step 5 — Full `DCP06.so`

**Purpose:** Build the complete DCP06 plugin shared library on Linux — all 110 translation units from `DCP06-VS2008.vcproj`, linked against Captivate POSIX SDK libraries. Exports **`Start15751`** (same entry as Windows `DCP06.dll`).

### New / updated files

| Path | Role |
|------|------|
| `Project/Linux/step05_full_plugin/` | CMake target → `DCP06.so` |
| `Project/Linux/cmake/Dcp06Sources.cmake` | Source list from vcproj (regenerate with `gen_dcp06_sources.py`) |
| `Project/Linux/cmake/CaptivatePosixSdk.cmake` | Extended: extra includes, `DCP_USE_JSON_DATABASE`, SDK + Boost libs |
| `src/application/DCP06.cpp` | `DllMain` guarded with `#ifdef _WIN32` |
| Header fixes | MSVC “extra qualification” in `DistFile.hpp`, `ScanFileFunc.hpp`, `ResBestFit.hpp` |
| `src/file/SelectFile.cpp` | Linux path → ASCII helper (`boost::filesystem` uses `char` paths on Linux) |

### Run (in Ubuntu)

```bash
source ~/.bashrc

mkdir -p ~/build/dcp06-step05
cd ~/build/dcp06-step05
rm -rf ./*
cmake "$DCP06_ROOT/Project/Linux/step05_full_plugin"
cmake --build . -j"$(nproc)"

file DCP06.so
nm -D DCP06.so | grep Start15751
echo "Exit code: $?"
```

Or use the helper script:

```bash
bash "$DCP06_ROOT/Project/Linux/step05_full_plugin/build_wsl.sh"
```

### Expected result

- Build completes (~110 `.cpp` files; many Leica header warnings are OK)
- `file DCP06.so` → `ELF 64-bit LSB shared object, x86-64`
- `nm -D DCP06.so | grep Start15751` shows exported symbol **`Start15751`**
- Exit code `0`

### Notes

- **Not deployable yet** — still needs `.LEN` localization, SWXRes/png assets, and Captivate packaging (MkEdit equivalent on Linux TBD).
- Do **not** include `3rdparty/msvc2008_compat` on Linux (breaks system `<cstdint>`).
- Source list must match vcproj; do not GLOB `src/` (orphan WIP files like `Alignment321Model.cpp` are excluded).

When Step 5 passes, continue to Step 6 (Yocto cross-compile for TS20 ARM).

---

## Step 6 — ARM cross-compile (`DCP06.so` for TS20)

**Purpose:** Build the same full DCP06 plugin for the **TS20 device ABI** (`aarch64`, `armv8-leicageo_linux_5.0-gcc13`) using the Yocto cross-SDK installed in WSL Stage 2.

**Prerequisites:**

- [x] Step 5 passed (x86-64 `DCP06.so`)
- [x] Yocto SDK installed at `/usr/local/leicasdk-x86_64` (see [DCP06_POSIX_WSL_Setup.md §6](DCP06_POSIX_WSL_Setup.md#6-stage-2--optional-yocto-cross-sdk-ts20-device-builds))

### New / updated files

| Path | Role |
|------|------|
| `Project/Linux/step06_arm_cross/` | CMake target → ARM `DCP06.so` |
| `Project/Linux/step06_arm_cross/build_wsl.sh` | Sources Yocto env + runs cross-build |
| `Project/Linux/cmake/CaptivatePosixSdk.cmake` | ARM mode when `CAPTIVATE_POSIX_ARM_LIBS` is set (`HW_ARM`, `PLAT_WINCE_ARM`) |

### Run (in Ubuntu)

```bash
source ~/.bashrc
source /usr/local/leicasdk-x86_64/environment-setup-cortexa53-crypto-leicageo-linux
export CAPTIVATE_POSIX_ARM_LIBS="$CAPTIVATE_POSIX_SDK/Binary/armv8-leicageo_linux_5.0-gcc13/libs"

bash "$DCP06_ROOT/Project/Linux/step06_arm_cross/build_wsl.sh"
```

Or manually:

```bash
source ~/.bashrc
source /usr/local/leicasdk-x86_64/environment-setup-cortexa53-crypto-leicageo-linux
export CAPTIVATE_POSIX_ARM_LIBS="$CAPTIVATE_POSIX_SDK/Binary/armv8-leicageo_linux_5.0-gcc13/libs"

TOOLCHAIN=/usr/local/leicasdk-x86_64/sysroots/x86_64-leicasdk-linux/usr/share/cmake/cortexa53-crypto-leicageo-linux-toolchain.cmake

mkdir -p ~/build/dcp06-step06
cd ~/build/dcp06-step06
rm -rf ./*
cmake -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN" "$DCP06_ROOT/Project/Linux/step06_arm_cross"
cmake --build . -j"$(nproc)"

file DCP06.so
aarch64-leicageo-linux-nm -D DCP06.so | grep Start15751
echo "Exit code: $?"
```

### Expected result

- Build completes (~110 `.cpp` files; Leica header warnings are OK)
- `file DCP06.so` → `ELF 64-bit LSB shared object, ARM aarch64`
- `aarch64-leicageo-linux-nm -D DCP06.so | grep Start15751` shows exported symbol **`Start15751`**
- Exit code `0`

### Notes

- Build **out of tree** under `~/build/dcp06-step06` (Linux filesystem — faster than `/mnt/c/...`).
- Uses Leica's `cortexa53-crypto-leicageo-linux-toolchain.cmake` from the Yocto SDK.
- ARM libs come from `Binary/armv8-leicageo_linux_5.0-gcc13/libs` (not the x86 Ubuntu libs).
- Defines `HW_ARM` + `PLAT_WINCE_ARM` (matches DCP06 physical device config; `__linux__` handles POSIX paths).

### If it fails

- `Yocto SDK not installed` → complete [WSL Setup §6](DCP06_POSIX_WSL_Setup.md#6-stage-2--optional-yocto-cross-sdk-ts20-device-builds)
- `aarch64-leicageo-linux-g++: not found` → `source .../environment-setup-cortexa53-crypto-leicageo-linux` first
- Linker errors / undefined references → paste full output; ARM libs may need extra transitive `.so` entries
- CMake picks host `g++` → ensure `-DCMAKE_TOOLCHAIN_FILE=...` is passed on the **first** `cmake` invocation

---

## Step 7 — Linux Captivate package (`.lxx`) and dev install

**Purpose:** Turn the Step 5 `DCP06.so` into a loadable Ubuntu simulator package (`.lxx`) or install directly into the Linux Captivate plugin tree for UI testing.

**Tooling:** MkEdit from the **x86_64 Plugin SDK** (Windows only). PObs build without a dongle; **final `.lxx` LOB packaging requires a Leica dongle** (same as Win32 `.dxx` — see [DCP06_Build_PostBuild_Troubleshooting.md](DCP06_Build_PostBuild_Troubleshooting.md)).

### Prerequisites

- [x] Step 5 complete (`~/build/dcp06-step05/DCP06.so`)
- [x] Linux Captivate simulator running (see [DCP06_POSIX_WSL_Setup.md](DCP06_POSIX_WSL_Setup.md))
- [x] `DCP06.LEN` — run on Windows: `scripts\build_lang.bat` (TextTool)
- [x] x86_64 SDK MkEdit at `Captivate_PluginSDK_x86_64_v10.00_RC\...\Tools\MKTools\MkEdit.exe`

### Run (in Ubuntu / WSL)

```bash
source ~/.bashrc
bash "$DCP06_ROOT/Project/Linux/step07_lxx_package/build_lxx.sh"
```

Modes:

```bash
# Dev install only (works without dongle)
bash "$DCP06_ROOT/Project/Linux/step07_lxx_package/build_lxx.sh" install

# .lxx package only (needs dongle on Windows host)
bash "$DCP06_ROOT/Project/Linux/step07_lxx_package/build_lxx.sh" package
```

### What the script does

1. Copies `DCP06.so` → staging as `libDCP06.so` (Linux plugin naming convention)
2. Stages `SWXRes/` logos (fixes `DCP06_logo_2X.png` case for `.sys`)
3. Runs MkEdit `-M:mk -C` from `Project/` (attempts `DCP06.lxx` — **fails without dongle**, PObs still built)
4. Runs MkEdit `-M:mk -I:… -C` to install PObs under `/home/gui-app/captivate/internal-storage/System/Plugin/DCP06/`
5. Post-install fixups: `libDCP06.app` → `DCP06.app`, copies `EN/DCP06.LEN` and `SWXRes/` (MkEdit resources copy via WSL path is flaky)

### Expected result (dev install, no dongle)

After `install` or `all`:

```
/home/gui-app/captivate/internal-storage/System/Plugin/DCP06/
  DCP06.app
  DCP06.sys
  libDCP06.so
  EN/DCP06.LEN
  SWXRes/DCP06_logo_1X.png
  SWXRes/DCP06_logo_2X.png
```

Restart Captivate to load the plugin:

```bash
# TS sim must be running first
start_ts_simulator_grpc_client -d
captivate    # Ctrl+C to stop; rerun to reload plugins
```

Look for **DCP06** in the Apps menu (App.Id `15751`, entry `Start15751`).

**Verified 2026-06-15:** After fixing `DCP06.sys` (`App.Type=mixed`, CRLF, EntryPoint comment), dev install shows DCP06 as **tile #25** in Apps. Tapping it shows *“This app may be damaged”* because `libDCP06.so.sig` is missing — see [Captivate Sim §7.1](DCP06_POSIX_Linux_Captivate_Sim.md#71-launch--this-app-may-be-damaged-2026-06-15).

### Expected result (`.lxx`, dongled machine)

On a PC with a valid Leica dongle, stage assets then run MkEdit with **Release** config (includes signature POB):

```bash
bash "$DCP06_ROOT/Project/Linux/step07_lxx_package/build_lxx.sh" package
```

From Windows `Project/` directory:

```
MkEdit.exe -M:mk -F:x86_64-ubuntu_22.04-gcc11\Config\DCP06_Release.xml -C
```

Output: `Project/Linux/step07_lxx_package/out/RelWithDebInfo/DCP06.lxx`

Install via Captivate v10 Linux sim UI (same as `HelloWorldGSV.lxx` in the Captivate distribution). Built-in samples ship signed `.lxx` files; partner plugins need the dongle for `libDCP06.so.sig`.

### If it fails

| Symptom | Fix |
|---------|-----|
| `DCP06.so not found` | Run Step 5 build script first |
| `DCP06.LEN missing` | Run `scripts\build_lang.bat` on Windows |
| `can't build applications without valid dongle` | **Normal** for `.lxx` without dongle; use `install` mode to verify menu registration |
| `cannot copy directory tree: DCP06_Res.pob` on install | Script runs post-install fixups; verify `SWXRes/` and `EN/` manually |
| DCP06 not in Apps menu | Restart `captivate`; confirm TS sim running; check `DEFAULT_SENSOR_TYPE=2300` in `/etc/captivate/captivate.env` |
| MkEdit `cmd.exe` quoting errors | Run from WSL as above; script uses `Project/` as MkEdit working directory |
| Captivate GUI flashes / `Killed` on startup | Broken `DCP06.sys` — see [Captivate Sim §7](DCP06_POSIX_Linux_Captivate_Sim.md#7-dcp06-plugin-crash--important); fix `.sys` and re-run `install` |
| *“This app may be damaged”* when opening DCP06 | Missing `libDCP06.so.sig` — build signed `DCP06.lxx` with dongle (`DCP06_Release.xml`); see [§7.1](DCP06_POSIX_Linux_Captivate_Sim.md#71-launch--this-app-may-be-damaged-2026-06-15) |
| Blank DCP06 menu icon | PNGs manually copied to `SWXRes/`; may improve after full signed `.lxx` install |

### Step 7 status (2026-06-15)

| Milestone | Status |
|-----------|--------|
| MkEdit staging + PObs | Done |
| Dev install (`-I:`) | Done — DCP06 in Apps menu |
| Captivate startup with plugin | Done (after `.sys` fix) |
| DCP06 launches / runs | **Blocked** — needs signed `.lxx` (dongle) |
| `.lxx` release package | Pending — `DCP06_Release.xml` added for Pasi |

### Files added for Step 7

| Path | Role |
|------|------|
| `Project/x86_64-ubuntu_22.04-gcc11/DCP06.sys` | Plugin descriptor (Ubuntu, CRLF + `App.Type=mixed`) |
| `Project/x86_64-ubuntu_22.04-gcc11/DCP06.dat` | Version metadata (v10 / build 309) |
| `Project/x86_64-ubuntu_22.04-gcc11/Config/DCP06_RelWithDebInfo.xml` | MkEdit dev install / PObs |
| `Project/x86_64-ubuntu_22.04-gcc11/Config/DCP06_Release.xml` | MkEdit signed `.lxx` (dongle; includes `.sig` POB) |
| `Project/Linux/step07_lxx_package/build_lxx.sh` | Staging + MkEdit driver |

---

## Build path complete (Steps 1–7)

Reconnaissance build steps on AMS WSL (2026-06-14):

| Step | Output | Architecture | Entry symbol |
|------|--------|--------------|--------------|
| 5 | `~/build/dcp06-step05/DCP06.so` | x86-64 (Ubuntu dev) | `Start15751` |
| 6 | `~/build/dcp06-step06/DCP06.so` | ARM aarch64 (TS20 device) | `Start15751` |
| 7 | MkEdit dev install; Apps menu tile | x86-64 sim | `Start15751` — **run** needs signed `.lxx` |

**`.lxx` release packaging** needs a **dongled Windows host** — `DCP06_Release.xml` ready for Pasi.

**Linux Captivate simulator** runs on AMS WSL; see [DCP06_POSIX_Linux_Captivate_Sim.md](DCP06_POSIX_Linux_Captivate_Sim.md) for startup, Docker, and plugin crash recovery.

Remaining for hardware delivery:

1. **`.lxx` / `.yxx` signing** — Leica dongle + partner signing workflow
2. **Hardware test** — load ARM `.yxx` on TS20; exercise TBL/CPI/GuiPlus paths

Hand off compile/link/packaging findings to Pasi for the **production DCP05** Linux port.

---

## Related docs

- [DCP06_POSIX_WSL_Setup.md](DCP06_POSIX_WSL_Setup.md)
- [DCP06_POSIX_Linux_Captivate_Sim.md](DCP06_POSIX_Linux_Captivate_Sim.md)
- [DCP06_POSIX_SDK_Analysis.md](DCP06_POSIX_SDK_Analysis.md)

---

## Revision history

| Date | Change |
|------|--------|
| 2026-06-13 | Step 1 added |
| 2026-06-13 | Step 2 added |
| 2026-06-13 | Step 3 added (minimal plugin, Start15751) |
| 2026-06-13 | Step 4 added (Logger.cpp, stdafx Linux branch, CaptivatePosixSdk.cmake) |
| 2026-06-13 | Step 5 added (full DCP06.so, 110 sources, Start15751, Linux port fixes) |
| 2026-06-13 | Added “Why these steps exist” (DCP05 vs DCP06 reconnaissance context) |
| 2026-06-13 | Step 6 added (ARM cross-compile via Yocto SDK + `step06_arm_cross`) |
| 2026-06-14 | Step 7 added (MkEdit `.lxx` / dev install for Linux Captivate sim) |
| 2026-06-14 | Documented DCP06 plugin crash recovery; dev install marked experimental |
| 2026-06-15 | Step 7 verified: Apps menu + `.sys` fix; launch needs signed `.lxx`; `DCP06_Release.xml` |
