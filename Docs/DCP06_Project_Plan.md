# DCP06 Project Plan

**Document Purpose:** Detailed development plan for DCP06.  
**Repository:** https://github.com/ams-jari/DCP06  
**Base:** DCP05 (onboard 3D measurement for Leica total stations with Captivate)  
**Feature Sources:** DCP9 (laser tracker tablet app), PMP9 (FreeCAD workbench — minor reference)

---

## 1. Executive Summary

### 1.1 Product Vision

DCP06 is the next-generation **onboard 3D measurement software for Leica total stations** with Leica Captivate. It is based on DCP05 but adopts modern concepts from DCP9, most notably:

- **Database-driven data management** (replacing DCP05 ASCII file system)
- **Modern architecture** and UI patterns where compatible with Captivate
- **Preserved functionality** from DCP05 with improved workflows

### 1.2 Key Differences vs. Source Projects

| Aspect | DCP05 | DCP9 | DCP06 |
|--------|-------|------|-------|
| **Platform** | Captivate plugin (instrument) | Windows tablet app | Captivate plugin (instrument) |
| **Instrument** | Total station (MS60, TS16, TS60) | Laser tracker (AT403, AT960) | Total station |
| **Data storage** | ASCII files (.adf, .cdf, etc.) | JSON + .bin (job-based) | **Database** (SQLite / Captivate storage) |
| **UI** | GraphMenuDialog, F-keys | Qt 6, tablet layout | Captivate UI (inherit DCP05) |

### 1.3 PMP9 Role

PMP9 (FreeCAD workbench, Python) shares domain concepts (DOM, COM, CHST, ADF, alignments). Use as **reference** for:
- Alignment solver logic
- ADF format handling
- Data structure design

---

## 2. Development Phases

### Phase 0: Project Setup (Weeks 1–2)

| Task | Description | Owner |
|------|-------------|-------|
| 0.1 | Initialize DCP06 repository from DCP05 baseline | Dev |
| 0.2 | Set up GitHub: branch strategy, CI (if applicable) | Dev |
| 0.3 | Create Docs folder; copy/adapt DCP05 Docs; add DCP05/DCP9 summaries | Dev |
| 0.4 | Define coding standards, Cursor rules, AGENTS.md | Dev |
| 0.5 | Confirm Leica SDK version (System 1500 vs. newer Captivate) | PM/Tech Lead |

**Deliverables:** Repo initialized, documentation in place, build working.

---

### Phase 1: Database Foundation (Weeks 3–6)

**Goal:** Introduce database concept while preserving DCP05 behavior.

| Task | Description | Source |
|------|-------------|--------|
| 1.1 | Design database schema (jobs, points, alignments, settings) | DCP9 IDatabase/DatabaseTypes |
| 1.2 | Choose storage backend: SQLite on instrument storage, or Captivate storage API | TBD |
| 1.3 | Implement `IDatabase` (or equivalent) interface | DCP9 |
| 1.4 | Implement first DB backend (e.g. SQLite) for points, jobs | New |
| 1.5 | Migrate `DCP05ModelC` to use database instead of ADF files for active session | DCP05 |
| 1.6 | Implement ADF **import** (read DCP05 files into DB) | DCP05 |
| 1.7 | Implement ADF **export** (write DB to ADF for compatibility) | DCP05, DCP9 |

**Deliverables:** Database layer, ADF round-trip compatibility, model integrated with DB.

---

### Phase 2: Core Workflow Preservation (Weeks 7–12)

**Goal:** All DCP05 user workflows work with database backend.

