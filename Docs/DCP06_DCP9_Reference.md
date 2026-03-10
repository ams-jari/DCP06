# DCP06 Reference Projects

**Purpose:** DCP06 development often requires consulting DCP05 (base) and DCP9 (reference). This document records their locations and when to use each.

---

## 1. Reference Locations

| Project | Path | Role |
|---------|------|------|
| **DCP05** | `C:\Users\dell\Desktop\AMS\Development\DCP05` | Base for DCP06. Captivate plugin, total station, legacy ADF, calculation flows. |
| **DCP9** | `C:\Users\dell\Desktop\AMS\Development\DCP9` | Reference for DCP06. Modern design: database, auto-save, alignments, math, workflows. |
| **DCP9 Docs** | `C:\Users\dell\Desktop\AMS\Development\DCP9\Docs` | Implementation notes, plans, patterns (e.g. constant auto-save, job management). |

These paths are outside the DCP06 workspace but can be read and used as reference.

---

## 2. When to Use Each

### DCP05 (Base)
- Captivate integration, SDK usage, display/controller patterns
- ADF format, import/export, legacy point handling
- Calculation dialogs (Angle, Dist, Circle, BestFit, etc.)
- Total station measurement flows

### DCP9 (Reference)
- Database design: `IDatabase`, `JsonDatabase`, `PointData`, job management
- Auto-save: constant save after addPoint/updatePoint (see `constant_auto_save_implementation.md`)
- Alignment workflows: 321, Best Fit, Cylinder, Change Station
- Geometry and math: line/plane/circle fitting, transformations, Kabsch
- Point selection, SelectPointsDialog pattern
- UI/workflow patterns (Jobs, Settings, Measure, Align)

---

## 3. Key DCP9 Docs

| Doc | Content |
|-----|---------|
| `constant_auto_save_implementation.md` | Auto-save after every DB operation |
| `job_management_simplified_workflow.md` | Job load/create/save flow |
| `job_management_save_button_removed.md` | No save prompts (auto-save) |
| `auto_save_implementation_plan.md` | Auto-save strategy |
| `measurement_implementation.md` | Measurement flows |

---

## 4. Related DCP06 Docs

- `DCP06_Database_Design.md` — Database merge from DCP9
- `DCP06_Database_Migration_Plan.md` — ADF → DB migration
- `DCP06_DCP9_Math_Comparison.md` — Math alignment with DCP9
- `DCP06_Point_Reuse_Analysis.md` — Point reuse from DCP9
- `DCP9_Summary.md` — DCP9 technical overview

---

## 5. Future Option: Cursor Rule

A Cursor rule (e.g. `.cursor/rules/dcp9-reference.mdc`) could be added to:
- Store DCP9 and DCP05 paths
- Instruct the AI to use DCP9 as reference for DCP06 when relevant
- Ensure future sessions automatically know where to look

*Not yet implemented; consider when DCP9 reference usage becomes frequent.*

---

*Created: March 2025*
