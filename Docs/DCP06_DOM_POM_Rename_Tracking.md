# DCP06 DOM/POM → 321 Alignment / Best Fit Rename Tracking

**Date:** March 2025  
**Goal:** Use DCP9 terminology (321 Alignment, Best Fit Alignment) instead of DCP05 legacy (DOM, POM) in user-facing strings and documentation. Internal code identifiers and file names may retain DOM/POM for now; icons will be renamed in a future phase.

---

## 1. Completed Changes

### 1.1 UI Strings (Text/DCP05_TOK)

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

**File:** `src/application/DCP_Application.cpp` (lines ~1350-1351)

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

Reference in code: `DCP_Application.cpp` — search for `DOM_$SCALEFACTOR$` and `POM_$SCALEFACTOR$`.

---

## 3. Remaining: File Names and Internal Identifiers

These retain DOM/POM for backward compatibility and minimal code churn. Consider renaming in a future refactor.

### 3.1 File Names

| File | Contains | Rename To (future) |
|------|----------|-------------------|
| DCP_DCP05Dom.cpp/.hpp | 321 Alignment (DOM) logic | DCP_321Alignment.cpp/.hpp |
| DCP_DCP05DomUserDef.cpp/.hpp | User-defined 321 | DCP_321AlignmentUserDef.cpp/.hpp |
| DCP_DCP05Pom.cpp/.hpp | Best Fit logic | DCP_BestFitAlignment.cpp/.hpp |
| DCP_PomSelectPoints.cpp/.hpp | Best Fit point selection | DCP_BestFitSelectPoints.cpp/.hpp |
| DCP_ResPom.hpp | Best Fit residuals | DCP_ResBestFit.hpp |
| DCP_CalcDom.hpp | 321 calculation | DCP_Calc321.hpp |

### 3.2 Internal #defines and Identifiers (DCP_Defs.hpp, etc.)

| Identifier | Purpose | Rename To (future) |
|------------|---------|-------------------|
| DOM_DLG, DOM_CONTROLLER, DOM_USERDEF_* | 321 Alignment dialogs | ALIGNMENT321_* |
| POM_DLG, POM_CONTROLLER, POM_POINT_*, RES_POM_*, FILE_CONTROLLER_POM | Best Fit dialogs | BESTFIT_* |
| DCP_DOM_MENU, DCP_POM_MENU | Menu IDs | DCP_321_MENU, DCP_BESTFIT_MENU |
| MAX_POM_POINTS, MIN_POM_POINTS | Point limits | MAX_BESTFIT_POINTS, etc. |
| dom_*, pom_* (model members) | Config/serialization | alignment321_*, bestFit_* |

**Note:** Config keys `CNF_KEY_DOM`, `CNF_KEY_POM` and serialized fields (e.g. `dom_plane_buff`, `POM_point_DCS`) affect saved user data. Renaming these requires a migration path.

---

## 4. DCP9 Reference

DCP9 uses:
- **Alignment321Bridge**, **Alignment321Data** — for 321 Alignment (formerly DOM)
- **BestFitAlignmentBridge**, **BestFitAlignmentData** — for Best Fit Alignment (formerly POM)
- UI strings: "321 Alignment", "Best Fit Alignment", "Align" page with tabs

---

*Document created: March 2025. Update when further renames are completed.*
