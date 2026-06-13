# DCP06 — Captivate POSIX Plugin SDK Analysis

**Date:** 2026-06-13  
**SDK analysed:** `Captivate_PluginSDK_POSIX_v10.0.0-rc.309`  
**Location (local):** `C:\Users\dell\Desktop\AMS\Development\Captivate_PluginSDK_POSIX_v10.0.0-rc.309`  
**Purpose:** Assess Leica’s Linux/POSIX Captivate 10 SDK and document what AMS learns for **TS20 / Linux plugin development**. Findings here feed the **production DCP05 Linux port** (see [Context & goals](#context--goals) below).

---

## Context & goals

### Why AMS is doing this now

Leica **TS20** total stations ship with **Captivate 10 on Linux** (replacing Windows CE on older onboard controllers). Resellers and end customers who bought **DCP05 for TS20** are waiting; AMS was late to the Linux Captivate v10 transition because Leica did not give the same early notice as for past Captivate releases.

**Commercial interim option (separate from this doc):** CS20 controller (Windows EC7) can run DCP05 as a backup until onboard TS20 DCP05 is ready. Customer licensing and trial terms are handled by sales/management, not this engineering track.

### DCP05 vs DCP06 — who owns what

| | **DCP05** | **DCP06 (this repo)** |
|---|-----------|------------------------|
| **Role** | **Production** onboard 3D measurement for Leica resellers/customers | **Next trial / evolution** of DCP05 (database, LIST/PICK, etc.) |
| **Linux port owner** | **Pasi Ojaniemi** (`C:\Users\dell\Desktop\AMS\Development\DCP05`) | This POSIX work is **reconnaissance**, not the customer deliverable |
| **Goal on Linux** | Ship **DCP05** on TS20 | Learn SDK, prove build path, find pitfalls, hand findings to Pasi |

**Do not confuse the two:** shipping DCP06 on Linux is **not** the answer to “when is DCP05 for TS20 available?” This work shortens that path by de-risking the SDK and toolchain.

### What this POSIX track is trying to achieve

1. **Understand the POSIX SDK** — layout, libraries, gaps vs Windows SDK, TS20/Yocto toolchain.
2. **Prove a repeatable Linux build** — WSL2 steps in [DCP06_POSIX_Linux_Build_Steps.md](DCP06_POSIX_Linux_Build_Steps.md) (header smoke → full `.so` with `Start15751`).
3. **Catalog porting issues** — things MSVC allows but GCC/Linux rejects (see §5 and build-step notes).
4. **Hand off to Pasi** — concrete fixes, CMake patterns, and open questions so DCP05 porting can move faster.

### What “done” means here (vs production)

| Milestone | Meaning | Status |
|-----------|---------|--------|
| Steps 1–5 on x86 WSL | Compiles and links; exports plugin entry — **learning milestone** | **done** |
| Step 6 (Yocto ARM) | Cross-compile for **actual TS20 device** ABI | **done** |
| Deployable plugin on instrument | Needs localization (`.LEN`), assets, packaging — **MkEdit/TextTool equivalent on Linux still TBD** | pending |

Production **DCP05 on TS20** remains Pasi’s deliverable; this repo documents **how** and **what broke**, not a reseller-facing release.

### Findings already useful for DCP05 (living list)

Updated as the step-by-step build progresses; see also [DCP06_POSIX_Linux_Build_Steps.md](DCP06_POSIX_Linux_Build_Steps.md).

- Build **out of tree** under `~/build/...` in WSL (not on `/mnt/c/...`).
- Do **not** include `3rdparty/msvc2008_compat` on Linux — breaks system `<cstdint>`.
- Guard **`DllMain`** and **`vxworks.h`** paths; add `__linux__` branch in `stdafx.h`.
- **`boost::filesystem::path`** uses narrow strings on Linux — do not pass `path.c_str()` to `BSS_UTI_WCharToAscii`.
- MSVC **“extra qualification”** in class bodies (`Class::method()`) must be fixed for GCC.
- Link **versioned** SDK `.so` files directly; add **Boost filesystem/system** from SDK `libs/`.
- POSIX SDK ships **no** MkEdit, TextTool, SWXResBuilder, HelloWorld, or simulator.
- **ARM cross-build:** source Yocto `environment-setup-cortexa53-crypto-leicageo-linux` each session; use Leica's `cortexa53-crypto-leicageo-linux-toolchain.cmake`; set `CAPTIVATE_POSIX_ARM_LIBS` (not x86 libs).
- **ARM compile defs:** `HW_ARM` + `PLAT_WINCE_ARM` (no `OS_WINCE` / `WIN32` — `__linux__` handles POSIX paths).
- **Symbol check on ARM:** use `aarch64-leicageo-linux-nm -D DCP06.so` (host `nm` cannot read aarch64 ELF).

---

## 1. Executive Summary

Leica has released a **Captivate 10 Plugin SDK for Linux/POSIX** targeting the **TS20** total station (Linux on ARM, replacing Windows CE on older instruments).

| Finding | Detail |
|---------|--------|
| **SDK exists** | Headers + prebuilt `.so`/`.a` + Yocto cross-toolchain installer |
| **API compatibility** | Same Captivate 10 API families as Windows (`TPI`, `TBL`, `CPI`, `ABL`, `GuiPlus`, `OBS`, `BSS`) |
| **DCP06 port feasibility** | **High** for application logic; **medium–high** for build/deploy integration |
| **RC maturity** | Version `v10.0.0-rc.309` — early release candidate; expect updates |
| **Gap vs Windows SDK** | No HelloWorld sample, no MkEdit/TextTool/SWXResBuilder, no documentation bundle |

**Conclusion:** The POSIX SDK confirms Leica’s TS20/Linux direction. DCP06 can share most source with the Windows build, but requires a **new Linux build system, packaging pipeline, and dev host** — not a simple recompile on Windows.

---

## 2. Package Layout

Unpack produces a **double-nested** folder:

```
Captivate_PluginSDK_POSIX_v10.0.0-rc.309/
└── Captivate_PluginSDK_POSIX_v10.0.0-rc.309/    ← SDK root
    ├── Applications/
    ├── Binary/
    ├── Platform-SDK/
    ├── Projects/
    └── Tools/
```

| Metric | Value |
|--------|-------|
| Total size | ~3.2 GB |
| Total files | ~16,700 |
| Header files (`.HPP`/`.hpp`) | ~15,800 |
| C++ source (`.cpp`) | 17 (ConvertersBase only) |

The SDK is **headers + binaries**, not full Captivate source. Plugin developers link against prebuilt libraries.

---

## 3. Top-Level Folders

### 3.1 `Projects/` — API headers (37 modules)

Same module structure as the Windows `SWXIII_PluginDevKit/Projects/` tree. Key modules for DCP06:

| Module | DCP06 relevance |
|--------|-----------------|
| `ABL_Tps` | TBL measurement APIs (`TBL_Measurement.hpp`, survey model, prism search) |
| `TpsFacade` | TPI instrument info, sensor paths |
| `Common` | CPI config/archive, shared types |
| `GuiPlus` | Onboard UI, `Onboard_TOK.HPP`, controllers, softkeys |
| `HALTools` | Platform abstractions, unicode tools, path types |
| `GeoMath`, `Math` | Geometry / math |
| `GSV`, `GSV_Base`, `GSV_DataProvider` | Global survey / plugin registration |
| `Utilities`, `SLBasics` | Helpers |

**No build files** (no `CMakeLists.txt`, `Makefile`, `.vcxproj`) under `Projects/`.

### 3.2 `Binary/` — prebuilt libraries (two targets)

| Folder | Role | Toolchain | Library count |
|--------|------|-----------|---------------|
| `x86_64-ubuntu_22.04-gcc11` | Dev host / simulator-side | Ubuntu 22.04, **GCC 11** | **252** |
| `armv8-leicageo_linux_5.0-gcc13` | **TS20 device** | Leica Geo Linux 5.0, **GCC 13**, **ARMv8 / Cortex-A53** | **186** |

Libraries use versioned names, e.g. `libABL_Tps.so.10.00.0`, matching Captivate 10.

**Set differences:**

- **69 libraries x86-only** — mostly test, imaging, simulator, and dev extras (e.g. `libceres`, `libcppunit`, OpenCV-heavy imaging).
- **3 libraries ARM-only** — device gRPC drivers (`libSideCoverGrpcDriver.a`, `libUsbGrpcDriver.a`, `libGrpcServiceImplFactory.a`).

**DCP06-critical libraries present on both targets:**

| Library | x86 | ARM |
|---------|-----|-----|
| `libABL_Tps.so.10.00.0` | ✓ | ✓ |
| `libTpsFacade.so.10.00.0` | ✓ | ✓ |
| `libCommon.so.10.00.0` | ✓ | ✓ |
| `libGuiPlus.so.10.00.0` | ✓ | ✓ |
| `libHALTools.so.10.00.0` | ✓ | ✓ |
| `libGeoMath.so.7.2.7` | ✓ | ✓ |
| `libGSV*.so.10.00.0` | ✓ | ✓ |
| `libMath.so.10.00.0` | ✓ | ✓ |
| `libCaptivateRoot.so.10.00.0` | ✓ | ✓ |
| `libHelloWorldGSV.so` | ✓ | ✓ |

Additional stack on Linux (both targets where applicable): **Qt 6.8.2**, Boost 1.81, gRPC/protobuf, ACE/TAO CORBA, OpenCV 3.1 (x86-heavy).

### 3.3 `Platform-SDK/` — Yocto cross-compiler

| File | Purpose |
|------|---------|
| `cortexa53-crypto-sdk_v5.0.23.sh` | Self-extracting **Yocto/OpenEmbedded SDK** installer for ARM device builds |

**Installer requirements (from script header):**

- Host: **Linux x86_64** (not Windows native — use WSL2 or Linux VM)
- Python 2 or 3, `xz`, host `gcc`
- Kernel > 3.2.0
- Default install path: `/usr/local/leicasdk-x86_64`

After install, source the Yocto `environment-setup-*` script for cross-compiler, sysroot, and `CC`/`CXX`.

**Replaces:** Windows `Stellar_SWDC_EC7_SDK.msi` (WinCE/ARM cross-compile for TS16/CS20).

### 3.4 `Applications/API/` — sample headers only

Contains **Stakeout** sample API headers (Common, GPS, TPS). **Not** a complete HelloWorld plugin with sources, `.dat`, `.sys`, or build project.

### 3.5 `Tools/LibManager/` — symlink utility

| File | Type | Purpose |
|------|------|---------|
| `lm` | Linux x86_64 ELF binary | Manage versioned `.so` symlinks in cross-build setups |

Usage (from embedded help):

```bash
lm --lock   /path/to/library.so    # Replace symlink with actual binary
lm --release library.so            # Restore symlink from cache
lm --help
```

Documented for **Windows/Linux WSL** cross-build environments where versioned symlinks cause linker issues.

---

## 4. TS20 Support in Headers

TS20 is first-class in this SDK:

| Location | Evidence |
|----------|----------|
| `TPI_InstrumentInfo.hpp` | `IMO_TS20 = 50` |
| `Onboard_TOK.HPP` | `V_TS20_TOK` |
| `GUI_KeyTypes.hpp` | `Key_Launch7`, `Key_Launch8` — TS20 Hz/V knob buttons |
| `TBL_PrismClassificationCheck.hpp` | References TS20 prism classification |

---

## 5. Linux/POSIX API Differences (DCP06 impact)

Leica has adapted the API layer for Linux. Key points for DCP06 porting:

### 5.1 Platform detection

`SurveyDefines.hpp`:

```cpp
#if defined(WIN32)
    // windows.h, winsock2, ...
#elif defined(__linux__)
    // Linux
#else
    #error hmm... unknown platform!
#endif
```

### 5.2 Paths

`CPI_Types.hpp` defines Linux path limits via POSIX `PATH_MAX` (forward slashes, no `_MAX_PATH`).

### 5.3 Unicode / wchar_t

| Platform | Native `wchar_t` | Notes |
|----------|------------------|-------|
| Windows | UTF-16 | Existing DCP06 / Captivate 10 behaviour |
| Linux | **UTF-32** | `BSS_UTI_UnicodeStringTools.hpp` has `utf16LengthFromUtf32` |

`CPI_CFGArchive.hpp` documents: *"For Linux, treat wchar_t as 2 Bytes to compatible with Windows"* for config archive I/O — binary job/config compatibility with Windows instruments.

**DCP06 action:** Audit all `BSS_UTI_WCharToAscii`, `StringC`, and `Common::convert_to_ascii` usage. Offsv/MeasV were partially migrated on Windows; finish that pattern before POSIX build.

### 5.4 UI stack

Linux SDK ships **Qt 6.8.2** (`libQt6*.so`). This is a significant stack change vs older Windows Captivate builds. Indirect impact possible if DCP06 relies on Captivate GUI behaviour that changed with Qt 6.

### 5.5 Small DCP06 Windows-specific code today

Known `#ifdef _WIN32` in DCP06 (easy to abstract):

- `Logger.cpp`, `Application.cpp` — `localtime_s` vs `localtime_r` (POSIX branch already exists)
- `Model.cpp` — `%TEMP%` log path fallback
- `stdafx.h` — `windows.h` (replace with Linux stdafx or platform header)

---

## 6. Missing vs Windows SDK

Compared to `Captivate_PluginSDK_x86_v10.00_RC` / `SWXIII_PluginDevKit`:

| Present on Windows | POSIX SDK | Impact |
|--------------------|-----------|--------|
| `Documentation/` (GettingStarted, API doc, release notes) | **Missing** | Get from Leica separately |
| `Samples/HelloWorldBase/` (full project) | **Missing** | Create CMake/Makefile project from scratch |
| `Tools/MkEdit/` (packaging, `.dxx`) | **Missing** | New TS20 deployment workflow needed |
| `Tools/TextTool/` (`.men` → `.LEN`) | **Missing** | Localization pipeline on Linux TBD |
| `Tools/SWXResBuilder/` (SVG → PNG) | **Missing** | Resource pipeline TBD |
| `Captivate_v9.97.202/` simulator bundle | **Missing** | x86 Linux sim or on-device testing |
| VS 2008 projects / `SYSTEM1500_ROOT` docs | **Missing** | GCC + Yocto; new env var layout |
| `Stellar_SWDC_EC7_SDK.msi` | **Replaced** by `cortexa53-crypto-sdk_v5.0.23.sh` | Linux-only install |

The POSIX package is an **early developer drop**: sufficient to link against Captivate APIs, not a turnkey plugin kit like Windows.

---

## 7. Windows vs POSIX Workflow Comparison

```
Windows (current DCP06)                 POSIX (TS20 path)
─────────────────────────               ─────────────────────────
VS 2008 + vc90 libs                     GCC 11 (x86 sim) / GCC 13 cross (ARM device)
Win32 simulator → DCP06.dll             x86_64 Ubuntu → DCP06.so (dev/sim TBD)
WinCE ARM device (Stellar EC7)          ARMv8 Leica Geo Linux 5.0 → DCP06.so
MkEdit + TextTool + SWXResBuilder       Not included — must be built/replaced
SYSTEM1500_ROOT                         Yocto SDK path + Binary/.../libs
Post-build: registry → TS simulator      TBD — Leica packaging docs needed
Output: DCP06.dll                       Output: DCP06.so (expected)
```

---

## 8. Recommended Build Variant Strategy

Treat POSIX as a **fourth build config**, not a fork:

```
DCP06 (shared src)
├── Win32 simulator          (existing — DCP06-VS2008, Release|Win32)
├── WinCE/ARM device         (existing — TS16, MS60, CS20)
├── CS35 x64                 (existing — DCP06-VS2022)
└── Linux/POSIX              (new — TS20)
    ├── x86_64-ubuntu_22.04-gcc11     (dev / sim)
    └── armv8-leicageo_linux_5.0-gcc13 (device)
```

Shared source; separate project config, link paths, and post-build steps.

---

## 9. First-Build Checklist

Use this as the initial integration plan. Order matters — do not port all of DCP06 before a minimal plugin links and loads.

### Phase 0 — Environment (Linux host)

- [x] Set up **Ubuntu 22.04 x86_64** (bare metal, VM, or **WSL2**)
- [x] Extract SDK to a fixed path (AMS: `/mnt/c/.../Captivate_PluginSDK_POSIX_v10.0.0-rc.309/`)
- [x] Run `Platform-SDK/cortexa53-crypto-sdk_v5.0.23.sh` (requires sudo for default `/usr/local/leicasdk-x86_64`)
- [x] Source Yocto `environment-setup-cortexa53-crypto-leicageo-linux` for ARM cross-builds
- [x] Install build tools: `cmake`, `ninja-build` or `make`, `g++-11`, Python 3

### Phase 1 — Minimal plugin (HelloWorld parity)

- [x] Create `Project/Linux/` producing **`DCP06.so`** (steps 1–3: header smoke → link smoke → `Start15751` stub)
- [ ] Copy/adapt `DCP06.dat`, `DCP06.sys` (App.Id=15751, entry point) from Windows project
- [x] Add include paths to `Projects/*/Source/API_Hdr` (mirror Windows vcxproj list)
- [x] Link against `Binary/x86_64-ubuntu_22.04-gcc11/libs` — start with same libs as Windows Release|Win32
- [ ] Build empty plugin that registers and appears in Captivate menu (or confirm load on device)
- [ ] Use `LibManager/lm --lock` if symlinked libs break the linker on WSL

### Phase 2 — DCP06 source port

- [x] Replace / extend `stdafx.h` for Linux (`__linux__`, no `windows.h`)
- [x] Abstract platform layer: log path, time functions, path separators (partial — Logger, SelectFile)
- [x] Audit wchar / string conversion (see §5.3) — `SelectFile.cpp` fixed for Linux paths
- [x] Build DCP06 shared sources; fix compile errors module by module (110 vcproj sources, steps 4–5)
- [x] Rebuild third-party deps for Linux/arm64 (Eigen, jsoncpp — avoid Windows `vcpkg_installed/`)

### Phase 3 — Localization and resources

- [ ] Ask Leica for Linux TextTool or alternative `.men` → `.LEN` workflow
- [ ] Port or replace `scripts/build_lang.bat` (Python + TextTool)
- [ ] Port or replace SWXResBuilder for PNG resources
- [ ] Confirm plugin resource layout on TS20

### Phase 4 — Device build and deploy

- [x] Cross-compile with `armv8-leicageo_linux_5.0-gcc13` libs + Yocto sysroot (Step 6 — `DCP06.so` aarch64, `Start15751`)
- [ ] Ask Leica for TS20 plugin install / packaging (MkEdit equivalent)
- [ ] Test on TS20 hardware: measurement (TBL), paths (CPI), config (archive), UI (GuiPlus)

### Phase 5 — Ongoing dual maintenance

- [ ] Keep Windows/WinCE builds working (`#ifdef` only at platform boundaries)
- [ ] CI: separate jobs for Win32 and Linux x86 (and ARM when hardware available)

---

## 10. Questions for Leica

1. Is there a **POSIX HelloWorld sample project** (sources + CMake/Makefile + packaging)?
2. **Simulator** for x86_64 Ubuntu — is one available, or is on-device testing required?
3. **Plugin packaging** on TS20 — MkEdit equivalent, file layout, `.dxx` or new format?
4. **TextTool / localization** on Linux — same `.men`/`.LEN` pipeline?
5. **SDK stability** — timeline from `rc.309` to GA; breaking API changes expected?
6. **Support window** — how long will WinCE (TS16) and Linux (TS20) SDKs coexist?

---

## 11. Related DCP06 Documentation

| Document | Relevance |
|----------|-----------|
| [DCP_SDK_Setup_Instructions.md](DCP_SDK_Setup_Instructions.md) | Windows Captivate 10 SDK setup (baseline comparison) |
| [DCP06_POSIX_WSL_Setup.md](DCP06_POSIX_WSL_Setup.md) | WSL2 + Ubuntu 22.04 install and env vars for POSIX SDK |
| [DCP06_Architecture_Review.md](DCP06_Architecture_Review.md) | Multi-target constraints, folder structure |
| [DCP06_Agent_Handoff_Summary.md](DCP06_Agent_Handoff_Summary.md) | Current Windows build state, string pipeline |
| [DCP06_Build_PostBuild_Troubleshooting.md](DCP06_Build_PostBuild_Troubleshooting.md) | MkEdit / TextTool issues on Windows |

---

## 12. Revision History

| Date | Change |
|------|--------|
| 2026-06-13 | Initial analysis of `Captivate_PluginSDK_POSIX_v10.0.0-rc.309` |
| 2026-06-13 | Added Context & goals (DCP05 production vs DCP06 reconnaissance, Pasi handoff, findings list) |
| 2026-06-13 | Steps 1–6 complete: x86 + ARM `DCP06.so`; checklist and ARM cross-build findings updated |

---

*Analysed from local SDK folder on Windows; Steps 1–6 build path verified on WSL2 + Ubuntu 22.04 (x86-64 and ARM aarch64).*
