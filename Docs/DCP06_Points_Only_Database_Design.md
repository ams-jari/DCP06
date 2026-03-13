# DCP06 Points-Only Database Design

**Date:** March 2026  
**Status:** Design / Plan  
**Goal:** Simplify DCP06 database to store only points (JSON), align with DCP9, add PICK for point reuse, and hide object-level storage behind compiler switches.

---

## 1. Executive Summary

DCP06 will store **only points** in the JSON database. Circle, Midpoint, and other object-level storage (ADD, DEL, LIST with IDs) will be hidden behind compiler switches. Every measured point will have a Point ID and default ID logic (reference DCP9). A new **PICK** function will allow selecting any point from the database for use in any context (3D meas, circle, plane, line, etc.). **LIST** remains context-specific; **PICK** shows all points. **DIST** is removed from measurement displays; measurement happens only via **MEAS**.

---

## 2. Common Rules (Apply Multiple Times)

These rules must be remembered and applied across many places in the codebase.

| # | Rule | Applies To |
|---|------|------------|
| **R1** | Every point measured must have a **Point ID** | 321, BestFit, Change Station, Circle, Plane, Line, 3D meas, Offsv, MeasV, etc. |
| **R2** | Default Point ID logic must **reference DCP9** | All displays that create new points |
| **R3** | **LIST** = context-specific points (points measured for this object) | Every measurement display |
| **R4** | **PICK** = all points from database (SelectPoint-style) | Every measurement display |
| **R5** | Measurement happens only via **MEAS**; **DIST** is not used | All measurement displays |
| **R6** | Point parameters must match **DCP9 PointData** (and current DCP06) | Database schema, serialization |

---

## 3. One-Time Tasks

Tasks done once, not repeated.

| # | Task | Description |
|---|------|-------------|
| **T1** | Points-only JSON schema | Remove `circlesData`, `midpointsData` from job JSON; keep only `points`. Ensure DCP9 can open DCP06 JSON (points only) and vice versa. |
| **T2** | Hide Circle object storage | Put Circle ADD, DEL, LIST, `addCircle`, `getCircle`, etc. behind `#ifdef DCP06_STORE_CIRCLE_OBJECTS`. Default: disabled. |
| **T3** | Hide Midpoint object storage | Put Midpoint ADD, DEL, LIST, `addMidpoint`, `getMidpoint`, SelectMidpoint, MidpointDialog setup behind `#ifdef DCP06_STORE_MIDPOINT_OBJECTS`. Default: disabled. |
| **T4** | Remove DIST from displays | Remove ALL & DIST from Measure, LineFitMeas, HiddenPoint; keep only MEAS. |
| **T5** | Move SPECI to second row in 3D meas | SPECI behind Fn button (second row), not on main bar. |
| **T6** | Add PICK to 3D meas | New PICK function key; opens SelectPoint-style dialog showing **all** points from database. |
| **T7** | Add LIST + PICK to every measurement display | Each display: LIST (context points) + PICK (all points). |

---

## 4. Design Rules in Detail

### 4.1 Database: Points Only (Rule 1, 6)

- **DCP06 JSON job** stores only `points` (map of PointData by id).
- **PointData** structure: same as DCP9 and current DCP06 (`DatabaseTypes.hpp`).
- **Interop:** DCP06 JSON opens in DCP9 (points only). DCP9 JSON opens in DCP06 (points only; DCP9 may have extra fields we ignore).
- **Circle/Midpoint as objects:** Stored only when `DCP06_STORE_CIRCLE_OBJECTS` / `DCP06_STORE_MIDPOINT_OBJECTS` enabled. Otherwise, circle/midpoint *results* are computed from points; no persistent circle/midpoint objects.

### 4.2 Point ID and Default ID (Rule 2)

- **Every point** in 321, BestFit, Change Station, Circle, Plane, Line, 3D meas, Offsv, MeasV, etc. must have a Point ID.
- **Default ID:** Reference DCP9 implementation. Use last point in list (or equivalent) to suggest next ID. Already done for 3D meas ADD; apply same pattern everywhere new points are created.
- **DCP9 reference:** Check `DCP9/src/ui/DesignPointsBridge`, `SelectPointsDialog`, default ID logic.

### 4.3 LIST vs PICK (Rules 3, 4)

| Function | Shows | Use Case |
|----------|-------|----------|
| **LIST** | Points measured in *current context* (e.g. circle points for circle, 3D meas points for 3D meas) | Navigate/select within current object |
| **PICK** | **All** points in database (job) | Reuse any measured point in any context |

- **3D meas:** LIST = points measured in 3D meas. PICK = all points (new; like DCP9 SelectPoint).
- **Circle:** LIST = points measured for this circle. PICK = all points (to add existing point to circle).
- **Plane, Line, 321, BestFit, Change Station, etc.:** Same pattern.

### 4.4 Measurement: MEAS Only (Rule 5)

- Remove **DIST** (distance-only measurement) from displays that have ALL & DIST.
- **Affected:** `Measure.cpp`, `LineFitMeas.cpp`, `HiddenPoint.cpp` (and any similar).
- Keep **MEAS** (3D measurement) as the only measurement entry point in those displays.

