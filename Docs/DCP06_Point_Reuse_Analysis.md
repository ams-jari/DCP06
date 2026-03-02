# DCP06 Point Reuse Analysis — Merge from DCP9

**Date:** March 2025  
**Goal:** Enable reuse of already measured points in DCP06 using the database, like DCP9, while overcoming DCP05/ADF limitations.

---

## 1. Why DCP05 Could Not Reuse Points

| Reason | Detail |
|--------|--------|
| **1. Era** | DCP05 designed in the 1990s; point reuse was not a primary use case. |
| **2. ADF precision loss** | ADF stores coordinates with limited decimals (~9 chars, e.g. `%9.3f`); full instrument precision is lost. |
| **3. No SCS in ADF** | ADF stores only measured (x_mea/y_mea/z_mea) and design (x_dsg/ydsg/z_dsg) in OCS/DCS. **Instrument coordinates (x_scs, y_scs, z_scs)** are never written. |
| **4. Limited point metadata** | ADF: PID (6 chars), note (6 chars), x/y/z only. No angles, distance, prism, orientation, etc. |
| **5. File-based design** | Each open/save reads/writes ADF; no shared in-memory point registry for reuse across features. |

---

## 2. How DCP9 Benefits from Point Reuse (Source: `C:\Users\dell\Desktop\AMS\Development\DCP9`)

DCP9 uses a **JSON database** as the primary store:

- **PointData** includes: `x_dsg`, `y_dsg`, `z_dsg`, `x_mea`, `y_mea`, `z_mea`, **`x_scs`, `y_scs`, `z_scs`**, `hor_angle`, `ver_angle`, `distance`, `precision`, `prism`, etc.
- **Full precision** — no truncation to 9.3f.
- **SCS values** stored for change-station, best-fit, and other alignment workflows that need raw instrument coordinates.
- **IDatabase** provides: `getAllPoints()`, `getPoint(id)` — callers pick real stored values instead of re-reading ADF.
- **Alignments** (321/DOM, Best Fit/POM, Cylinder/COM, Change Station) use points from the DB with full data.

DCP06 already has `DatabaseTypes.hpp` aligned with DCP9 (`PointData` with `x_scs`, etc.) and `JsonDatabase` with `getAllPoints()` / `getPoint()`. The missing piece is **wiring the existing features to use the DB for point lists and point data** instead of ADF.

---

## 2.1 DCP9 Source: SelectPointsDialog (Primary Reuse Mechanism)

**File:** `DCP9/src/ui/SelectPointsDialog.cpp`, `include/ui/SelectPointsDialog.h`

DCP9 uses a single shared **SelectPointsDialog** for picking existing points from the database. It is used by:

| Feature | Context | `requireDesignValues` | Purpose |
|---------|---------|----------------------|---------|
| **Alignment321** | AvailableForPlane, AvailableForLine, AvailableForReference, PlanePoints, LinePoints, ReferencePoints | varies | Plane, line, reference point selection |
| **BestFit** | AllPoints | true (design) / false (actual) | Design points, actual points |
| **Cylinder** | AllPoints | false | Center line, reference angle, origin |
| **ChangeStation** | AllPoints | false | Points for prev/new station |
| **Circle** | AllPoints | false | Points for circle |
| **Midpoint** | AllPoints | false | Points for midpoint |
| **MeasureBridge** | AllPoints | false | **"Select Point (PID)"** — load existing point into main measure |

### Key DCP9 Implementation Details

1. **getAllPointsInJob()** (`JsonDatabase.cpp` ~line 2060)  
   Returns points from *every source* in the job:
   - Main point list
   - 321 alignments (plane, line, reference)
   - Best Fit `points_scs`
   - Cylinder (center_line, reference_angle, origin)
   - Change Station (points_prev, points_new)
   - Midpoints (constituent points)
   - Circles (circle points, plane points)  
   Dedupes by ID; only includes points with actual values (`pointHasActualValues`).

