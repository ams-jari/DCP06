# Captivate / Leica GUI Design Notes

This is a **living document** for Captivate/Leica interface design lessons learned. Update it when you discover new patterns, constraints, or workarounds.

---

## 1. Layout and Visibility

### 1.1 Small Screen Constraints

- **Total station displays** are small. Prefer **single-column layouts** when you need many fields visible.
- **Two-column layouts** can cause fields to be clipped or pushed off-screen on small devices.
- **VerticalBox (LT_VerticalBox)** with `AddCtrl` directly adds controls in a single column; all fields are visible when the user scrolls.

### 1.2 3D-MEAS Layout Example

For the 3D-MEAS display (Design, Actual, Deviation for X/Y/Z):

- **3D-MEAS layout:** Point ID only at top (no Job field). Then 2-column: Design/Actual in left column, Deviation in right column (with spacers for Design rows).
- **Point ID:** Strip any " ( job_name )" suffix—display only the point ID.
  ```
  [Point ID]                <- top row only
  Col1: X/Y/Z Design, X/Y/Z Actual
  Col2: (spacers), X/Y/Z Dev
  ```

### 1.3 Layout APIs

- `GUI::LayoutC::LT_VerticalBox` — stacks controls vertically.
- `GUI::LayoutC::LT_HorizontalBox` — places controls side by side.
- `pLayout->AddCtrl(ctrl)` — add a control to a layout.
- `pLayout->AddLayout(childLayout)` — nest layouts.
- `SetLayout(pMain)` — set the dialog’s root layout.

---

## 2. Control Types

### 2.1 ComboLineCtrlC

- **ComboLineCtrlC** = label + value (e.g. "Point ID: [value]").
- `IC_String` — text input.
- `IC_Float` — numeric input.
- `SetText(StringC(...))` — sets the label.
- `GetStringInputCtrl()->SetString(...)` — sets the value.
- `SetCharsCountMax(n)` — max length for string input.
- `SetEmptyAllowed(true)` — allows empty values.

### 2.2 SetTextWidth

- **`SetTextWidth`** is **not** supported on `GUI::ComboLineCtrlC` in Captivate.
- Do not use for field width control; use layout instead.

### 2.3 Text Alignment (SetAlign)

- **`SetAlign`** is **not** supported on `GUI::EditStringCtrlC` in Captivate.
- Text alignment (left/right/center) cannot be changed; controls use the platform default (typically right-aligned for numeric fields).

---

## 3. Data Assignment

### 3.1 Point ID vs Job File

- **Point ID** must show **only** the point identifier (e.g. `test_point_1`).
- **Job File** must show **only** the job name (e.g. `my_test_job_1`).
- **Never** concatenate or mix these in the same display field.
- Set each control explicitly in `RefreshControls()`:
  ```cpp
  m_pPointId->GetStringInputCtrl()->SetString(StringC(m_pDataModel->bPid));
  m_pFile->GetStringInputCtrl()->SetString(StringC(GetModel()->m_currentJobId.c_str()));
  ```

### 3.2 Buffer Overlap

- **Avoid** `sprintf(dest, "%s", src)` when `dest` and `src` overlap (e.g. `pid_ptr == bPid`).
- Use a branch or temporary buffer when copying from a pointer that may alias the destination.

---

## 4. Text Truncation

- Long text (Point ID, Job File) can be truncated in narrow fields.
- Single-column layout helps by giving each field full width.
- `SetCharsCountMax(DCP_POINT_ID_LENGTH)` limits input length; display width is controlled by layout.

---

## 5. 3D-MEAS Function Key Bar

The main function key bar (when not in point menu) is: **MEAS, POINT, AIM, LIST, SPECI, CONT**

- **MEAS** (F1): Measure point
- **POINT** (F2): Toggle point menu (PREV/ADD/NEXT/SWAP/FILE)
- **AIM** (F3): Aim at design coordinates
- **LIST** (F4): Open point list (select point dialog)
- **SPECI** (F5): Special functions menu (was on Shift+F3)
- **CONT** (F6): Continue/close

The "LIST" label replaces "PID" (Point ID) for clarity—both open the point list.

---

## 6. Checklist for New Displays

- [ ] Use single-column layout when many fields must be visible.
- [ ] Set Point ID and Job File (or similar) separately; never mix them.
- [ ] Do not use `SetTextWidth` on ComboLineCtrlC.
- [ ] Avoid `sprintf` when source and destination overlap.
- [ ] Test on small screens / emulator.

---

## 7. References

- **Display creation:** `Docs/DCP06_Creating_New_Displays_Guide.md`
- **3D-MEAS layout:** `src/measurement/3dmeas.cpp` (OnInitDialog, RefreshControls)
- **Tokens:** `Text/DCP06_TOK.HPP`, `Text/Languages/en/DCP06.men`

---

*Document version: 1.0 | Based on 3D-MEAS layout fixes and Captivate interface experience*
