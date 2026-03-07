# DCP06 DOM/POM → 321 Alignment / Best Fit Rename Tracking

**Date:** March 2025  
**Goal:** Use DCP9 terminology (321 Alignment, Best Fit Alignment) instead of DCP05 legacy (DOM, POM) in user-facing strings and documentation. Code identifiers and file names have been renamed; token names and icons remain for resource compatibility.

---

## 1. Completed Changes

### 1.1 UI Strings (Text/DCP06_TOK.HPP)

| Token | Old | New |
|-------|-----|-----|
| L_DCP_DOM_TOK (27) | "Direct. Orient. Method (dom)" | "321 Alignment" |
| L_DCP_POM_TOK (28) | "Point Orient. Method (pom)" | "Best Fit Alignment" |
| T_DCP_DOM_TOK (37) | "ORIENTATION\DOM" | "ORIENTATION\321" |
| T_DCP_DOM_PLANE_TOK (45) | "ORIENTATION\DOM\PLANE" | "ORIENTATION\321\PLANE" |
| T_DCP_DOM_LINE_TOK (55) | "ORIENTATION\DOM\LINE" | "ORIENTATION\321\LINE" |
| T_DCP_DOM_PLANE_MEAS_TOK (64) | "ORIENTATION\DOM\PLANE\MEAS" | "ORIENTATION\321\PLANE\MEAS" |
| T_DCP_POM_TOK (169) | "ORIENTATION\POM" | "Best Fit Align" |
| L_DCP_DOM_TEXT_TOK (211) | "DOM" | "321" |
| L_DCP_POM_TEXT_TOK (232) | "POM" | "BF" (abbrev for Best Fit) |
| T_ORIE_DOM_POINT_OFFSV_TOK (220) | "ORIENTATION\DOM\POINT\OFFSV" | "ORIENTATION\321\POINT\OFFSV" |
| T_ORIE_DOM_POINT_MEASV_TOK (221) | "ORIENTATION\DOM\POINT\MEASV" | "ORIENTATION\321\POINT\MEASV" |
| T_DCP_DOM_ROTATE_PLANE_TOK (223) | "ORIENTATION\DOM\ROTATE PLANE" | "ORIENTATION\321\ROTATE PLANE" |
| T_DCP_DOM_ROTATE_LINE_TOK (229) | "ORIENTATION\DOM\ROTATE LINE" | "ORIENTATION\321\ROTATE LINE" |
| T_DCP_DOM_LINE_MEAS_TOK (304) | "ORIENTATION\DOM\LINE\MEAS" | "ORIENTATION\321\LINE\MEAS" |
| M_DCP_DEFINE_LINE_LSET_TOK (324) | "Line must be defined in DOM!" | "Line must be defined in 321!" |
| M_DCP_REMEASURE_SCS_POINTS_TOK (441) | "Press MEAS-key in POM-display.." | "Press MEAS-key in BF-display.." |

### 1.2 Icon Comment

**File:** `src/application/Application.cpp` (lines ~1363-1365)

Added TODO comment for future icon rename:
```cpp
// TODO: Rename icons DOM_*.png and POM_*.png to Alignment321_*.png and BestFitAlignment_*.png in SWXRes when DCP06 icon set is updated
```

Icons currently used: `DOM_$SCALEFACTOR$.png`, `POM_$SCALEFACTOR$.png` — kept for now.

### 1.3 Documentation

- **DCP06_Project_Plan.md** — DOM/POM → 321 Alignment, Best Fit
- **DCP06_Architecture_Review.md** — same
- **DCP05_Summary.md** — same
- **README.md** — same
- **DCP06_Point_Reuse_Analysis.md** — POM → Best Fit in table

---

## 2. Remaining: Icons (Future Phase)

**Location:** `SWXRes/` (or equivalent DCP06 resource path)

| Current | Future (when icon set updated) |
|---------|-------------------------------|
| `DOM_$SCALEFACTOR$.png` | `Alignment321_$SCALEFACTOR$.png` |
| `POM_$SCALEFACTOR$.png` | `BestFitAlignment_$SCALEFACTOR$.png` |

Reference in code: `src/application/Application.cpp` — search for `DOM_$SCALEFACTOR$` and `POM_$SCALEFACTOR$`.

---

## 3. Internal Identifiers and File Names

### 3.1 File Names (Done)

| Current File | Contains |
|--------------|----------|
| Alignment321.cpp/.hpp | 321 Alignment logic |
| Alignment321UserDef.cpp/.hpp | User-defined 321 |
| BestFit.cpp/.hpp | Best Fit logic |
| BestFitSelectPoints.cpp/.hpp | Best Fit point selection |
| ResBestFit.cpp/.hpp | Best Fit residuals |
| Calculation321.hpp | 321 calculation |

### 3.2 Internal #defines and Identifiers (DCP_Defs.hpp, etc.)

| Identifier | Purpose | Status |
|------------|---------|--------|
| A321_DLG, A321_CONTROLLER, A321_USERDEF_* | 321 Alignment dialogs | Done |
| BESTFIT_DLG, BESTFIT_CONTROLLER, BESTFIT_POINT_*, RES_BESTFIT_*, FILE_CONTROLLER_BESTFIT | Best Fit dialogs | Done |
| DCP_A321_MENU, DCP_BESTFIT_MENU | Menu IDs | Done |
| MAX_BESTFIT_POINTS, MIN_BESTFIT_POINTS | Point limits | Done |
| align321_*, bestFit_* (model members) | Config/serialization | Done |
| DOM_ROTATION (local var) | 321 rotation status | Renamed to A321_ROTATION |
| DCP_CALCDOM_HPP | Include guard | Renamed to DCP_CALC321_HPP |
| DOM_USER_DLG, DOM_DSP (in comments) | Legacy display constants | Updated to A321_USERDEF_DLG, A321_DLG |

**Note:** Token names (T_DCP_DOM_*, L_DCP_POM_*, etc.) remain for string resource IDs; display text was updated. Config keys and serialized fields may require migration for full rename.

---

## 4. DCP9 Reference

DCP9 uses:
- **Alignment321Bridge**, **Alignment321Data** — for 321 Alignment (formerly DOM)
- **BestFitAlignmentBridge**, **BestFitAlignmentData** — for Best Fit Alignment (formerly POM)
- UI strings: "321 Alignment", "Best Fit Alignment", "Align" page with tabs

---

*Document created: March 2025. Update when further renames are completed.*
