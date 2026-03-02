# DCP06 Database Design — Merge from DCP9

**Date:** March 2025  
**Goal:** Merge database functionality from DCP9 into DCP06 with a shared design for easy maintenance.

---

## 1. Executive Summary

- **Source:** DCP9 uses `IDatabase` + `JsonDatabase` (JSON files) with `DatabaseTypes.h`.
- **Target:** DCP06 (Captivate) currently uses ASCII files (.adf, .cdf, etc.) via `AdfFileFunc`, `DCP05ModelC`.
- **Strategy:** Port the DCP9 database design into DCP06 with a **parallel structure** so both projects stay aligned. Future fixes and updates can be applied to both with minimal divergence.
- **Future option:** An AMS database library (shared API) is documented as a potential next step when it proves valuable.

---

## 2. Design Principles

| Principle | Rationale |
|-----------|-----------|
| **Database is primary** | The database (JSON jobs) stores full object/point information. It is the single source of truth. |
| **ADF only for import/export** | ADF is used solely for: (1) Import from legacy DCP05 ADF files, (2) Export to ADF when user requests. No ADF as runtime storage. |
| **Same interface** | DCP06 and DCP9 both implement the same `IDatabase`-style API so fixes and features propagate cleanly. |
| **Same data types** | `DatabaseTypes` (PointData, JobData, alignments) should be structurally identical; only container types differ (Qt vs STL). |
| **Backend-agnostic** | `IDatabase` abstracts storage; DCP9 uses JSON, DCP06 may use JSON or SQLite. Both behave the same. |
| **ADF compatibility** | ADF import/export logic should be shared or duplicated with identical behavior for DCP05 compatibility. |

---

## 3. Current State

### 3.1 DCP9 (Source)

| Component | Implementation |
|-----------|----------------|
| **Interface** | `IDatabase.h` — job, point, alignment, import/export |
| **Types** | `DatabaseTypes.h` — `PointData`, `JobData`, `MidpointData`, `CircleData`, `Alignment321Data`, etc. |
| **Backend** | `JsonDatabase` — one `.json` per job, `.bin` for point clouds |
| **Storage** | `%APPDATA%\A.M.S\DCP9\jobs\` |
| **Dependencies** | Qt (QString, QFile, QJsonDocument, QMap, QVector) |

### 3.2 DCP06 (Target)

| Component | Implementation |
|-----------|----------------|
| **Storage** | `AdfFileFunc` — direct FILE* I/O, fixed-width text |
| **Model** | `DCP05ModelC` — in-memory state, config via `CPI::CFG::ArchiveC` |
| **Paths** | Leica `CPI::` APIs (`GetPath`, device storage) |
| **Dependencies** | Boost, Leica SDK, no Qt |

### 3.3 Differences to Bridge

- DCP9 uses Qt types; DCP06 must use STL (`std::string`, `std::vector`, `std::map`).
- DCP9 stores in AppData; DCP06 stores on instrument (e.g. PC card, Captivate storage).
- Both must support ADF import/export with compatible formats.

---

## 4. Target Architecture

### 4.1 Layered Design (Same in DCP06 and DCP9)

```
┌─────────────────────────────────────────────────────────────────┐
│  Application (DCP06 / DCP9)                                      │
│  - Controllers, UI, workflows                                    │
├─────────────────────────────────────────────────────────────────┤
│  IDatabase (abstract interface)                                   │
│  - createJob, loadJob, saveJob, addPoint, getAllPoints, etc.     │
│  - importFromADF, exportToADF                                    │
├─────────────────────────────────────────────────────────────────┤
│  DatabaseTypes (shared conceptual model)                         │
│  - PointData, JobData, Alignment321Data, BestFitAlignmentData,   │
│    CylinderAlignmentData, ChangeStationData, CircleData, etc.    │
├─────────────────────────────────────────────────────────────────┤
│  Backend (per-project)                                           │
│  - DCP9:  JsonDatabase (Qt)                                      │
│  - DCP06: JsonDatabaseSTL or SqliteDatabase (no Qt)              │
└─────────────────────────────────────────────────────────────────┘
```

### 4.2 IDatabase Interface (Conceptual — Aligned with DCP9)

```cpp
// Common operations (both DCP06 and DCP9)
// Job
createJob(id) -> bool
loadJob(id) -> bool
saveJob() -> bool
deleteJob(id) -> bool
copyJob(from, to) -> bool
swapJob(id1, id2) -> bool