2. **Filtering for actual values** (`SelectPointsDialog::populatePointsTable` ~line 297):  
   When `requireDesignValues == false`, filters to points with valid SCS:
   ```cpp
   bool hasAllActualValues = !std::isnan(pt.x_scs) && !std::isnan(pt.y_scs) && !std::isnan(pt.z_scs);
   ```
   **SCS is required for point reuse in “actual value” mode.**

3. **getSelectedPoints()** returns full `QList<PointData>`. For display, applies `selectDialogGetDisplayCoords` — transforms SCS to OCS if an active alignment exists. Returned points have `x_mea, y_mea, z_mea` set to display coords.

4. **MeasureBridge::OnPidButtonClicked** (“Select Point”):  
   - Opens SelectPointsDialog  
   - If selected point exists only in an alignment (not in main list), adds it to main list via `addPoint`  
   - Calls `LoadPointById(pointId)` to load into DRO/main measure  
   This is the main “reuse already measured point” flow in Measure.

5. **Point order**: Points sorted numerically by ID (P1, P2, … P10, P11).

---

## 3. DCP06 Current Point Selection Flows

### 3.1 Data Flow (ADF path)

```
AdfFileFunc::GetPointList()  →  S_SELECT_POINTS[]  (id, bActualDefined, bDesignDefined, …)
         ↓
User picks point (e.g. index 3)
         ↓
AdfFileFunc::form_pnt(3)     →  xmea_front, ymea_front, zmea_front, xdes_front, ...
         ↓
Caller uses atof(xmea_front) etc.  →  S_POINT_BUFF or calculation inputs
```

**Limitation:** All coordinate data comes from ADF (truncated, no SCS).

### 3.2 Call Sites That Need Point Reuse

| Feature | File | Uses | Point Source |
|---------|------|------|--------------|
| **Best Fit (select points)** | `DCP_PomSelectPoints.cpp` | `GetPointList`, `form_pnt1` | File selector: current job or external ADF |
| **CalcDist** | `DCP_CalcDist.cpp` | `GetPointList`, `form_pnt`, `form_pnt1` | User-selected 3D file (job or ADF) |
| **CalcAngle** | `DCP_CalcAngle.cpp` | `GetPointList`, `form_pnt`, `form_pnt1` | User-selected 3D file |
| **CalcCircle** | `DCP_CalculationCircle.cpp` | `GetPointList`, `form_pnt1` | User-selected 3D file |
| **3D File View** | `DCP_3dFileView.cpp` | `GetPointList`, `form_pnt` | Current job's `m_pAdfFile` |
| **Offset** | `DCP_Offsv.cpp` | `GetPointList`, `form_pnt1` | Design points from file |
| **DOM UserDef plane/line** | `DCP_DefinePlaneUserDef`, `DCP_DefineLineUserDef` | `select_point_list` | From DOM selection flow |
| **Select Point (PID)** | — | *Not yet in DCP06* | DCP9: MeasureBridge "Select Point" loads existing point into main measure |

### 3.3 S_SELECT_POINTS / form_pnt Contract

- **S_SELECT_POINTS**: `iId`, `point_id`, `bActualDefined`, `bDesignDefined`, `bActualSelected`, `bDesignSelected`
- **form_pnt(index)** / **form_pnt1(index, …)**: Fills `xmea_front`, `ymea_front`, `zmea_front`, `xdes_front`, ... from the point at that index. Callers read these into `S_POINT_BUFF` or calculation structs.

---

## 4. Database vs ADF — When to Use Which

| Scenario | Use DB | Use ADF |
|----------|--------|---------|
| Current job loaded, points from current job | ✓ | |
| User selects *another* job from Jobs list | ✓ | |
| User selects external legacy ADF file | | ✓ |
| 3D File View (always current job) | ✓ | |
| POM: "Select from file" = current job | ✓ | |
| POM: "Select from file" = external ADF | | ✓ |
| CalcDist/Angle/Circle: selected file = current job | ✓ | |
| CalcDist/Angle/Circle: selected file = external ADF | | ✓ |
| Offset: design points from current job | ✓ | |
| Offset: design points from external file | | ✓ |

---

## 5. Prerequisite: Persist Full Point Data to DB

Today, points reach the DB **only via** `importFromADF` on Save. That means:

