# DCP06 — POSIX Linux Build (Step by Step)

**Date:** 2026-06-13  
**Environment:** WSL2 + Ubuntu 22.04 (see [DCP06_POSIX_WSL_Setup.md](DCP06_POSIX_WSL_Setup.md))  
**SDK:** `Captivate_PluginSDK_POSIX_v10.0.0-rc.309`

Work through these steps **in order**. Do not skip ahead until the current step passes.

---

## Roadmap

| Step | Folder | Goal | Status |
|------|--------|------|--------|
| **1** | `Project/Linux/step01_header_smoke/` | Compile a tiny program that includes Leica SDK headers | done |
| **2** | `Project/Linux/step02_link_smoke/` | Link a test `.so` against core SDK libraries | done |
| **3** | `Project/Linux/step03_min_plugin/` | Minimal plugin stub exporting `Start15751` | **→ you are here** |
| 4 | `Project/Linux/` | CMake build of a **subset** of DCP06 sources | pending |
| 5 | `Project/Linux/` | Full `DCP06.so` (all sources) | pending |
| 6 | Yocto SDK | Cross-compile for TS20 (`armv8-leicageo_linux_5.0-gcc13`) | pending |

---

## Prerequisites (already done)

- [x] WSL2 + Ubuntu 22.04
- [x] `CAPTIVATE_POSIX_SDK`, `DCP06_ROOT`, `CAPTIVATE_POSIX_LIBS` in `~/.bashrc`
- [x] `build-essential`, `cmake`, `ninja-build` installed
- [x] SDK libs and `lm --help` verified

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

## Related docs

- [DCP06_POSIX_WSL_Setup.md](DCP06_POSIX_WSL_Setup.md)
- [DCP06_POSIX_SDK_Analysis.md](DCP06_POSIX_SDK_Analysis.md)

---

## Revision history

| Date | Change |
|------|--------|
| 2026-06-13 | Step 1 added |
| 2026-06-13 | Step 2 added |
| 2026-06-13 | Step 3 added (minimal plugin, Start15751) |