// Points
addPoint(point) -> bool
updatePoint(id, point) -> bool
deletePoint(id) -> bool
getPoint(id) -> PointData*
getAllPoints() -> vector<PointData>

// Alignments (321/DOM, BestFit/POM, Cylinder/COM, ChangeStation/CHST)
// Midpoints, Circles, SurfaceScans
// ... (same set as DCP9)

// Import/Export
importFromADF(path) -> bool
exportToADF(path) -> bool
importFromTXT(path) -> bool
exportToTXT(path) -> bool
// XLS, PRD optional
```

### 4.3 Data Types (Shared Schema)

| Type | Key Fields | Notes |
|------|------------|-------|
| **PointData** | id, x_dsg, y_dsg, z_dsg, x_mea, y_mea, z_mea, x_scs, y_scs, z_scs, hor_angle, ver_angle, distance, note, status | Same semantics as DCP9 |
| **JobData** | id, date, measurer, instrument_id, units, active_orientation, active_chst, points, alignments, ... | Same structure |
| **Alignment321Data** | plane, line, ref_point, matrix | DOM |
| **BestFitAlignmentData** | points_scs, points_ocs, matrix, residuals | POM |
| **CylinderAlignmentData** | center_line, ref_angle, matrix | COM |
| **ChangeStationData** | points_prev, points_new, matrix | CHST |
| **CircleData** | center, normal, diameter, plane_points, circle_points | |
| **SurfaceScanData** | metadata in DB, points in .bin | |

---

## 5. Implementation Plan

### Phase 1: Introduce Database Layer in DCP06 ✅ (March 2025)

| Step | Action | Status |
|------|--------|--------|
| 1.1 | Create `include/dcp06/database/IDatabase.hpp` — abstract interface using **STL types** | Done |
| 1.2 | Create `include/dcp06/database/DatabaseTypes.hpp` — structs matching DCP9 semantics | Done |
| 1.3 | Create `src/database/JsonDatabase.cpp` — JSON backend using nlohmann/json, `std::fstream` | Done |
| 1.4 | Storage: `setDataDirectory()` — use `%APPDATA%\A.M.S\DCP06\jobs\` on CS35, Captivate path on instrument | Done |

### Phase 2a: Model Integration + ADF Bridge ✅ (March 2025)

| Step | Action | Status |
|------|--------|--------|
| 2a.1 | Add `m_pDatabase` (JsonDatabase) to `DCP05ModelC` | Done |
| 2a.2 | `GetDatabase()`, `SetDatabaseDataDirectory()` | Done |
| 2a.3 | Data directory from `CPI::SensorC::GetPath` + `/DCP06/jobs` | Done |
| 2a.4 | **DB-primary:** Open lists jobs from DB, load job → export to work ADF → setFile | Done |
| 2a.5 | **DB-primary:** New creates job in DB, `create_adf_file_at_path` in `.work/` | Done |
| 2a.6 | **DB-primary:** Copy uses `copyJob` in DB, load new job | Done |
| 2a.7 | **DB-primary:** Save (Shift-F3) — working ADF → importFromADF → saveJob | Done |
| 2a.8 | **Import (Shift-F1):** ADF file → importFromADF → create job → save → open | Done |
| 2a.9 | **Export (Shift-F4):** DB → exportToADF to `*_export.adf` | Done |
| 2a.10 | Terminology doc — DCP9 terms (321, BestFit, Cylinder, Change Station) | See `DCP06_Terminology.md` |

### Phase 2: ADF Compatibility (remaining)

| Step | Action |
|------|--------|
| 2.1 | Port ADF import logic from DCP9 (`importFromADF`) — fixed-width parsing, map to `PointData` |
| 2.2 | Port ADF export logic — tab-separated or fixed-width per DCP05 format |
| 2.3 | Align DCP9 ADF format with DCP05 expectations; document differences if any |
| 2.4 | Add round-trip tests: DCP05 ADF → import → export → compare |

### Phase 3: Model Integration (remaining)

| Step | Action |
|------|--------|
| 3.1 | Add `IDatabase*` (or `std::unique_ptr<IDatabase>`) to `DCP05ModelC` — ✅ Done in Phase 2a |
| 3.2 | Replace direct `AdfFileFunc` usage for active session with `IDatabase` calls |
| 3.3 | Keep `AdfFileFunc` for legacy open/save flows initially; migrate gradually |
| 3.4 | Job management: create, load, save, delete, copy — wired to database |

### Phase 4: Keep DCP06 and DCP9 Aligned

| Practice | Description |
|----------|-------------|
| **Interface sync** | When adding a method to `IDatabase`, add to both DCP9 and DCP06. |
| **Type sync** | `DatabaseTypes` changes (new field, rename) — apply to both. |
| **ADF sync** | ADF format changes — update both import/export. |
| **Bug fixes** | Fix in both codebases; consider cross-referencing in commit messages. |
| **Docs** | Keep `DCP06_Database_Design.md` and a parallel doc in DCP9 (`DCP9_Database_Design.md` or section) in sync. |

---

## 6. Future: AMS Database Library

### 6.1 When to Consider

- When DCP06 and DCP9 both need the **same fix** repeatedly.
- When a **third product** (e.g. new app) needs the same database API.
- When **testing** and **CI** would benefit from a shared lib.

### 6.2 What It Would Contain

| Component | Description |
|-----------|-------------|
| **ams_db** (or similar) | Static or shared library |
| **IDatabase** | Pure abstract interface, STL types |
| **DatabaseTypes** | POD structs + STL containers |
| **JsonDatabase** | Default implementation, no Qt |
| **ADF I/O** | `importFromADF`, `exportToADF` as free functions or `IDatabase` methods |
| **Optional backends** | SQLite adapter, in-memory for tests |

### 6.3 Adoption Path

1. **Now:** Implement in DCP06, keep DCP9 as reference. Design for similarity.
2. **Later:** Extract common code into `ams-database` repo; DCP06 and DCP9 link to it.
3. **DCP9 migration:** Replace Qt-based `JsonDatabase` with `ams-database` + Qt bindings if needed, or pure STL.

### 6.4 Not Required Now

- No new repository.
- No shared build system.
- Focus on **design alignment** and **parallel implementation**.

---

## 7. Summary

| Item | Decision |
|------|----------|
| **Merge approach** | Port DCP9 database design to DCP06; same interface and types. |
| **Backend** | DCP06: JSON (STL) or SQLite; DCP9: JSON (Qt) — both behind `IDatabase`. |
| **Maintenance** | Keep DCP06 and DCP9 database layers parallel; fix/update both together. |
| **AMS library** | Document as future option; implement when duplication or third product justifies it. |
| **ADF** | Shared format; import/export logic aligned between projects. |

---

## 8. Sync Checklist (DCP06 ↔ DCP9)

When making database-related changes, apply to **both** projects:

- [ ] **IDatabase** — New method? Add to both DCP06 and DCP9 interfaces.
- [ ] **DatabaseTypes** — New field/struct? Update both `DatabaseTypes.h` (or equivalent).
- [ ] **ADF format** — Import/export change? Verify round-trip in both; update both implementations.
- [ ] **Bug fix** — Fix in one? Apply same fix in the other; reference commit in PR/issue.
- [ ] **Docs** — Update `DCP06_Database_Design.md`; if DCP9 has `DCP9_Database_Design.md`, keep it in sync.

---

*Document created: March 2025. To be updated as implementation progresses.*