- Precision comes from the working ADF (truncated).
- SCS is faked: `pt.x_scs = pt.x_mea` (import has no real SCS).

To get true point reuse with full data:

1. **On measurement:** When a point is measured, call `addPoint` / `updatePoint` with full `PointData` (including `x_scs` from `to_xyz`).
2. **On Save:** Continue to sync working ADF → DB, but treat DB as primary; ADF export truncates for compatibility only.
3. **Optional:** If we keep the ADF-centric save for now, we can still use DB for point list/coordinates **when the job was loaded from DB** and the work ADF was exported from DB — we'd have full precision in DB from a previous "direct to DB" measurement pass. For Phase 1 of point reuse, we can use whatever is in DB (even if SCS is currently approximated).

---

## 6. Feasible Merge Plan (Enhanced from DCP9 Source)

### Phase A: Database-Backed Point Provider (Aligned with DCP9)

**A1. Add `getAllPointsInJob()` to DCP06 JsonDatabase** (DCP9 pattern)

DCP9's `getAllPointsInJob()` aggregates points from main list + alignments + circles + midpoints + change stations. DCP06 currently has only `getAllPoints()` (main points). For full parity with DCP9's "select any measured point" behavior:

```cpp
// In IDatabase.hpp / JsonDatabase
std::vector<std::shared_ptr<PointData>> getAllPointsInJob() const;
```

Implementation: when DCP06 has alignment/circle/midpoint/change-station storage, aggregate their points; otherwise return `getAllPoints()`. Dedupe by ID; only include points with actual values (x_mea or x_scs).

**A2. Add DB-backed `GetPointList` and `getPointByIndex`** (compat with DCP06 callers)

To avoid changing all call sites at once, add methods that mirror `AdfFileFunc::GetPointList` and `form_pnt1`:

```cpp
// In JsonDatabase or a helper used by callers
int getPointListAsSelectPoints(S_SELECT_POINTS* pList, int maxPoints, int def);  // def: BOTH/ACTUAL/DESIGN
bool getPointByIndex(int index, bool useActual, char* pid, char* xmea, char* xdes, char* ymea, char* ydes, char* zmea, char* zdes);
// Alternative: getPointById(pointId, useActual, PointData& out) — callers can then fill S_POINT_BUFF
```

- **getPointListAsSelectPoints**: Iterate `getAllPointsInJob()` (or `getAllPoints()` initially), map to `S_SELECT_POINTS`. Filter: for ACTUAL, require valid x_scs/y_scs/z_scs (DCP9 pattern); for DESIGN, require x_dsg/ydsg/z_dsg. Sort by ID (numeric).
- **getPointByIndex**: Use the same ordered list; fill buffers from `PointData` (x_mea/x_dsg as per `useActual`).

### Phase B: Branch in Callers — DB vs ADF

For each call site in §3.2:

1. **Determine source:** Is the point source the current job (or a DB job)?
   - e.g. `m_currentJobId == jobId` and `db->loadJob(jobId)` succeeded, or
   - selected file path equals `jdb->getJobWorkingPath(m_currentJobId)`.
2. **If DB path:** Call `getPointListFromCurrentJob` / `getPointFromCurrentJobByIndex` instead of `GetPointList` / `form_pnt1`.
3. **If ADF path:** Keep existing `AdfFileFunc::GetPointList` and `form_pnt` / `form_pnt1`.

**3D File View** is special: it always uses the current job. It can use the DB path whenever `m_currentJobId` is set and the job is loaded.

### Phase C: Measurement → DB (Full Data)

When a point is measured (e.g. in `DCP_DCP05Meas` or equivalent):

1. Build `PointData` with `x_mea`, `y_mea`, `z_mea`, `x_scs`, `y_scs`, `z_scs` (from `to_xyz`), angles, distance, etc.
2. Call `db->addPoint(pointId, pt)` or `db->updatePoint(pointId, pt)`.
3. Keep writing to the working ADF for legacy flows, but DB becomes the source of truth for reuse.

This ensures SCS and full precision are available for reuse.

---

## 7. Implementation Order

