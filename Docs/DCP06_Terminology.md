# DCP06 Terminology — DCP9 Alignment

**Principle:** DCP06 uses DCP9 (modern) terminology, not DCP05 legacy naming. When naming, documenting, or adding features, prefer DCP9 terms.

---

## Alignment Types

| DCP05 (legacy) | DCP9 / DCP06 |
|----------------|--------------|
| **DOM** (Direct Orientation Method) | **321 Alignment** |
| **POM** (Point Orientation Method) | **Best Fit Alignment** |
| **COM** (Cylinder Orientation Method) | **Cylinder Alignment** |
| **CHST** (Change Station) | **Change Station** (unchanged) |

---

## Usage in Code and Docs

- **Use:** 321 Alignment, 321, Alignment321, alignments321
- **Avoid:** DOM, dom_*, DCP05Dom (except when referring to legacy/config keys for backward compatibility)

- **Use:** Best Fit Alignment, BestFit, BestFitAlignment, bestFitAlignments
- **Avoid:** POM, pom_*, POM_point_DCS (except legacy struct names that cannot be changed yet)

- **Use:** Cylinder Alignment, CylinderAlignment, cylinderAlignments
- **Avoid:** COM, com_* (except legacy)

---

## Other DCP9-Aligned Terms

| Context | Prefer | Avoid |
|---------|--------|-------|
| Job | Job, jobId | — |
| Point | Point, PointData | — |
| Coordinate system | SCS (Station Coordinate System), OCS (Object Coordinate System) | — |
| Surface scan | Surface Scan, SurfaceScanData | — |
| Midpoint | Midpoint, MidpointData | — |
| Circle | Circle, CircleData | — |

---

## Legacy Compatibility

Some DCP05 symbols remain for backward compatibility (e.g. config keys `CNF_KEY_DOM`, `CNF_KEY_POM`, struct names `POM_point_DCS`). When adding new code or documentation, use DCP9 terms. When refactoring, consider renaming legacy symbols in a future phase.

**Rename tracking:** See `DCP06_DOM_POM_Rename_Tracking.md` for completed UI string changes (DOM→321, POM→Best Fit) and remaining file/icon/identifier renames.

---

*Created: March 2025. Update when adding DCP9 features.*