| Task | Description | DCP05 Reference |
|------|-------------|------------------|
| 2.1 | Init: User, unit, target, atm, face, file storage → DB persistence | DCP_DCP05Init |
| 2.2 | DOM (orientation): Plane, Line, Point, Rotate → DB | DCP_DCP05Dom |
| 2.3 | POM (best-fit): Points, calculation, residuals → DB | DCP_DCP05Pom |
| 2.4 | CHST (change station): Transfer points, file/measure → DB | DCP_DCP05Chst |
| 2.5 | Measurement: XYZ, Dist, Angle, Hidden, Circle, Scan → DB | DCP_Meas*, DCP_Scan* |
| 2.6 | Calculation: Dist, Angle, Line/Plane/Circle fit → DB | DCP_Calc* |
| 2.7 | File: CDF, AGF, CRL, SFT support (import/export via DB) | AdfFileFunc, etc. |
| 2.8 | Application: Line setting, shaft, scanning, line fitting | DCP_LineSetting, etc. |

**Deliverables:** Full feature parity with DCP05, all data in database.

---

### Phase 3: DCP9 Features Integration (Weeks 13–18)

**Goal:** Add DCP9-inspired improvements where applicable.

| Task | Description | DCP9 Reference |
|------|-------------|----------------|
| 3.1 | Job management: create, load, save, delete, copy, swap | JobsBridge, JsonDatabase |
| 3.2 | Multi-job support in UI (job selector) | JobsPage |
| 3.3 | Settings persistence in DB (target, decimals, temp, etc.) | SettingsBridge |
| 3.4 | Alignment data model alignment (321/DOM, BestFit/POM, Cylinder/COM, CHST) | DatabaseTypes |
| 3.5 | CAD import (if Captivate allows): DXF, STEP, IGES → design points | cad/, libdxfrw, OpenCASCADE |
| 3.6 | Export formats: TXT, XLS (if feasible on Captivate) | DCP9 export |
| 3.7 | Point limit policy (DCP9: 1000) or scalable with indexing | DatabaseTypes |

**Deliverables:** Job-centric workflow, enhanced import/export, alignment terminology aligned.

---

### Phase 4: Architecture & Code Quality (Weeks 19–22)

| Task | Description |
|------|-------------|
| 4.1 | Refactor controllers: introduce Bridge-like separation (UI ↔ DB ↔ Leica API) |
| 4.2 | Split `DCP05ModelC` into domain sub-models (Init, DOM, POM, Meas, etc.) |
| 4.3 | Math library: evaluate keep vs. Eigen; add unit tests for fitting/transforms |
| 4.4 | Add unit tests: DB CRUD, ADF import/export, coordinate transforms |
| 4.5 | Regression testing: compare results with DCP05 on simulator |

**Deliverables:** Cleaner architecture, test coverage, regression suite.

---

### Phase 5: UI/UX & Localization (Weeks 23–26)

| Task | Description |
|------|-------------|
| 5.1 | Preserve Captivate UI patterns (GraphMenuDialog, ComboLineCtrl, F-keys) |
| 5.2 | Add new tokens for job management, DB-related messages |
| 5.3 | Migrate ~628 DCP05 tokens; keep IDs stable |
| 5.4 | Job selector UI integration (within Captivate constraints) |

**Deliverables:** Complete UI, localized strings.

---

### Phase 6: Testing, Packaging, Release (Weeks 27–30)

| Task | Description |
|------|-------------|
| 6.1 | Integration tests on CS35 simulator |
| 6.2 | Tests on physical instruments (MS60, TS16, TS60) if available |
| 6.3 | Config migration: DCP05 → DCP06 upgrade path |
| 6.4 | Build configuration: instrument, Win32, x64 simulator |
| 6.5 | Documentation: user manual, migration guide |
| 6.6 | Release v1.0 |

**Deliverables:** Release builds, documentation, migration guide.

---

## 3. Technical Architecture

### 3.1 Target Stack

| Layer | Technology |
|-------|-------------|
| **Platform** | Leica Captivate System 1500 (or newer) |
| **Language** | C++17 |
| **Data** | Database (SQLite or Captivate storage) |
| **File compat** | ADF import/export for DCP05 compatibility |

