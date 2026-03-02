# DCP06 Architecture Review — Modern Standards Assessment

**Date:** March 2025  
**Status:** Pre-implementation review  
**Conclusion:** Current structure is **not** modern; recommended to restructure now before development scales.

---

## 1. Honest Assessment: Current State vs. Modern Standards

| Criterion | DCP06 (Current) | Modern 2024 Standard | Gap |
|-----------|------------------|------------------------|-----|
| **Folder structure** | Flat `Hdr/` + `Src/` + only `Src/math/` | Feature-based `include/`, `src/` with domain subdirs | **High** |
| **Architecture** | God object (`DCP05ModelC` ~1400+ lines) | Domain models, single responsibility | **High** |
| **Build system** | vcxproj only, env vars, no CMake | CMake + optional vcpkg/Conan | **Medium** |
| **Include layout** | Flat headers, no namespace hierarchy | `include/project/module/` | **Medium** |
| **Persistence** | Direct `CPI::CFG::ArchiveC` + ASCII files | Abstract `IDatabase` + backends | **High** (planned) |
| **Testing** | None | `tests/` with unit/integration | **High** |
| **Dependencies** | 20+ hardcoded SDK paths | CMake `find_package`, manifest | **Medium** |
| **C++ standard** | Unspecified (legacy) | C++17/20 | **Low** |

**Verdict:** The base from 1990 shows: flat files, monolithic model, no tests, type-based (Hdr/Src) instead of feature-based organization. DCP9 is a better reference for layout.

---

## 2. Specific Issues in Current Design

### 2.1 Folder Hierarchy

```
Current (DCP05/06):              DCP9 (modern reference):
├── Hdr/      (~72 files, flat)  ├── include/
├── Src/      (~102 files, flat) │   ├── database/
│   └── math/ (only subfolder)   │   ├── core/
├── Project/                     │   ├── ui/
├── Text/                        ├── src/
└── SWXRes/                      │   ├── database/
                                 │   ├── core/
                                 │   ├── ui/
                                 └── tests/
```

**Problems:**
- No domain separation beyond `math/` — Init, 321 Alignment, Best Fit, Measurement, Calc, File, Application all mixed
- Hard to navigate 100+ files in one folder
- New developers cannot quickly locate feature code
- Poor fit for the planned database layer (no natural place)

### 2.2 Architecture: God Object

`DCP05ModelC` / `DCP_Model.hpp` holds almost everything:

| Responsibility | Lines (approx) | Should be |
|----------------|----------------|-----------|
| Init config | — | `InitModel` |
| 321 Alignment (plane, line, OCS) | — | `DomModel` |
| Best Fit points | — | `PomModel` |
| CHST | — | `ChstModel` |
| Circle, Shaft, Line fit | — | Domain models |
| Files (ADF, CDF, ...) | — | Via `IDatabase` |
| Tools, licensing | — | Separate |

**Problem:** Single point of change, hard to test, tight coupling, violates single responsibility.

### 2.3 Build System

- **vsproj (VS 2008):** Obsolete format
- **No CMake:** Hard to automate CI, generate projects, manage SDK discovery
- **Env vars:** `SYSTEM1500_ROOT` — fragile, machine-specific
- **20+ include paths:** Hardcoded in vcxproj

### 2.4 Naming and Conventions

- `DCP_DCP05Chst`, `DCP_DCP05Dom` — DCP05 names in DCP06 codebase
- `_HEW_*` (Hello World) template leftovers
- Single flat namespace `DCP`
- No coding standards (formatting, naming, etc.)

---

## 3. Proposed Modern Structure for DCP06

### 3.1 Target Folder Hierarchy

```
DCP06/
├── include/dcp06/              # Public API, namespace-qualified
│   ├── init/
│   ├── orientation/            # 321 Alignment, Best Fit, CHST
│   ├── measurement/
│   ├── calculation/
│   ├── file/
│   ├── application/            # Line setting, shaft, line fitting
│   ├── database/               # IDatabase, types (Phase 1)
│   ├── core/                   # Common, model base, defs
│   └── math/                   # Or keep in src/math with internal headers
├── src/
│   ├── init/
│   ├── orientation/
│   ├── measurement/
│   ├── calculation/
│   ├── file/
│   ├── application/
│   ├── database/               # SQLite/Json impl (Phase 1)
│   ├── core/
│   └── math/                   # Existing math library
├── tests/
│   ├── unit/                   # Math, transforms, ADF round-trip
│   └── integration/            # (Later)
├── third_party/                # Or vcpkg.json
├── ui/                         # SWXRes, Text (or keep as-is)
│   ├── resources/
│   └── localization/
├── build/                      # Out-of-source builds
├── scripts/                    # SWXResBuilder, packaging
├── docs/
├── CMakeLists.txt              # Optional, alongside vcxproj
├── DCP06-VS2008.sln / DCP06-VS2022.sln
└── README.md
```

