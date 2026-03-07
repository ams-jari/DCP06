# DCP06 Database Migration Plan

**Goal:** ADF only in one place (import/export). All other data from database.

**Rule:** Whatever user does that needs to be stored → database.

---

## Current State

- **File flow**: Uses `AdfFileFunc` (m_pAdfFile) for working ADF in `.work/{jobId}.adf`
- **Checks**: `m_pAdfFile->IsOpen()` used in 20+ places
- **Point access**: `pAdfFileFunc->form_pnt1()`, `form_pnt()`, `GetPointList()` in calculation dialogs
- **JsonDatabase**: Has `importFromADF`, `exportToADF`, `getAllPoints`, `addPoint`, etc.
- **Model**: Has `m_currentJobId`, `ADFFileName`, `GetDatabase()`

---

## Target State

| Operation | Current | Target |
|-----------|---------|--------|
| Is job open? | `m_pAdfFile->IsOpen()` | `!m_currentJobId.empty()` |
| Point count | `m_pAdfFile->getPointsCount()` | `db->getAllPoints().size()` |
| Load job | DB→work ADF→setFile | DB→loadJob→sync Model from DB |
| Save job | work ADF→importFromADF→saveJob | Model→saveJob (sync Model to DB) |
| Create job | create_adf_file_at_path | createJob in DB only |
| Import | ADF→importFromADF | ADF→importFromADF (unchanged, single place) |
| Export | DB→exportToADF | DB→exportToADF (unchanged, single place) |
| Point selection | pAdfFileFunc->GetPointList | db->getPointListAsSelectPoints |

---

## Phases

### Phase 1: Replace ADF checks with job-loaded checks ✅ DONE
- Replace `m_pAdfFile->IsOpen()` with `!m_pModel->m_currentJobId.empty()` 
- Replace `GetModel()->ADFFileName` with `m_pModel->m_currentJobId` for display
- File dialog RefreshControls: get point count/size/date from DB

### Phase 2: Remove working ADF from Load/Create/Save ✅ DONE
- Load: `loadJob(id)` only (no working ADF)
- Create: `createJob(id)` only
- Save: `saveJob(id)` directly
- Close: `closeJob()` + clear m_currentJobId
- Copy/Swap/Delete: use JsonDatabase
- 3D File View: uses DB for point display/navigation (m_currentPointIndex)

### Phase 3: Replace pAdfFileFunc in calculation dialogs (partial)
- CalculationAngle, CalculationDist2Points, BestFitSelectPoints: use `db->getPointListAsSelectPoints()` when points from "current job"
- For "3D file" selection: use DB (current job) as source

### Phase 4: Remove AdfFileFunc from FileModel (optional)
- Keep AdfFileFunc only for import/export format parsing if needed
- Or use JsonDatabase's importFromADF/exportToADF exclusively

---

## Sync: Model ↔ Database

Model has: `dom_point_buff`, `pom_point_buff`, `S_POINT_BUFF` structures.
Database has: `PointData`.

**On Load:** `db->getAllPoints()` → convert to Model buffers (dom_point_buff, etc.)
**On Save:** Model buffers → convert to `PointData` → `db->addPoint` (or replace all)
**On Add point (measurement):** Add to Model AND `db->addPoint()`

---

*Created: March 2025*