### 3.2 Component Diagram (Conceptual)

```
┌─────────────────────────────────────────────────────────────────────┐
│  Captivate Host (DCP06.dll)                                         │
├─────────────────────────────────────────────────────────────────────┤
│  DCP06ControllerC                                                    │
│       │                                                              │
│       ├──► Database Layer (IDatabase implementation)                 │
│       │         └── SQLite / Captivate Storage                        │
│       │                                                              │
│       ├──► Model (DCP06ModelC, domain sub-models)                    │
│       │         └── Config, Orientation, Points, Matrices             │
│       │                                                              │
│       └──► Leica API (TPI, CPI, TBL, ABL)                            │
└─────────────────────────────────────────────────────────────────────┘
```

### 3.3 Database Schema (Draft)

| Table/Entity | Purpose |
|--------------|---------|
| **Job** | id, date, measurer, instrument_id, units, active_orientation_id, active_chst_id, settings |
| **Point** | job_id, point_id, x_act, y_act, z_act, x_des, y_des, z_des, note, status, ... |
| **DOM_321** | job_id, alignment_id, plane_points, line_points, ref_point, matrix |
| **POM_BestFit** | job_id, alignment_id, points_scs, points_ocs, matrix, residuals |
| **COM_Cylinder** | job_id, alignment_id, center_line, ref_angle, matrix |
| **ChangeStation** | job_id, station_id, points_prev, points_new, matrix |
| **Scan** | job_id, scan_id, metadata; points in separate store or .bin |
| **Circle** | job_id, circle_id, center, normal, diameter, plane_points, circle_points |

---

## 4. Risk Register

| Risk | Impact | Mitigation |
|------|--------|------------|
| Captivate storage limits (e.g. SQLite size) | High | Use external .bin for point clouds like DCP9; test on instrument |
| Leica SDK changes | Medium | Lock SDK version; plan migration if upgrade required |
| ADF compatibility gaps | Medium | Unit tests for ADF round-trip; validate with real DCP05 files |
| Performance on instrument (ARM, WinCE) | High | Benchmark DB operations; optimize indexes; consider lazy loading |
| DCP05 regression | High | Automated regression suite; side-by-side result comparison |

---

## 5. Dependencies and Prerequisites

### 5.1 External

- Leica Captivate System 1500 SDK (or newer)
- Boost (filesystem, platform adapters)
- SQLite (if chosen) — ensure WinCE/ARM support or use alternative

### 5.2 Internal

- DCP05 codebase (baseline)
- DCP9 codebase (reference for DB, bridges, alignment types)
- PMP9 (reference for alignment math, ADF)

---

## 6. Version Control

- **Repository:** https://github.com/ams-jari/DCP06  
- **Branch strategy:** main (release), develop (integration), feature/* for tasks  
- **Commit messages:** Conventional format (e.g. feat:, fix:, docs:)

---

## 7. Documentation Structure (DCP06 Docs)

| Document | Purpose |
|----------|---------|
| **DCP05_Summary.md** | DCP05 technical summary (this folder) |
| **DCP9_Summary.md** | DCP9 technical summary (this folder) |
| **DCP06_Project_Plan.md** | This document |
| **DCP06_Architecture.md** | (Phase 4+) Detailed architecture |
| **DCP06_Database_Schema.md** | (Phase 1+) Final database schema |
| **DCP06_Migration_Guide.md** | (Phase 6) User migration from DCP05 |

---

## 8. Success Criteria

1. **Functional parity** with DCP05 for all user workflows.
2. **Database** as primary data store (no reliance on ASCII files for active session).
3. **ADF import/export** for backward compatibility with DCP05.
4. **Job management** (create, load, save, delete, copy) as in DCP9.
5. **Regression tests** passing; results match DCP05 within tolerance.
6. **Documentation** complete for developers and users.

---

*Document version: 1.0 | Created: March 2025 | Last updated: March 2025*
