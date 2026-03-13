# DCP06 LIST and PICK Design

**Date:** March 2026  
**Status:** Design (refined)  
**Related:** `DCP06_Points_Only_Database_Design.md`

---

## 1. Overview

DCP06 uses two point-selection functions across measurement dialogs:

| Function | Purpose | Filter |
|----------|---------|--------|
| **LIST** | Fast navigation within current context | Points with **current dialog's tag** |
| **PICK** | Copy measured values to avoid remeasuring | Points with **any tag** AND **actual values** |

**Key principle:** Single point store, no duplicates. Each point is tagged to indicate which list(s) it belongs to.

---

## 2. LIST – Context-Specific

### Behavior

- **Scope:** Points with the **current dialog's tag**
- **Filter:** Tag only (no actual-values requirement)
- **Purpose:** Fast navigation – jump from current point to another point in the same context

### Tag by Dialog

| Dialog | Tag | LIST shows |
|--------|-----|------------|
| 3D meas | `3d_meas` | 3D meas points |
| 321 Alignment (Offsv, MeasV) | `321` | 321 offset + reference points |
| BestFit | `bestfit` | BestFit points |
| DefinePlane | `plane` | Plane points |
| DefineLine | `line` | Line points |
| Circle | `circle` | Circle points |
| Midpoint | `midpoint` | Midpoint points |
| Change Station | `chst` | CHST points |
| Line Setting / LineFitMeas | `line_setting` | Line fitting points |
| Measure (generic) | `measure` | Measure points |
| HiddenPoint | `hidden` | Hidden points |

### Example

User in 321 dialog → presses LIST → sees only points tagged `321` (offset point, reference point, plane points, line points).

---

## 3. PICK – Cross-Context, Measured Only

### Behavior

- **Scope:** Points from **any tag**
- **Filter:** Tag (any) **AND** point has **actual values** (`x_mea`, `y_mea`, `z_mea` defined)
- **Purpose:** Copy measured coordinates from another point to current point – avoid remeasuring

### Why Actual Values Only?

Design-only points (e.g. 321 offset entered manually) have no measured coordinates. PICK is for **copying measured values**, so only points with actual/measured coords are useful.

### Example Use Case

1. User measures 321 line point in hard-to-reach location (2 hours to get target in place).
2. Same physical point exists as P123 in 3D meas list.
3. User in 3D meas, editing P123.
4. User presses PICK → sees all points with actual values (321, 3d_meas, bestfit, etc.).
5. User selects 321 line point → P123 gets `x_mea`, `y_mea`, `z_mea` from that point.
6. No remeasuring needed.

---

## 4. Summary Table

| Function | Shows | Filter | Use Case |
|----------|-------|--------|----------|
| **LIST** | Points with current tag | Tag = current context | Navigate within current list |
| **PICK** | Points from any tag | Tag (any) + **has actual values** | Copy measured coords to avoid remeasuring |

---

## 5. Point Tags (Schema)

### Option: Single `source` Tag

Each point has one primary source tag:

```json
{
  "points": {
    "321_pnt_1": {
      "id": "321_pnt_1",
      "source": "321",
      "x_mea": 100.0, "y_mea": 200.0, "z_mea": 50.0,
      ...
    },
    "P001": {
      "id": "P001",
      "source": "3d_meas",
      "x_mea": 105.0, "y_mea": 205.0, "z_mea": 52.0,
      ...
    }
  }
}
```

- **LIST** in 321: `points where source == "321"`
- **LIST** in 3D meas: `points where source == "3d_meas"`
- **PICK**: `points where source in [any] AND has actual values`

### Tag Values (Constants)

| Tag | Description |
|-----|-------------|
| `3d_meas` | 3D measurement display |
| `321` | 321 Alignment (plane, line, offset, reference) |
| `bestfit` | Best Fit Alignment |
| `plane` | Define Plane |
| `line` | Define Line |
| `circle` | Circle points |
| `midpoint` | Midpoint |
| `chst` | Change Station |
| `line_setting` | Line fitting / Line setting |
| `measure` | Generic measure |
| `hidden` | Hidden point |

---

## 6. DCP06 vs DCP9 Compatibility

**Decision:** DCP06 jobs are **not** compatible with DCP9.
- DCP06 can use its own JSON structure and point schema.
- No need to maintain DCP9 interop.
- DCP06 can add `source` (or `tags`) to PointData without compatibility concerns.

---

## 7. Implementation Status

See `DCP06_LIST_PICK_Implementation_Plan.md` for detailed status and next steps.

---

*Created: March 2026*
