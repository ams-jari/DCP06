# DCP9 Technical Summary

**Document Purpose:** Detailed summary of DCP9 for DCP06 development planning.  
**Source:** C:\Users\dell\Desktop\AMS\Development\DCP9  
**Product Type:** Windows tablet application for Leica laser tracker control.

---

## 1. Overview and Product Role

DCP9 is a **native Windows x64 C++ application** for controlling Leica laser trackers. It targets **Windows tablets** and measurement workflows (e.g. shipbuilding, paper machinery). Unlike DCP05 (onboard total station plugin), DCP9 runs on a separate PC/tablet and communicates with the tracker via network (IP or simulator).

| Aspect | Details |
|--------|---------|
| **Platform** | Windows x64 (tablet/desktop) |
| **Instruments** | AT403, AT500, AT960 (physical + simulator) |
| **Connection** | IP-based, LMF (Leica Measurement Framework) |
| **Output** | DCP9.exe (standalone application) |

---

## 2. Project Structure

```
DCP9/
├── include/           # Headers (database, ui, core, geometry, cad, lmf, utils)
├── src/               # Implementation
│   ├── database/      # JsonDatabase
│   ├── cad/           # DXF, STEP, IGES parsers
│   ├── core/          # Geometry, workflows, alignment
│   ├── ui/            # Bridges, dialogs, widgets
│   ├── lmf/           # TrackerConnectionManager
│   └── tests/
├── ui/                # Qt Designer .ui files
├── LMF/               # Leica LMF assemblies
├── libdxfrw/          # DXF library
├── QXlsx/             # Excel I/O
├── Docs/              # Implementation notes, plans
├── Scripts/           # build_msi.ps1, generate_moc_files.ps1, prepare_*.ps1
├── Installer/         # WiX (Product.wxs, Bundle.wxs)
└── ReleasePackage/    # Deployment layout
```

---

## 3. Technology Stack

| Component | Technology |
|-----------|-------------|
| **GUI Framework** | Qt 6.8.2 + MFC hybrid (Qt for UI, MFC for legacy/CLR integration) |
| **Language** | C++17, C++/CLI (for LMF .NET interop) |
| **Build** | MSBuild (Visual Studio v143), vcpkg for dependencies |
| **Installer** | WiX 3.x (MSI + Burn bundle with VC++ Redist prerequisite) |
| **Package Mgmt** | vcpkg with manifest mode |

### Dependencies

- **vcpkg:** gtest, eigen3, opencascade
- **Qt 6:** Core, Gui, Widgets, UiTools, OpenGL, OpenGLWidgets
- **Third-party:** QXlsx, libdxfrw (DXF), dxfrw.lib
- **LMF:** `LMF.Tracker.Connection.dll` (.NET 4.8)

---

## 4. Database Design

### 4.1 Conceptual Model

DCP9 uses a **JSON-based database** (not SQL/ORM). One job = one `.json` file. Point clouds stored in external `.bin` files to keep JSON manageable.

| Aspect | Implementation |
|--------|----------------|
| **Interface** | `IDatabase` (abstract) |
| **Implementation** | `JsonDatabase` |
| **Storage location** | `%APPDATA%\A.M.S\DCP9\jobs\` |
| **Job file** | `{jobId}.json` |
| **Point clouds** | `JobId_scan_Scan01.bin` (external) |
| **Limit** | `MAX_POINTS_PER_JOB = 1000` |

### 4.2 IDatabase Interface

```cpp
// Job operations
createJob, loadJob, saveJob, deleteJob, copyJob, swapJob

// Point operations
addPoint, updatePoint, deletePoint, getPoint, getAllPoints

// Midpoint, Circle, SurfaceScan operations
add/update/delete/get for each type

// Alignment operations
ShaftAlignment, ChangeStation, 321Alignment, CylinderAlignment, BestFitAlignment

// File import/export
importFromADF, exportToADF, importFromTXT, exportToTXT, 
importFromXLS, exportToXLS, importFromPRD, exportToPRD
```

### 4.3 Data Types (DatabaseTypes.h)

| Entity | Key Fields |
|--------|------------|
| **JobData** | id, date, measurer, instrument_id, units, active orientation/change station, settings (target, decimals, temperature, etc.) |
| **PointData** | x_dsg/y_dsg/z_dsg (design), x_mea/y_mea/z_mea (measured), x_scs/y_scs/z_scs (SCS), hor_angle, ver_angle, distance, precision, dev_x/y/z, prism |
| **Alignment321Data** | plane (points, normal), line (points, direction), reference point, offsets, transformation matrix |
| **CylinderAlignmentData** | center line, reference angle, origin, matrix |
| **BestFitAlignmentData** | points_scs, points_ocs, points_residuals, matrix |
| **ChangeStationData** | points_prev, points_new, points_residuals, matrix |
| **SurfaceScanData** | metadata in JSON; points in external .bin |
| **CircleData** | center, normal, diameter, plane points, circle points |

---

## 5. Main Features

### 5.1 Tracker Control

- Connect via IP or simulator (AT403, AT500, AT960)
- LMF TrackerConnectionManager wraps LMF .NET API

### 5.2 Measurements

- Point, midpoint, circle, surface scan (point clouds)
- OVC (On-Video Capture)
- Positioning (aim to coordinates, orientation to gravity)
- Probe trigger (hardware measurement trigger)

### 5.3 Alignments

| Type | Description |
|------|-------------|
| **321 (DOM)** | Plane + line + reference point |
| **Best Fit (POM)** | Point-only (≥3 pairs) |
| **Cylinder (COM)** | Center line + reference angle + optional origin |
| **Change Station** | Relocation transformation |

### 5.4 Other Features

- Shaft alignment (plane, line, circle measurements)
- CAD import: DXF (libdxfrw), STEP, IGES (OpenCASCADE)
- Export: ADF, TXT, XLS, PRD

---

## 6. Architecture

### 6.1 Application Structure

```
DCP9.cpp (CWinApp) - MFC entry, Qt event loop in PreTranslateMessage
    ├─► CSampleSheet (hidden MFC PropertySheet) - hosts MFC PropPages
    │     └─► CPropPageMeasurement, CPropPageSettings, OVC, Trigger, Positioning
    └─► MainUIManager (Qt) - loads DCP9.ui, manages stacked pages
              ├─► Bridge classes (QObject) - connect Qt UI ↔ LMF/Database
              └─► GlobalStatusBar / GlobalStatusBarManager
