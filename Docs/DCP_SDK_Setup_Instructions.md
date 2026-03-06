# DCP SDK Setup Instructions

**Target project: DCP06** (not DCP05). These instructions are for DCP06 development.

The setup follows the same pattern as DCP05. Where we refer to Pasi's email, that was written for **DCP05**; the guidance applies to DCP06 as well. *Source: SDK folder content, Pasi's DCP05 email, local analysis.*

---

## Priority: 32-bit (TS, CS20)

| SDK | Targets | VS Required | Simulator |
|-----|---------|-------------|-----------|
| **32-bit** (primary) | TS, CS20 | **VS 2008** | TS simulator – **already installed** for Captivate 10 |
| **64-bit** | CS35 | VS 2022 | No simulator / needs license *(per Pasi)* |

**Primary path:** Use **`Captivate_PluginSDK_x86_v10.00_RC`** (32-bit) for TS and CS20. TS simulator is available on this PC for Captivate 10.

**VS 2008 required:** For TS and CS20 (32-bit), you **must** have Visual Studio 2008 installed. The SDK libraries are built with vc90 and are not compatible with VS 2022. VS 2008 can be installed alongside VS 2022.

**CS35:** Per Pasi's notes, there is no CS35 simulator or it requires additional licensing – so CS35 (64-bit) is not the main development path.

---

# Part A: Taking the SDK in Use

## Where to Start

You need **both** before building for TS/CS20. Order does not matter; do them in parallel if you prefer:

1. **Install Visual Studio 2008** – Required for TS/CS20. SDK libs (vc90) are incompatible with VS 2022. Install alongside VS 2022.
2. **Install the SDK** – Extract, run MSIs, set `SYSTEM1500_ROOT`.

**DCP06 project files** (renamed for clarity):

| File | VS | Targets |
|------|-----|---------|
| **DCP06-VS2008.sln** / **DCP06-VS2008.vcproj** | VS 2008 | TS, CS20 (Win32 simulator + Stellar device) |
| **DCP06-VS2022.sln** / **DCP06-VS2022.vcxproj** | VS 2022 | CS35 (Win32 + x64; no simulator per Pasi) |

---

## 1. Prerequisites

- [ ] **Visual Studio 2008** – Required for TS/CS20 (32-bit). SDK libs are vc90; VS 2022 cannot build for TS/CS20. Install VS 2008 alongside VS 2022 if needed.
- [ ] Run the **MSI installers** (see section 2 below) before building.

## 2. Installers (MSI Files) – Run These First

The 32-bit SDK contains MSI installers that **must be run** for a complete SDK setup:

| MSI File | Location | Purpose |
|----------|----------|---------|
| **Stellar_SWDC_EC7_SDK.msi** | `SWXIII_PluginDevKit\Platform-SDK\` | **Required.** Platform SDK for WinEC7 (Stellar) – target device build, cross-compilation tools, ARM libs. Run this first. |
| **TextToolSetup_v5.1.1.msi** | `SWXIII_PluginDevKit\Tools\TextTool\` | Optional. Text database tool for localization (`.men` files). |

**Full path for the required installer:**
```
C:\Users\dell\Desktop\AMS\Development\Captivate_PluginSDK_x86_v10.00_RC\SWXIII_PluginDevKit\Platform-SDK\Stellar_SWDC_EC7_SDK.msi
```

**Note:** The 64-bit SDK (`Captivate_PluginSDK_x86_64_v10.00_RC`) has no `Platform-SDK` folder and no MSI installers – it is used only for CS35 (no simulator available per Pasi).

## 3. SDK Installation (32-bit)

1. **Extract** `Captivate_PluginSDK_x86_v10.00_RC.zip` to your chosen folder, e.g.:
   ```
   C:\Users\dell\Desktop\AMS\Development\Captivate_PluginSDK_x86_v10.00_RC
   ```

2. **Run the MSI installers** – See section 2 above. At minimum, run **Stellar_SWDC_EC7_SDK.msi**.

3. **Set environment variable** `SYSTEM1500_ROOT`:
   - Path: `C:\Users\dell\Desktop\AMS\Development\Captivate_PluginSDK_x86_v10.00_RC\SWXIII_PluginDevKit`
   - System or User env var; restart any open terminals/IDE after changing.

4. **Simulator** – Use TS simulator for Captivate 10 (already installed).  
   Optional: SDK also includes `Captivate_v9.97.202` if you need the bundled version.

## 4. SDK Layout (32-bit)

```
Captivate_PluginSDK_x86_v10.00_RC/
├── Captivate_v9.97.202/              ← Optional bundled simulator
└── SWXIII_PluginDevKit/               ← %SYSTEM1500_ROOT%
    ├── Applications/
    │   └── API/                       ← Captivate APIs (Common, GPS, TPS)
    ├── Binary/
    │   ├── Win32/                    ← 32-bit libs (vc90)
    │   │   └── libs/
    │   ├── RCL/                       ← Build output (DLL, .dxx)
    │   └── Object/                    ← Intermediate files
    ├── Documentation/
    │   ├── Captivate-GeoC++GettingStarted.txt
    │   ├── Captivate-GeoC++PluginSDK_API_DOC.zip
    │   └── Captivate-GeoC++v10-Stellar-SDK-ReleaseNotes.pdf
    ├── Projects/                      ← Captivate API source (ABL_*, GeoMath, etc.)
    ├── Platform-SDK/
    │   └── Stellar_SWDC_EC7_SDK.msi   ← Run this
    ├── Samples/                       ← HelloWorld, etc.
    ├── Tools/
    │   ├── MKTools/                   ← MkEdit.exe – deployment packaging
    │   ├── SWXResBuilder/             ← SVG → PNG for icons
    │   └── TextTool/                  ← Text database
    └── Fonts/
