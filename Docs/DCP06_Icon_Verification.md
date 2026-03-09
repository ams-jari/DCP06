# DCP06 Icon Verification

Verification of required icons in `SWXRes/` for a clean build.  
**Check date:** March 2026

---

## Required Icons (from DCP06.sys and code)

### Boot-critical (DCP06.sys)
`Project/DCP06.sys` expects these for application registration:

| Required | Present in SWXRes | Notes |
|----------|-------------------|-------|
| DCP06_logo_1X.png | ✓ DCP06_logo_1X.png | |
| DCP06_logo_2X.png | ✓ DCP06_LOGO_2X.png | Debug/Win32 build copies to lowercase; Release|x64 may need manual copy |

### Menu icons (GetAppResourceUrl in code)

Each icon needs `_1X.png` and `_2X.png` variants.

| Base name | 1X | 2X | Status |
|-----------|-----|-----|--------|
| 3D_file | ✓ | ✓ | OK |
| 3D_roller_alignment | ✓ | ✓ | OK |
| 3D_shaft_alignment | ✓ | ✓ | OK |
| Angle | ✓ | ✓ | OK |
| Angle_file | ✓ | ✓ | OK |
| Application | ✓ | ✓ | OK |
| Best_fit_line | ✓ | ✓ | OK |
| Calculation | ✓ | ✓ | OK |
| Calculation_circle | ✓ | ✓ | OK |
| Change_Station | ✓ | ✓ | OK |
| Circle | ✓ | ✓ | OK |
| Circle_file | ✓ | ✓ | OK |
| Distace_file | ✓ | ✓ | OK |
| Distance | ✓ | ✓ | OK |
| DOM | ✓ | ✓ | OK |
| File | ✓ | ✓ | OK |
| Hidden_point | ✓ | ✓ | OK |
| Home_Points | ✓ | ✓ | OK |
| Line_setting | ✓ | ✓ | OK |
| Measurement | ✓ | ✓ | OK |
| mid_point | ✓ | ✓ | OK |
| Orientation | ✓ | ✓ | OK |
| Point_Cloud_Scanning | ✓ | ✓ | OK |
| POM | ✓ | ✓ | OK |
| Separate_recording | ✓ | ✓ | OK |
| Settings | ✓ | ✓ | OK |
| Shaft_align_file | ✓ | ✓ | OK |
| Simple_scanninng | ✓ | ✓ | OK |
| User_Def | ✓ | ✓ | OK |
| X_or_Y_or_Z | ✓ | ✓ | OK |

---

## Summary

**All 29 required icon base names have both 1X and 2X variants.**

The build post-step copies `DCP06_LOGO_2X.png` → `DCP06_logo_2X.png` when the lowercase file is missing, so the logo is covered.

---

## Optional / Extra files in SWXRes

| File | Used? |
|------|-------|
| DCP05 with icons.pptx | No (design reference) |
| Leica_data_formats_1X/2X.png | No (commented out in code) |
| mid_point.png | No (legacy; code uses mid_point_1X/2X.png) |

---

## Before clean build

1. Ensure `SWXRes_Source/DCP06_logo.svg` exists (source for logo).
2. Ensure `SWXRes_Source/Config.xml` lists `DCP06_logo.svg`.
3. Run a full rebuild so `SWXResBuilder` and `MkEdit` run.
4. After install, verify `DCP06_Res.pob` is present in the Captivate system folder.

---

*Last verified: March 2026*
