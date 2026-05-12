# DCP06 Agent Handoff Summary

**Date:** 2026-05-06 (section 2.4 added) · original 2026-03-07  
**Purpose:** Handoff document for new agent to continue work on DCP06 (Leica Captivate plugin – onboard 3D measurement).

---

## 1. Project Overview

**DCP06** is a Leica Captivate plugin for onboard 3D measurement. Key concepts:

- **321 Alignment (was "DOM")** – Direct Orientation Method: plane + line + reference point
- **Best Fit (was "POM")** – Points-based alignment
- **Offsv** – Offset point dialog (step 1 of 321 reference point entry)
- **MeasV** – Measure dialog (step 2 of 321 reference point entry)

Naming migration: DOM → 321, POM → BF in titles and strings.

---

## 2. Completed Implementation

### 2.1 Build Fixes (Offsv.cpp, MeasV.cpp)

| Issue | Fix |
|-------|-----|
| `OBS::ObserverC` constructor error (line 75 Offsv, line 74 MeasV) | Added `OBS::CommandC` base class to `OffsetVDialog` and `MeasVDialog`; added `#include <BSS_TSK_ApiComponent.hpp>` in Offsv.cpp |
| `BSS::UTI::BSS_UTI_WCharToAscii` not found (Offsv line 256) | Replaced with `common.convert_to_ascii(sPoint, cPoint, ...)` using `DCP::Common` |
| `StringC::GetChar` does not exist (MeasV line 272) | Replaced with `m_pCommon->convert_to_ascii(...)` |

**Files changed:** `include/dcp06/orientation/Offsv.hpp`, `MeasV.hpp`, `src/orientation/Offsv.cpp`, `MeasV.cpp`

### 2.2 321 Reference Point ID – Editable & Transfer

- **Default IDs:** 321 context uses **`321_pnt_1`** (offset / MeasV), **`321_pl_1`** (plane meas; `DCP_321_PLANE_MEAS_DEFAULT_PID_PREFIX`), **`321_li_1`** (line meas; `DCP_321_LINE_MEAS_DEFAULT_PID_PREFIX`). Legacy job rows **`321_pl_pnt_*` / `321_li_pnt_*`** remain recognized as internal placeholders in `JsonDatabase`. **Circle rim** meas defaults to **`{circleId}_rim_`** (lowercased trimmed slug + `DCP_CIRCLE_RIM_MEAS_PID_SUFFIX`) + index → e.g. **`ci1_rim_1`** (legacy **`Ci1Pnt1`**).
- **Offsv Point ID editable:** ReadWrite when `display == A321_DLG || A321_USERDEF_DLG`
- **Offsv/MeasV Point ID observers:** `OnPointIdChanged` saves edits to `ovalues_buff` (Offsv) and `ref_point_buff` (MeasV)
- **Offsv OnF5/OnF6:** Use `321_pnt_` prefix when in 321 context
- **get_suggested_next_point_id** (`src/core/Common.cpp`): Job lookup bypassed for prefixes `321_`, OFF, BF, REF

### 2.3 DOM → 321 and POM → BF in Strings

- **DCP06.men:** Titles use 321 and BF (e.g. `T_ORIE_DOM_POINT_OFFSV` = `ORIENTATION\321\POINT\OFFSV`)
- **DCP06_TOK.HPP:** Updated comments for all DOM/POM title tokens
- **Last DOM→321 updates in DCP06_TOK.HPP:**
  - `T_DCP_DOM_ROTATE_PLANE_TOK` → `ORIENTATION\321\ROTATE PLANE`
  - `T_DCP_DOM_ROTATE_LINE_TOK` → `ORIENTATION\321\ROTATE LINE`
  - `T_DCP_DOM_LINE_MEAS_TOK` → `ORIENTATION\321\LINE\MEAS`
  - `T_ORIE_DOM_POINT_OFFSV_TOK` → `ORIENTATION\321\POINT\OFFSV` (already done)
  - `T_ORIE_DOM_POINT_MEASV_TOK` → `ORIENTATION\321\POINT\MEASV` (already done)

### 2.4 Circle: DEL on F2 (DCP05 parity) and default plane method

