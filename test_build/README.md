# DCP06 Test Build (No Leica SDK)

**Purpose:** Temporary standalone build to verify DCP06 code compiles and links **without** the Leica System 1500 SDK. Use this until SDK installation instructions are available. Once ready for simulator/total station targets, delete this folder and use the main project with the full SDK.

**Goal:** Compile → static library (.lib). No executable. No meaning to run anything—just validate that the code builds.

## Options

### Option A: CMake

```powershell
cd test_build
mkdir build -Force
cd build
# With vcpkg: cmake .. -G "Visual Studio 17 2022" -A Win32 -DCMAKE_TOOLCHAIN_FILE=$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
# Without:    cmake .. -G "Visual Studio 17 2022" -A Win32
cmake --build . --config Release
```

Requires: CMake, Visual Studio 2022. vcpkg (eigen3, boost-filesystem) recommended.

### Option B: Visual Studio 2022

1. Open `test_build/DCP06_TestBuild.sln` in VS2022
2. Build (Ctrl+Shift+B)
3. If you get errors: copy the Error List and share—we'll add stubs or fix includes iteratively

Uses vcpkg manifest in the repo root; ensure vcpkg integration is enabled (VcpkgEnabled in vcxproj).

## What gets built

- **Target:** Static library `dcp06_test.lib` (no DLL, no .exe)
- **Includes:** All DCP06 sources—same as main project
- **Excludes:** No `$(SYSTEM1500_ROOT)`, no Leica SDK paths, no `ABL_*.lib` etc.
- **Stubs:** Uses `include/sdk_fallback/` and `Text/TXT_EXP.HPP` for SDK substitutes

## Removing this

When you have SDK setup from your colleague:

1. Delete the `test_build/` folder
2. Use `Project/MSVS/DCP06.vcxproj` with `SYSTEM1500_ROOT` set
3. Build the real DLL for simulator/total station
