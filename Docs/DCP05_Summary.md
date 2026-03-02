# DCP05 Technical Summary

**Document Purpose:** Detailed summary of DCP05 for DCP06 development planning.  
**Source:** C:\Users\dell\Desktop\AMS\Development\DCP05  
**Product Type:** Onboard 3D measurement software for Leica total stations with Captivate.

---

## 1. Overview and Product Role

DCP05 is a **Leica Captivate TPS (Total Station Positioning) plugin** that runs directly on the instrument (WinCE/ARM) or in the CS35 simulator (Windows x64). It provides 3D measurement, orientation, alignment, and calculation workflows for surveying and construction applications.

| Aspect | Details |
|--------|---------|
| **Application ID** | 15750 |
| **Output** | DCP05.dll (loadable Captivate plugin) |
| **Platform** | Leica System 1500 (MS60, TS16, TS60, CS20 physical; CS35 simulator) |
| **OS** | WinCE (ARM) on instrument, Windows (x86/x64) on simulator |

---

## 2. Project Structure

```
DCP05/
├── Docs/                    # Technical documentation (13 MD files)
├── Hdr/                     # ~72 header files (.hpp)
├── Project/                 # Build configuration
│   ├── DCP05.dat           # Version/build metadata
│   ├── DCP05.sys           # Captivate app registration
│   └── MSVS/               # Visual Studio projects
├── Src/                     # ~102 C++ source files (.cpp)
│   └── math/               # 34 internal math library modules
├── SWXRes/                  # UI resources (PNG icons)
├── SWXRes_Source/          # Source assets (SVG, Config.xml)
└── Text/                    # Localization (~628 tokens)
```

---

## 3. Technology Stack

| Layer | Technology |
|-------|-------------|
| **Language** | C++ |
| **Platform SDK** | Leica Captivate System 1500 |
| **Build system** | MSVC (VS 2008 / VS 2022) |
| **External libs** | Boost (filesystem, WinCE adapter), Leica SDK libs |
| **UI Framework** | Captivate GraphMenuDialog, ComboLineCtrl, F1–F6, Shift+F keys |

### Dependencies

- ABL_Base, ABL_Tps, ABL_Manage, ABL_Scanning, ABL_System  
- Common, GeoMath, TpsFacade  
- GuiPlus, HALTools, ScanData, SensorData, Utilities, GSV  
- boost-WinCEAdapter (ARM), wbemuuid (CS35 WMI)

---

## 4. Architecture

### MVC Pattern

- **Model:** `DCP05ModelC` – central state (config, orientation, points, matrices) extending `ABL::AppConfigModelC`
- **View:** Captivate GraphMenuDialog, icon grids, ComboLineCtrl
- **Controller:** `DCP05ControllerC` (root), `DCP05ConfigControllerC` (config persistence)

### Entry Point

- `Start15750` (from `DCP05.sys`) via ABL framework
- `APP_ENTRY_POINT(AT_DCP05, 15750, DCP::DCP05ApplicationC)`

### Coordinate Systems

| Abbrev | Name | Purpose |
|--------|------|---------|
| DCS | Station | Station coordinate system |
| OCSP | Plane | Plane coordinate system |
| OCSD | Design | Design coordinate system |
| OCSC | Circle | Circle coordinate system |
| OCSU | User | User coordinate system |

4×4 matrices: `dcs_matrix`, `ocsd_matrix`, `ocsp_matrix`, `ocsc_matrix`, `ocsu_matrix` (and inverses).

---

## 5. User Functionality (Main Menu)

### 5.1 Initialization (INIT_DLG)

- Target type (prism, etc.)
- User name
- Units (mm, inch, feet)
- Atmospheric correction (PPM)
- Face I/II measurement (All, Dist, etc.)
- Design values, overwrite info, tool info
- Auto increment, average count
- File storage (SD card vs local)

### 5.2 Orientation