```

### 6.2 Bridge Pattern

Bridge classes bind Qt widgets (from .ui files) to logic, call `TrackerConnectionManager` (LMF) and `JsonDatabase`, and use Qt signals/slots for async updates.

**Examples:** MeasureBridge, DROBridge, Alignment321Bridge, BestFitAlignmentBridge, CylinderAlignmentBridge, ChangeStationBridge, CircleBridge, MidpointBridge, OVCBridge, PositioningBridge, ProbeTriggerBridge, SettingsBridge, JobsBridge.

### 6.3 LMF Integration

- **ManagedWrapper** (C++/CLI): Holds LMFTracker, MFC pages; handles events (`OnMeasurementArrived`, `OnTargetSelectedChanged`, etc.)
- **TrackerConnectionManager**: Static API in native C++ wrapping `ManagedWrapper::LMFTracker`
- **Measurement flow:** LMF → OnMeasurementArrived → LastMeasurement → MFC/Qt bridges

### 6.4 Geometry & Workflows

- **Geometry:** Point, Plane, Line, Circle; PlaneFitting, LineFitting, CircleFitting, AlignmentCalculator, Transformations
- **Workflows:** Alignment321Workflow, BestFitAlignmentWorkflow, CylinderAlignmentWorkflow, ChangeStationWorkflow

---

## 7. UI Structure

### 7.1 Navigation

- **Sidebar** (120px): Home, Jobs, Settings, Align, Measure, Tools, Exit
- **Content:** `QStackedWidget` with per-page UIs
- **Pages:** MainMenu, FileMenu, JobsPage, SettingsPage, MeasurePage (tabs), AlignPage (321, Best Fit, Cylinder, Change Station), ToolsPage, DesignPointsPage, ShaftPage

### 7.2 Layout

- Main window: **1024×630** minimum, **1280×730** default
- Sidebar buttons: height **56px** (touch-friendly)
- Font: Segoe UI 10pt
- No close button (X); Exit via Home page to avoid accidental shutdown

### 7.3 Status Bar

- Instrument (connection, init)
- Measurement status (ready/not ready)
- Info roller: job, orientation, change station, battery, target

---

## 8. Build System

### Build Flow

1. **Pre-build:** `Scripts\generate_moc_files.ps1` for Qt meta-objects
2. **Compile:** MSBuild, C++17, x64 Release/Debug
3. **Post-build (Debug):** `Scripts\prepare_debug_package.ps1`
4. **Release package:** `Scripts\prepare_release_package.ps1` (windeployqt)
5. **MSI:** `Scripts\build_msi.ps1` (WiX heat/candle/light, VC++ Redist bundle)

### Configuration

- **Qt:** `C:\Qt\6.8.2\msvc2022_64`
- **vcpkg:** `$(SolutionDir)..\vcpkg`
- **Preprocessor:** `HAVE_OPENCASCADE`, `HAVE_LIBDXFRW`
- **Version:** `include/Version.h` (current: 1.0.1)

---

## 9. Key Takeaways for DCP06

1. **Database concept** – JSON-based job storage with `IDatabase` abstraction; replace DCP05 ASCII file system.
2. **Modern UI** – Qt 6 + tablet-oriented design; DCP06 on Captivate will differ but workflow patterns (Jobs, Settings, Measure, Align) are reusable.
3. **Alignment terminology** – 321 (DOM), Best Fit (POM), Cylinder (COM), Change Station; align with DCP05 concepts.
4. **ADF import/export** – DCP9 already supports ADF; reuse for DCP05 compatibility.
5. **Bridge pattern** – Clean separation between UI and logic; applicable to DCP06 controller design.
6. **CAD import** – DXF, STEP, IGES; consider for DCP06 if Captivate platform allows.
7. **Point limit** – 1000 points per job in DCP9; consider for DCP06 or increase with proper DB backend.