- **Problem (DCP06 vs DCP05):** Main Circle screen had blank F1–F3 in the non–`DCP06_STORE_CIRCLE_OBJECTS` build, so **DEL** disappeared; plane field defaulted to **“circle p”** (`CIRCLE_POINTS_PLANE`) because `Model::circle_plane_type` was initialized that way and `clear_circle()` used to reset `PLANE_TYPE` to circle-points after clearing.
- **DEL:** Legacy layout shows **DEL** on **F2**; `OnF2Pressed` (main screen, `PLANE_KEYS == 0`) calls `OnSHF2Pressed()` (same delete path as before). When **`DCP06_STORE_CIRCLE_OBJECTS`** is defined, softkeys are **F2 = DEL**, **F3 = LIST** so DEL stays on F2 like DCP05.
- **Default plane method:** `Model` constructor sets **`circle_plane_type = XY_PLANE`** (not `CIRCLE_POINTS_PLANE`). `CircleModel` copies `pModel->circle_plane_type` on construction. **`clear_circle()`** clears geometry only and **does not** overwrite `PLANE_TYPE`, so the user’s plane method is not forced back to “circle p” after delete/clear.
- **Legacy jobs:** Settings/archives that already store **`circle_plane_type = CIRCLE_POINTS_PLANE`** still load as **“circle p”** until the user changes it—no automatic one-time migration in code.
- **Files:** `src/measurement/Circle.cpp`, `Src/measurement/Circle.cpp` (keep in sync), `src/core/Model.cpp`, `Src/core/Model.cpp`.

---

## 3. Current State

### Working

- Default point ID `321_pnt_1` in 321 context
- Point ID editable in Offsv when in 321 flow
- Offsv/MeasV edits propagate to buffers via observers
- Build compiles (Offsv/MeasV observer and string conversion fixes applied)

### User-Reported Issue

- **Screen title still shows `ORIENTATION\DOM\POINT`** instead of `ORIENTATION\321\POINT`
- Point ID default is correct (`321_pnt_1`)

### Likely Cause

- **Stale `DCP06.LEN`** – Runtime strings come from `DCP06.LEN`, built by TextTool from `DCP06.men`. If `build_lang.bat` didn’t run or DCP06.LEN wasn’t copied to the simulator, old strings are shown.

---

## 4. Build / String Pipeline

```
DCP06_TOK.HPP (source of truth)
    ↓ hpp_to_men.py
DCP06.men
    ↓ TextTool
DCP06.LEN
    ↓ build_lang.bat copies
Simulator Plugin\DCP06\en\DCP06.LEN
```

- **build_lang.bat** runs in post-build; it depends on Python and TextTool (TextToolSetup_v5.1.1.msi)
- **TextTool** needs `Text/Languages/en/LangAndTypes.xml` to produce `.LEN` (not just `.L`)
- If you edit strings, update `DCP06_TOK.HPP` comments; `hpp_to_men.py` regenerates `DCP06.men` from them

---

## 5. Key Paths

| Purpose | Path |
|---------|------|
| String source of truth | `Text/DCP06_TOK.HPP` |
| Generated strings | `Text/Languages/en/DCP06.men` |
| Runtime strings | `Text/Languages/en/DCP06.LEN` |
| Build language script | `scripts/build_lang.bat` |
| HPP→men script | `scripts/hpp_to_men.py` |
| Offsv dialog | `src/orientation/Offsv.cpp`, `include/dcp06/orientation/Offsv.hpp` |
| MeasV dialog | `src/orientation/MeasV.cpp`, `include/dcp06/orientation/MeasV.hpp` |
| Common (convert_to_ascii, get_suggested_next_point_id) | `src/core/Common.cpp`, `include/dcp06/core/Common.hpp` |
| Circle dialog / controller (softkeys, plane type) | `src/measurement/Circle.cpp`, `include/dcp06/measurement/Circle.hpp` (mirror under `Src/` if used by VS project) |
| Global circle defaults (`circle_plane_type`) | `src/core/Model.cpp`, `include/dcp06/core/Model.hpp` |
| Tracking doc | `Docs/DCP06_DOM_POM_Rename_Tracking.md` |

---

## 6. Suggested Next Steps

1. **Force DCP06.LEN refresh**
   - Full rebuild (Release | Win32)
   - In build output, confirm: `Generated ... DCP06.men`, TextTool success, `DCP06.LEN installed successfully`
   - Restart the simulator so it reloads the new LEN

2. **If title still shows DOM**
   - Check whether the Captivate framework builds the breadcrumb from parent controllers or other tokens
   - Verify `DCP06.LEN` timestamp vs last build time
   - Confirm copy destinations in `build_lang.bat` match the simulator’s plugin path

3. **Optional cleanup**
   - `T_DCP_POM_POINTS_TOK` is still `ORIENTATION\POM\POINT`; could be updated to `ORIENTATION\BF\POINT` if desired (see tracking doc)

---

## 7. Relevant Documentation

- `Docs/DCP06_DOM_POM_Rename_Tracking.md` – Token rename tracking
- `Docs/DCP_SDK_Setup_Instructions.md` – Build setup, TextTool, environment
- `Docs/DCP06_Simulator_Fixes.md` – TextTool/LEN behavior, LangAndTypes.xml

---

## 8. Git Workflow

**No commit or push unless the user asks.**  
Rule: `.cursor/rules/git-workflow.mdc` – wait for an explicit user request (e.g. “commit”, “push”).

---

*Last updated: 2026-05-06*