| Step | Description |
|------|-------------|
| A1 | Add `getAllPointsInJob()` to DCP06 JsonDatabase (DCP9 pattern; initially = getAllPoints if no alignments yet). |
| A2 | Add `getPointListAsSelectPoints()` and `getPointByIndex()` (or equivalent) to populate `S_SELECT_POINTS` and form_pnt1-style buffers from DB. |
| B1 | **3D File View:** Use DB when `m_currentJobId` set; fallback to ADF. |
| B2 | **POM (PomSelectPoints):** When selected file is current job, use DB. |
| B3 | **CalcDist, CalcAngle, CalcCircle:** When selected file matches current job path, use DB. |
| B4 | **Offset:** When design points come from current job, use DB. |
| B5 | **DOM UserDef:** Trace source of `select_point_list`; use DB when applicable. |
| B6 | **Select Point (PID) from DB:** DCP9 MeasureBridge allows picking any job point (incl. from alignments). DCP06 currently has SELECT_POINT_CONTROLLER that switches within point_table only. Consider adding "Select from job" to load any measured point from DB into current measure slot. |
| C1 | **Measurement → DB:** Persist full PointData (incl. SCS) on measure; keep ADF write for compatibility. |

---

## 8. Open Questions (Resolved / Updated from DCP9 Source)

1. **Change Station (CHST):** DCP9 uses `points_prev` and `points_new` (PointData). CHST calculation needs SCS for relocation. Phase C (measurement → DB with real SCS) is required for correct CHST reuse.
2. **Index vs ID:** DCP9 uses **getSelectedPointIds()** and **getSelectedPoints()** — returns by ID and full PointData. DCP06 callers use 1-based index. Plan: build an ordered list (numeric ID sort) when populating `S_SELECT_POINTS`; `getPointByIndex` uses same order. Future refactor could switch to ID-based where feasible.
3. ~~**DCP9 source:**~~ Resolved. DCP9 source at `C:\Users\dell\Desktop\AMS\Development\DCP9` has been analyzed. Key patterns documented in §2.1.

---

## 9. Summary

| Item | Status |
|------|--------|
| DCP05 limitations | Documented (ADF truncation, no SCS) |
| DCP9 benefits | Full PointData in DB, getAllPoints, getPoint |
| DCP06 call sites | Identified (POM, CalcDist, CalcAngle, CalcCircle, 3dFileView, Offsv, DOM UserDef) |
| Merge strategy | DB-backed point provider + branch in callers (DB vs ADF) |
| Prerequisite | Measurement → DB for full data (incl. SCS) |
| Next steps | Implement Phase A, then B1–B5, then C1 |

---

## 10. DCP9 Source References

| File | Purpose |
|------|---------|
| `DCP9/include/ui/SelectPointsDialog.h` | Point selection dialog interface, PointSelectionContext |
| `DCP9/src/ui/SelectPointsDialog.cpp` | populatePointsTable, getSelectedPoints, SCS filter, display coords |
| `DCP9/include/database/JsonDatabase.h` | getAllPointsInJob() declaration |
| `DCP9/src/database/JsonDatabase.cpp` | getAllPointsInJob() implementation (~line 2060) |
| `DCP9/src/ui/MeasureBridge.cpp` | OnPidButtonClicked — Select Point from job (~line 2789) |
| `DCP9/src/ui/Alignment321Bridge.cpp` | OnPlaneSelectClicked, OnLineSelectClicked, OnReferencePointSelectClicked |
| `DCP9/src/ui/BestFitAlignmentBridge.cpp` | OnSelectDesignPointsClicked, OnSelectActualPointsClicked |
| `DCP9/src/ui/CylinderAlignmentBridge.cpp` | Center line, reference angle, origin point selection |
| `DCP9/src/ui/ChangeStationBridge.cpp` | Point selection for prev/new station |
| `DCP9/src/ui/CircleBridge.cpp` | Circle point selection |
| `DCP9/src/ui/MidpointBridge.cpp` | Midpoint point selection |

---

*Document created: March 2025. Enhanced with DCP9 source analysis. To be updated as implementation progresses.*
