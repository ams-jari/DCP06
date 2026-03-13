# DCP06 LIST and PICK Implementation Plan

**Date:** March 2026  
**Related:** `DCP06_LIST_PICK_Design.md`, `DCP06_Points_Only_Database_Design.md`

---

## 1. Design Summary (Quick Reference)

| Function | Filter | Use Case |
|----------|--------|----------|
| **LIST** | Points with **current tag** | Navigate within context |
| **PICK** | Points with **any tag** + **actual values** | Copy measured coords to avoid remeasuring |

---

## 2. What We Have (Current State)

### 2.1 Database & Point Storage

| Item | Status | Notes |
|------|--------|------|
| `PointData` in DatabaseTypes.hpp | ✅ Exists | Has `x_mea`, `y_mea`, `z_mea`, `x_dsg`, `y_dsg`, `z_dsg` |
| `points` map in JobData | ✅ Exists | Main point store |
| `addPoint`, `updatePoint`, `getPoint` | ✅ Exists | JsonDatabase |
| `getAllPoints()` | ✅ Exists | Returns main points only |
| `getAllPointsInJob()` | ✅ Exists | Returns main points only (no alignment points yet) |
| `getPointListAsSelectPoints()` | ✅ Exists | Fills S_SELECT_POINTS from getAllPointsInJob |
| `getPointListAsSelectPoint()` | ✅ Exists | Simpler format for SelectPoint dialog |
| `getPointByIndex()` | ✅ Exists | 1-based index lookup |
| **Point tags / source** | ❌ Missing | PointData has no `source` or `tags` field |

### 2.2 3D Meas

| Item | Status | Notes |
|------|--------|------|
| LIST (F4) | ✅ Works | Opens SelectPoint, shows `getPointListAsSelectPoints(..., BOTH)` |
| PICK | ❌ Not implemented | No PICK key; would need F5 or similar |
| Points added via addPoint | ✅ Yes | When user saves/measures |
| Tag on add | ❌ No | 3D meas points not tagged |

### 2.3 321 Alignment

| Item | Status | Notes |
|------|--------|------|
| Offsv / MeasV points in model | ✅ Yes | In align321_ovalues_buff, align321_ref_point_buff |
| Points in alignments321 JSON | ⚠️ Partial | reference_point, offset_value written on CALC (F5); id added in recent fix |
| Points in main `points` map | ❌ No | 321 points never call addPoint |
| LIST in Offsv | ⚠️ Uses DESIGN | getPointListAsSelectPoints(..., DESIGN) – for PICK-from-design, not LIST |
| PICK in Offsv/MeasV | ❌ No | Not implemented |

### 2.4 Other Dialogs (BestFit, Line, Plane, Circle, etc.)

| Dialog | LIST | PICK | Points to DB | Tag |
|--------|------|------|--------------|-----|
| BestFit | getPointListAsSelectPoints(DESIGN) | Has PICK key | Partial | No |
| BestFitSelectPoints | PICK uses getPointListAsSelectPoints | - | - | No |
| Offsv | getPointListAsSelectPoints(DESIGN) | No | No | No |
| DefinePlane, DefineLine | Various | No | No | No |
| Circle | getPointListAsSelectPoints(ACTUAL) | No | No | No |
| CalculationDist | getPointListAsSelectPoints(ACTUAL) | No | - | No |

### 2.5 Tokens / UI

| Item | Status |
|------|--------|
| K_DCP_LIST_TOK | ✅ Exists |
| K_DCP_PICK_TOK | ✅ Exists |

---

## 3. What We Need to Implement

### Phase 1: Point Tags / Source Schema

| Task | Description |
|------|-------------|
| 1.1 | Add `source` (or `tags`) to PointData in DatabaseTypes.hpp |
| 1.2 | Update pointDataToJson / jsonToPointData to serialize `source` |
| 1.3 | Define tag constants (e.g. `DCP_POINT_SOURCE_3D_MEAS`, `DCP_POINT_SOURCE_321`, etc.) |

### Phase 2: Database API for LIST and PICK

| Task | Description |
|------|-------------|
| 2.1 | Add `getPointsForList(const std::string& tag)` – returns points with given tag |
| 2.2 | Add `getPointsForPick()` – returns points with any tag AND actual values |
| 2.3 | Add `getPointListAsSelectPointsForList(S_SELECT_POINTS*, tag)` – LIST variant |
| 2.4 | Add `getPointListAsSelectPointsForPick(S_SELECT_POINTS*)` – PICK variant (actual values only) |
| 2.5 | Update `addPoint` to accept tag/source – or add `addPointWithSource(id, data, source)` |

### Phase 3: Store Points with Tags

| Task | Description |
|------|-------------|
| 3.1 | 3D meas: when addPoint, set `source = "3d_meas"` |
| 3.2 | 321: when saving CALC, add offset + reference to main points with `source = "321"` |
| 3.3 | BestFit: when adding points, set `source = "bestfit"` |
| 3.4 | Plane, Line, Circle, Midpoint, CHST, etc.: set tag when adding points |

### Phase 4: Wire LIST to Tag-Filtered API

| Task | Description |
|------|-------------|
| 4.1 | 3D meas LIST: call `getPointsForList("3d_meas")` |
| 4.2 | 321 LIST: call `getPointsForList("321")` |
| 4.3 | BestFit LIST: call `getPointsForList("bestfit")` |
| 4.4 | Each dialog: pass its tag to LIST handler |

### Phase 5: Implement PICK

| Task | Description |
|------|-------------|
| 5.1 | 3D meas: Add PICK key (e.g. F5), open SelectPoint with `getPointsForPick()` |
| 5.2 | On PICK selection: copy `x_mea`, `y_mea`, `z_mea` from selected point to current point |
| 5.3 | Add PICK to 321, BestFit, Plane, Line, Circle, etc. where applicable |

### Phase 6: Aggregate Points from Alignments (for PICK)

| Task | Description |
|------|-------------|
| 6.1 | `getPointsForPick()` must include points from alignments321, bestFitAlignments, etc. |
| 6.2 | When 321/BestFit/etc. store points, ensure they are in main `points` OR aggregated in getPointsForPick |
| 6.3 | Dedupe by point ID when aggregating |

---

## 4. Implementation Order

```
Phase 1 (Schema)     → Phase 2 (API)     → Phase 3 (Store with tags)
                            ↓
                    Phase 4 (LIST)       Phase 5 (PICK)
                            ↓
                    Phase 6 (Aggregate alignment points)
```

---

## 5. Open Decisions

1. **Storage:** Keep alignments321, bestFitAlignments as separate structures that reference points by ID, OR merge all into `points` with tags? (Recommendation: single `points` map with tags; alignments reference by ID.)
2. **Backward compatibility:** Existing jobs without `source` – treat as `3d_meas` or unknown?
3. **PICK key placement:** F5 in 3D meas? Same key across dialogs?

---

## 6. Dependencies

- **Phase 3.2** depends on 321 points being added to main points (partially done in Alignment321.cpp for alignments321; need to also add to `points` with tag).
- **Phase 6** may require refactoring how alignment points are stored (currently in alignments321.offset_value, reference_point, plane.points, line.points).

---

*Created: March 2026*