| Method | Description |
|--------|--------------|
| **321 Alignment** | Define Orientation Method: Plane (XY/ZX/YZ, horizontal, 3–20 points), Line (X/Y/Z, 2–20 points), Point Offset, Point Measure, Rotate Plane/Line, Calculation |
| **Best Fit Alignment** | Point Orientation Method: Select object points in OCS, measure in SCS, calculation, file or point input, RMS/residuals display |
| **CHST** | Change Station: Define transfer points, select file or measure points |
| **User Defined** | Custom orientation workflows |

### 5.3 Measurement

| Function | Description |
|----------|-------------|
| XYZ | Full 3D point (face 1/2, average) |
| Distance | Single-axis distance |
| Angle | Angle measurement |
| Hidden Point | Hidden point mode |
| X/Y/Z only | Single-axis measurement |
| Circle | Circle fitting |
| Separate Recording | Separate recording mode |
| Scan | Point cloud scanning (DCP_ScanMeasXYZ, DCP_PlaneScanning, DCP_AdvScan) |
| Mid-Point | Midpoint calculation |
| Home Points | Reference points |

### 5.4 Calculation

- Distance calculation
- Angle calculation
- Line fit, Plane fit, Circle fit
- Best-fit transformations

### 5.5 Application

- Line setting
- Shaft alignment
- Plane scanning
- Line fitting

### 5.6 File

- ADF, CDF, AGF, CRL, SFT file operations
- File selection, import/export

---

## 6. ASCII File System (Critical for DCP06 Migration)

### 6.1 Overview

| Aspect | Current DCP05 | Notes for DCP06 |
|--------|---------------|-----------------|
| **Encoding** | ASCII only | Consider UTF-8, Unicode |
| **Primary format** | ADF (AMS proprietary) | Replace with database; maintain ADF import/export |
| **I/O API** | C stdio (FILE*, fopen, fgets, fputs) | Modern streams, database persistence |
| **Storage path** | `CPI::ftUserAscii` via `CPI::SensorC` | Check Captivate SDK; integrate with database |
| **File discovery** | `boost::filesystem::directory_iterator` | N/A if database-based |

### 6.2 File Formats

| Extension | Name | Purpose | Handler |
|-----------|------|---------|---------|
| **.adf** | ADF | 3D points (actual + design), **main format** | AdfFileFunc |
| .sta | Station | Station data (ADF-style) | AdfFileFunc |
| .scn | Scan | Scan point cloud | AdfFileFunc, ScanFileFunc |
| .bft | Best-fit | Best-fit data | AdfFileFunc |
| .cdf | Distance | Distance calc results | CdfFileFunc |
| .agf | Angle | Angle calc results | AgfFileFunc |
| .crl | Circle | Circle fit results | CircleFileFunc |
| .sft | Shaft | Shaft alignment | ShaftFileFunc |
| .ref | Reference | Design import (tab-separated) | — |
| .dat | Data | Actual export (tab-separated) | — |

### 6.3 ADF Format Specification

- **Encoding:** ASCII  
- **Line ending:** CR+LF  
- **Record length:** 81 chars per line (80 + CRLF)

**Point Record Layout (81 chars):**

| Offset | Length | Field | Description |
|--------|--------|-------|-------------|
| 0 | 6 | PID | Point ID (e.g. "P1   ", "P1F  ", "P1B  ") |
| 7 | 1 | Xsta | 'X'=measured, '_'=not defined |
| 9 | 9 | Xact | X actual (measured) |
| 19 | 9 | Xdes | X design |
| 29 | 1 | Ysta | Y status |
| 31 | 9 | Yact | Y actual |
| 41 | 9 | Ydes | Y design |
| 51 | 1 | Zsta | Z status |
| 53 | 9 | Zact | Z actual |
| 63 | 9 | Zdes | Z design |
| 73 | 6 | Note | Note field |

**Point types:** Single, Front (`F`), Back (`B`) pairs stored consecutively.

### 6.4 Config File References (Model)

- `sPomFile`, `sChstFile`, `s3DFile`, `sCalcDistFile`, `sCalcAngleFile`, `sCircleFile`, `sShaftFile`

---

## 7. Main Modules and Relationships