```

## 5. Placing DCP06 for the SDK

**DCP06 project layout** (for reference):

```
DCP06/
├── include/dcp06/        # Headers (core, init, orientation, measurement, calculation, file, application)
├── src/                  # C++ source
├── Project/              # Build config – DCP06.dat, DCP06.sys, MSVS/ (DCP06-VS2008.*, DCP06-VS2022.*)
├── Text/                 # Localization (DCP06_TOK.*, .men files)
├── SWXRes_Source/        # SVG icons (DCP06_logo.svg)
├── SWXRes/               # Built PNGs (from SWXRes_Source)
├── docs/                 # DCP06 docs (this file)
└── scripts/              # Build scripts
```

The MkEdit deployment expects DCP06 at:

```
%SYSTEM1500_ROOT%\Applications\Common\DCP06\
```

**Option A – Symlink (recommended for development):**

```powershell
cd %SYSTEM1500_ROOT%\Applications
if (-not (Test-Path Common)) { New-Item -ItemType Directory -Path Common }
New-Item -ItemType Junction -Path "Common\DCP06" -Target "C:\Users\dell\Desktop\AMS\Development\DCP06"
```

**Option B – Copy:**

Copy the entire DCP06 folder (include, src, Project, Text, SWXRes, SWXRes_Source) into `%SYSTEM1500_ROOT%\Applications\Common\DCP06\`. You must copy changes back and forth for edits.

**Option C – Edit XML paths** (advanced):

If you keep DCP06 elsewhere, edit `Project\MSVS\DCP06_Release_Win32.xml` and replace `%SYSTEM1500_ROOT%\Applications\Common\DCP06\` with your actual DCP06 path. For device build, edit `DCP06_Release_Stellar_EC7_3P (ARMV4I).xml` the same way.

---

# Part B: Continuing with the SDK

## 6. Build Configuration

| Config | Platform | Output | Use |
|--------|----------|--------|-----|
| Release\|Win32 | Win32 | DCP06.dll | TS simulator (desktop) |
| Release\|Stellar_EC7_3P (ARMv4I) | ARM | DCP06.dll | Physical device (TS/CS20) |

- **Include paths:** DCP06-VS2008.vcproj already references `$(System1500_ROOT)\Projects\...` for SDK headers.
- **Library path:** `$(SYSTEM1500_ROOT)\Binary\Win32\libs`
- **Output:** `$(SYSTEM1500_ROOT)\Binary\RCL\Win32\Release\`

## 7. Build Steps

1. Set `SYSTEM1500_ROOT` (see section 3).
2. Place DCP06 in `Applications\Common\DCP06` (see section 5; symlink or copy).
3. Open **`DCP06\Project\MSVS\DCP06-VS2008.sln`** (or `DCP06-VS2008.vcproj`) in **VS 2008**.
4. Select **Release | Win32**.
5. Build (F7).

**Post-build (automatic):**

- **SWXResBuilder** – Converts SVG → PNG in `SWXRes_Source/` → `SWXRes/`.
- **MkEdit** – Installs app into simulator and generates loadable `.dxx`:
  - Reg key: `HKLM\Software\Wow6432Node\Leica Geosystems\System 1500\TS\Configuration;DeviceRootPathRelease`
  - Output: `DCP06_dll.pob`, `DCP06_sys.pob`, `DCP06_Res.pob` → packaged for simulator.

## 8. Run in Simulator

1. **Start** the TS simulator (Captivate 10) if not running.
2. Build DCP06 (Release | Win32) – post-build copies the app into simulator.
3. **Launch** Captivate in the simulator; DCP06 appears in the menu (TPS, App.Id=15751).

## 9. Debug in Simulator

1. Build DCP06 (Release | Win32).
2. Start the TS simulator.
3. In VS 2008: **Debug → Attach to Process** → select the Captivate simulator process.
4. Set breakpoints in DCP06 code and use the app in the simulator.

## 10. Reference: HelloWorld Sample and DCP06

SDK sample layout (DCP06 follows the same pattern):

```
Samples/Common/HelloWorldBase/
├── Hdr/                  ← Headers
├── Src/                  ← Source
├── Project/
│   ├── HelloWorld.dat    ← Version metadata
│   ├── HelloWorld.sys    ← App registration (App.Id, EntryPoint)
│   └── MSVS/
│       ├── HelloWorld.vcproj
│       └── HelloWorld_Release_Win32.xml   ← MkEdit config
├── Text/                 ← Localization (.men tokens)
├── SWXRes_Source/        ← SVG icons
└── Help/
```

**DCP06** matches this layout. Key DCP06 files:

- **DCP06.dat** – Version (BuildNr, VersionNr, ReleaseDate).
- **DCP06.sys** – App.Id=15751, EntryPoint, menu placement (TPS).
- **DCP06-VS2008.vcproj** / **DCP06-VS2008.sln** – VS 2008 project/solution (TS/CS20).
- **DCP06_Release_Win32.xml** – MkEdit config for simulator (Win32).
- **DCP06_Release_Stellar_EC7_3P (ARMV4I).xml** – MkEdit config for physical device (TS/CS20).

## 11. Environment Variables Summary

| Variable | Path |
|----------|------|
| `SYSTEM1500_ROOT` | `C:\Users\dell\Desktop\AMS\Development\Captivate_PluginSDK_x86_v10.00_RC\SWXIII_PluginDevKit` |

*Note: Project files use both `$(SYSTEM1500_ROOT)` and `$(System1500_ROOT)`. Windows env vars are case-insensitive; both resolve to the same value.*

## 12. Visual Studio 2008 Setup Troubleshooting

If the VS 2008 setup **closes soon after** clicking "Install Visual Studio 2008", or shows *"Pyydetty toiminto edellyttää korotusta"* (requires elevation), try:

### A. Run as Administrator

1. Browse to `C:\Users\dell\Desktop\AMS\Development\VS2008`
2. **Right‑click** `setup.exe` → **Run as administrator**
3. Click "Install Visual Studio 2008" again.

### B. Bypass missing WCU (workaround)

Your VS 2008 media is missing the `wcu` folder (Windows Component Update – prerequisites). The main launcher looks for `wcu\setup.exe` and exits when it fails.

**1. Ensure prerequisites are installed**

- **.NET Framework 3.5**: Windows Settings → Apps → Optional features → Add a feature → check **.NET Framework 3.5**
- Or: Control Panel → Programs → Turn Windows features on or off → .NET Framework 3.5

**2. Run the Client setup directly (skip Baseline)**

1. Open an **elevated** Command Prompt (Run as administrator).
2. Run:
   ```
   cd /d C:\Users\dell\Desktop\AMS\Development\VS2008
   setup\setup.exe
   ```
3. This runs the VS 2008 Client installer and skips the missing WCU phase.
4. If it still exits quickly or errors, the media may be incomplete and you may need full VS 2008 or SP1 media.

### C. Incomplete media

The full VS 2008 DVD/ISO should include a `wcu` folder at the root with subfolders such as `dotNetFramework`, `msi31`, etc. If your copy has only `setup`, `cab1`–`cab50`, and root files, the WCU phase cannot run.

- **Options:** Obtain a complete VS 2008 ISO, use VS 2008 SP1 layout, or use the workaround in B if .NET 3.5 and other prerequisites are already installed.

---

## 13. Common Issues

| Issue | Check |
|-------|-------|
| "Cannot find SYSTEM1500_ROOT" | Env var set? Restart VS/terminal. |
| MkEdit fails / paths not found | DCP06 at `%SYSTEM1500_ROOT%\Applications\Common\DCP06\`? Project, Text, SWXRes present? |
| MkEdit fails on LangFolder | XML expects `Text\Languages`; create it if missing (see HelloWorld sample). |
| Link errors (missing .lib) | `Binary\Win32\libs` present? Stellar_SWDC_EC7_SDK.msi run? |
| App not in simulator menu | Post-build ran? DCP06.sys correct? Simulator restarted? |

## 14. Documentation in SDK

- `Documentation\Captivate-GeoC++GettingStarted.txt` – SDK overview, install steps
- `Documentation\Captivate-GeoC++PluginSDK_API_DOC.zip` – API reference
- `Documentation\Captivate-GeoC++v10-Stellar-SDK-ReleaseNotes.pdf` – Release notes

*DCP06 project docs:* `docs\DCP_SDK_Setup_Instructions.md` (this file), `Docs\DCP06_Project_Plan.md`, `Docs\README.md`.

---

## 15. 64-bit (CS35) – Secondary

- Per Pasi: CS35 simulator is not available or needs additional licensing.
- If it becomes available: set `SYSTEM1500_ROOT_x64`, use **DCP06-CS.sln** / **DCP06-CS.vcxproj**, build Release|x64.

---

## Quick Checklist (DCP06, TS/CS20)

| Step | Done |
|------|------|
| Extract 32-bit SDK | [ ] |
| Run Stellar_SWDC_EC7_SDK.msi | [ ] |
| Set SYSTEM1500_ROOT | [ ] |
| Install VS 2008 | [ ] |
| Place DCP06 in Applications\Common\DCP06 (symlink/copy) | [ ] |
| Open DCP06-VS2008.sln in VS 2008 | [ ] |
| Build Release Win32 | [ ] |
| Run TS simulator, launch DCP06 | [ ] |

---

*Source: Pasi's email (DCP05 developer; guidance applied to DCP06), Leica GettingStarted.txt, SDK folder analysis. March 2026.*