### 4.5 SPECI in 3D Meas (Task T5)

- **Current:** Main bar: MEAS, POINT, AIM, LIST, SPECI, CONT. Second row: Fn, Camera, Init, SPECI, Tool, Calc, ChangeFace.
- **Target:** SPECI on second row only (behind Fn). Main bar: MEAS, POINT, AIM, LIST, PICK, CONT (or similar; PICK takes F5 if added).

---

## 5. Compiler Switches

| Switch | Purpose | Default |
|--------|---------|---------|
| `DCP06_STORE_CIRCLE_OBJECTS` | Circle ADD, DEL, LIST, JsonDatabase circlesData, SelectCircle, Circle job serialization | **Off** |
| `DCP06_STORE_MIDPOINT_OBJECTS` | Midpoint ADD, DEL, LIST, JsonDatabase midpointsData, SelectMidpoint, MidpointDialog setup | **Off** |

- When **Off:** Circle and Midpoint workflows use points only; no persistent circle/midpoint objects in JSON.
- When **On:** Current behavior (circle/midpoint stored as objects with IDs).
- Implementation: `#ifdef DCP06_STORE_*` around the relevant code paths. Do not delete; only conditionally compile.

---

## 6. Implementation Phases

### Phase A: Compiler Switches (One-Time)

1. Add `DCP06_STORE_CIRCLE_OBJECTS` and `DCP06_STORE_MIDPOINT_OBJECTS` to project (Defs.hpp or vcxproj).
2. Wrap Circle object storage (JsonDatabase, CircleController ADD/DEL/LIST, SelectCircle, job load/save for circles) in `#ifdef`.
3. Wrap Midpoint object storage (JsonDatabase, MidPointController ADD/DEL/LIST, SelectMidpoint, MidpointDialog) in `#ifdef`.
4. Ensure build succeeds with both switches Off and On.

### Phase B: Points-Only JSON

1. Modify job JSON schema: when switches Off, do not write `circlesData`, `midpointsData`.
2. Ensure `points` structure matches DCP9.
3. Test: DCP06 job (points only) opens in DCP9; DCP9 job (points only) opens in DCP06.

### Phase C: PICK and LIST

1. **3D meas:** Add PICK (F5 or appropriate key). Implement SelectPoint-style dialog showing `getAllPoints()` from database.
2. **3D meas:** LIST unchanged (3D meas points). PICK = all points.
3. **Other displays:** Add PICK where missing. Ensure LIST shows context points.

### Phase D: Point ID Everywhere

1. Audit all point-creation flows: 321, BestFit, Change Station, Circle, Plane, Line, Offsv, MeasV, etc.
2. Ensure each assigns Point ID. Use DCP9 default-ID logic as reference.
3. Add default ID helper (e.g. `getNextPointId()`) if not already present.

### Phase E: Remove DIST, Move SPECI

1. Remove DIST (and ALL where redundant) from Measure, LineFitMeas, HiddenPoint.
2. Move SPECI in 3D meas to second row (behind Fn).

---

## 7. Affected Components (Reference)

| Component | LIST | PICK | Point ID | DIST Removal | Compiler Switch |
|-----------|------|------|----------|--------------|-----------------|
| 3D meas | ✅ (3D points) | ➕ Add | ✅ | - | - |
| Circle | ✅ (circle points) | ➕ Add | ✅ | - | Circle objects |
| Midpoint | ✅ (mid points) | ➕ Add | ✅ | - | Midpoint objects |
| 321 Alignment | ✅ | ➕ Add | ✅ | - | - |
| BestFit | ✅ | ➕ Add | ✅ | - | - |
| Change Station | ✅ | ➕ Add | ✅ | - | - |
| DefinePlane | ✅ | ➕ Add | ✅ | - | - |
| DefineLine | ✅ | ➕ Add | ✅ | - | - |
| Offsv | ✅ | ➕ Add | ✅ | - | - |
| MeasV | ✅ | ➕ Add | ✅ | - | - |
| Measure (generic) | ✅ | ➕ Add | ✅ | ➕ Remove DIST | - |
| LineFitMeas | ✅ | ➕ Add | ✅ | ➕ Remove DIST | - |
| HiddenPoint | ✅ | ➕ Add | ✅ | ➕ Remove DIST | - |

---

## 8. DCP9 Reference Documents

- `DCP9/Docs/` — SelectPointsDialog, DesignPointsBridge, default point ID
- `DCP06_DCP9_Reference.md` — Paths and when to use DCP9
- `DCP06_Database_Design.md` — Existing database design (to be updated for points-only)

---

## 9. Summary Checklist

- [ ] Add compiler switches for Circle and Midpoint object storage
- [ ] Wrap Circle/Midpoint object code in `#ifdef`
- [ ] Points-only JSON schema; DCP06↔DCP9 interop
- [ ] Add PICK to 3D meas (all points)
- [ ] Add LIST + PICK to every measurement display
- [ ] Point ID + default ID everywhere (reference DCP9)
- [ ] Remove DIST from Measure, LineFitMeas, HiddenPoint
- [ ] Move SPECI to second row in 3D meas

---

*Created: March 2026*