| Group | Role | Main Components |
|-------|------|-----------------|
| **Init** | User, unit, target, atm, face, tools | DCP_DCP05Init, DCP_DCP05User, DCP_DCP05Unit |
| **Orientation** | 321 Alignment, Best Fit, change station | DCP_DCP05Dom, DCP_DCP05Pom, DCP_DCP05Chst |
| **Measurement** | XYZ, Dist, Angle, special modes | DCP_MeasXYZ, DCP_ScanMeasXYZ, DCP_Circle |
| **Calculation** | Dist, Angle, Line/Plane/Circle fit | DCP_CalcDist, DCP_CalcAngle, DCP_CalcLineFit |
| **Application** | Line setting, shaft, scanning | DCP_LineSetting, DCP_Shaft, DCP_PlaneScanning |
| **File** | ADF, CDF, AGF, CRL, SFT, selection | DCP_AdfFileFunc, DCP_SelectFile, DCP_File |
| **Math** | Line/plane/circle fitting, transforms | Src/math/ (FITLINE, FITPLANE, ccircle, MAT*, etc.) |

### Data Flow

- `DCP05ModelC` – central state
- Controllers – navigation and workflows
- `DCP05ConfigControllerC` – load/save model via `CPI::CFG::ArchiveC`

---

## 8. Leica API Integration

| API | Purpose |
|-----|---------|
| **TPI** | `TPI::SensorC`, `TPI::InstrumentInfoC` – instrument capabilities, serial |
| **CPI** | Paths, config, `CPI::deviceSdCard` / `CPI::deviceLocalMemory` |
| **TBL** | Measurement flow, `TBL::MeasurementC`, `TBL::SurveyModelC`, error handling |
| **ABL** | App registration, config model |

---

## 9. Build System

### Target Platforms

| Platform | Solution | Toolset | Preprocessor |
|----------|----------|---------|--------------|
| Physical (MS60, TS16, CS20) | DCP05.sln | VS 2008 | HW_ARM, OS_WINCE |
| Desktop 32-bit | DCP05.sln | VS 2008 | HW_X86, PLAT_WINPC_X86 |
| CS35 Simulator | DCP05-CS.sln | VS 2022 v143 | CS35, WIN64, PLAT_WINPC_X64 |

### Configuration

- **DCP05.dat:** BuildNr=101, VersionNr=10, ReleaseDate=01.01.2013
- **DCP05.sys:** App.Id=15750, EntryPoint=Start15750, TPS type, logo
- **Environment:** `SYSTEM1500_ROOT`, `SYSTEM1500_ROOT_x64`, `INCSCAPE`

---

## 10. Documentation Index (DCP05 Docs)

| Document | Subject |
|----------|---------|
| 00_Overview.md | Product overview, version |
| 01_Project_Structure.md | Directories, build setup |
| 02_Architecture.md | MVC, entry points |
| 03_Modules_Reference.md | Module catalog |
| 04_Source_Files_Catalog.md | Source file list |
| 05_External_Dependencies.md | SDK, libraries |
| 06_User_Functionality.md | User features, menus |
| 07_Data_Structures.md | Types, enums |
| 08_Configuration_And_Build.md | Build configs |
| 09_DCP06_Migration_Notes.md | DCP06 migration notes |
| 10_File_Handling_Deep_Dive.md | File formats, ADF |
| 11_Leica_Captivate_API_Deep_Dive.md | Leica/Captivate APIs |
| 12_UI_Design_Captivate_Deep_Dive.md | Captivate UI patterns |

---

## 11. Key Takeaways for DCP06

1. **Preserve all user functionality** – Init, Orientation (321 Alignment, Best Fit, CHST), Measurement, Calculation, Application, File.
2. **ASCII → Database** – DCP05 file system is marked for total redesign; DCP06 should use database concept.
3. **ADF compatibility** – Maintain ADF import/export for backward compatibility.
4. **Coordinate systems** – Keep DCS, OCSP, OCSD, OCSC, OCSU and 4×4 matrices.
5. **Captivate UI** – Must follow GraphMenuDialog, ComboLineCtrl, function keys (not Windows-style).
6. **Leica APIs** – TPI, CPI, TBL, ABL are critical; plan SDK migration if newer Captivate.
7. **Math library** – Consider refactoring or replacing with Eigen; validate against DCP05.