### 3.2 Constraints We Must Keep

| Constraint | Reason |
|------------|--------|
| Leica Captivate SDK | TPI, CPI, TBL, ABL — required for plugin |
| MSVC / vcxproj | Leica toolchain expects it; CMake can generate vcxproj |
| WinCE/ARM support | Physical instrument target |
| DCP06.dll output | Plugin format |
| MkEdit, SWXResBuilder | Leica packaging pipeline |

### 3.3 What We Can Change Now (Low Risk)

1. **Folder structure** — Move files into domain subdirs, update include paths in vcxproj
2. **Rename Hdr → include/dcp06** — Or keep `Hdr` but add subdirs: `Hdr/init/`, `Hdr/orientation/`, etc.
3. **Add tests/ folder** — Placeholder, add tests in Phase 4
4. ** scripts/** — Centralize build/deploy scripts

### 3.4 What We Change Over Time (Phased)

| Phase | Change |
|-------|--------|
| 1 | Introduce `IDatabase`, folder for `database/` |
| 2 | ADF → Database migration |
| 3 | Split `DCP05ModelC` into domain sub-models |
| 4 | Add CMake (optional), unit tests |

---

## 4. Recommended Action: Restructure Now

**Why now:**
- Only Docs + baseline source exist; no feature work yet
- Moving files later = more merge conflicts, broken includes
- Clear structure makes Phase 1 (database) and Phase 3 (DCP9 features) easier

**Two options:**

### Option A: Conservative — Add Domain Subfolders, Keep Hdr/Src

```
DCP06/
├── Hdr/
│   ├── init/
│   ├── orientation/
│   ├── measurement/
│   ├── calculation/
│   ├── file/
│   ├── application/
│   ├── core/
│   └── math/
├── Src/
│   ├── init/
│   ├── orientation/
│   ├── measurement/
│   ├── calculation/
│   ├── file/
│   ├── application/
│   ├── core/
│   └── math/
```

- Minimal change to build scripts
- vcxproj: add subdirs to `AdditionalIncludeDirectories`
- Move files into correct folders

### Option B: Modern — Full include/src Layout (Like DCP9)

```
DCP06/
├── include/dcp06/
├── src/
├── tests/
├── scripts/
└── ...
```

- Aligns with DCP9, industry practice
- Requires updating all `#include` paths and vcxproj
- Better long-term

---

## 5. Recommendation

**Recommend Option A (conservative) for immediate implementation:**

1. **Lower risk** — Keeps `Hdr/` and `Src/` names; Leica examples likely use similar layout
2. **Clear benefit** — Domain folders improve navigation and future modularity
3. **Reversible** — Can migrate to Option B later if desired

**Concrete steps:**
1. Create `Hdr/init/`, `Hdr/orientation/`, `Hdr/measurement/`, etc.
2. Create matching `Src/` subdirs
3. Move files (e.g. `DCP_DCP05Init.*` → `init/`, `DCP_MeasXYZ.*` → `measurement/`)
4. Update vcxproj `ClCompile`/`ClInclude` paths and `AdditionalIncludeDirectories`
5. Add `tests/` and `scripts/` folders
6. Update Docs and README

---

## 6. Summary Table

| Aspect | Current | Recommended |
|--------|---------|--------------|
| **Folders** | Flat Hdr/, flat Src/ | `Hdr/<domain>/`, `Src/<domain>/` |
| **Model** | God object | Plan split in Phase 4 |
| **Database** | None | `IDatabase` in Phase 1 |
| **Build** | vcxproj only | Keep vcxproj; add CMake later if useful |
| **Tests** | None | Add `tests/` now, populate Phase 4 |
| **Scripts** | Inline in vcxproj | Extract to `scripts/` |

---

## 7. Implementation Note (March 2025)

**Option B was implemented** instead of Option A. The codebase now uses the modern layout:

- `include/dcp06/{core,init,orientation,measurement,calculation,file,application}/` — headers
- `src/{core,init,orientation,measurement,calculation,file,application}/` — sources
- `src/math/` — math library (unchanged)
- `tests/unit/` — placeholder for unit tests
- `scripts/` — restructure_option_b.ps1, update_includes.ps1, update_vcxproj_paths.ps1, update_vcproj_paths.ps1

All `#include` paths were updated to use `<dcp06/domain/Header.hpp>`. Both DCP06-VS2022.vcxproj (CS35) and DCP06-VS2008.vcproj (instrument) have been updated.

*Document approved for implementation. Option B implemented March 2025.*
