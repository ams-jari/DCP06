# DCP06 Point ID Naming Design — Prefix + Middlefix + Index

Design note: coherent **suggested** point IDs across the app. This document is a **target model** for implementation and migration; not all call sites follow it yet.

---

## 1. Goals

1. **Survey / 3D measurement** points remain the user’s main job sequence (`P1`, `P2`, … or job-driven continuation). They must not compete with internal or feature-specific IDs.
2. **All non-survey “feature” points** use one **predictable** pattern: **prefix + middlefix + index** (with agreed separators and length rules).
3. **One place** (eventually) generates each family of IDs so behavior is testable and documented.
4. Respect **`DCP_POINT_ID_LENGTH` (64)** and `POINT_ID_BUFF_LEN` / `DCP_POINT_ID_FMT` in code.

---

## 2. Two families (only two mechanisms)

| Family | Purpose | Suggestion source (target) |
|--------|---------|----------------------------|
| **A — Survey** | Points stored as normal **3D meas / survey** job points, LIST/tape navigation, `getLastSurveyPointId` style continuity | **Single** function, e.g. `suggestSurveyPointId()` — job last survey ID +1, or `P{n}` from survey count; excludes internal rows. |
| **B — Feature / context** | 321 buffers, scan boundary, Best Fit, circle rim, REF/OFF, internal grids, etc. | **Single** function, e.g. `suggestFeaturePointId(prefix, middlefix, index)` — **no** job-based “last P” bump unless explicitly specified for a feature. |

Everything that is **not** a survey point should use **family B** (or fixed internal names like `rp-p1` where history requires it — see §7).

---

## 3. Terminology

| Term | Meaning |
|------|--------|
| **Prefix** | Short **application or area** tag (e.g. alignment, scan, circle). Often 2–6 characters. Should be **stable** in code (constants). |
| **Middlefix** | **Role** or **sub-context** within that area: plane vs line, boundary vs grid, rim vs center, etc. Optional but recommended when one prefix has several point classes. |
| **Index** | **1-based** integer (or zero-padded only if we standardize it — default **no padding** for simplicity). Scoped per **(prefix, middlefix, parent id)** as defined per feature. |
| **Separator** | Single character between parts. **Recommendation:** `_` (underscore) for readability and to avoid clashes with IDs that use `-`. |

**Canonical pattern (feature family B):**

```text
{prefix}_{middlefix}_{index}
```

If **middlefix** is empty for a simple feature, collapse to:

```text
{prefix}_{index}
```

**Rules:**

- Use **lowercase** for new IDs unless an existing feature already uses mixed case (keep compatibility).
- **No** ambiguous `inc_id`-only logic for structured IDs: **always** format with `printf("%s_%s_%d", ...)` (or equivalent) so the **index is the last numeric segment** when possible.
- **Length budget:** `prefix + middlefix + digits(index) + separators` ≤ `DCP_POINT_ID_LENGTH`. Prefer short middlefix tokens (`bd`, `pln`, `ln`, `rim`).

---

## 4. Examples (target naming)

These are **illustrative**; exact strings should be centralized as constants before wide rollout.

| Feature | Prefix | Middlefix | Example IDs | Index scope |
|---------|--------|-----------|-------------|-------------|
| 321 plane measure (DOM) | `321` | `pln` | `321_pln_1` … | Points in that measure dialog |
| 321 line measure (DOM) | `321` | `ln` | `321_ln_1` … | Same |
| 321 offset / MeasV (when 321 context) | `321` | `pnt` | `321_pnt_1` … | Align with existing `321_pnt_*` if kept |
| Offsv (non-321) | `off` | — | `off_1` | Per session / buffer |
| Best Fit | `bf` | — | `bf_1`, `bf_2` | BF point list |
| Simple Scan boundary | `sc` | `bd` | `sc_bd_1`, `sc_bd_2`, `sc_bd_3` | Boundary corners (maps conceptually from `ScBd_*`) |
| Simple Scan grid (design) | User base (e.g. `sc`) | — | `sc_1`, `sc_2` or keep `SC1`-style legacy | Row-major counter; user prefix may stay short |
| Circle rim (per circle id) | `ci` + short id | `rim` | `ci1_rim_1` or `c01_rim_1` | Per circle + rim order |
| Change station / REF style | `ref` | — | `ref_1` | CHST / MeasV non-321 |
| Internal hz plane refs | (fixed) | — | `rp-p1` … `rp-p3` | **Legacy fixed** — see §7 |

**Survey (family A):** `P1`, `P2`, … or continuation from last **survey** point in job — **no** `prefix_middlefix` requirement.

---

## 5. Principles for index assignment

1. **Survey (A):** One global job rule: last survey ID numeric tail +1, or `P{surveyCount+1}` when no numeric tail; internal / feature IDs must be **excluded** from “last survey” (already partially done in JSON layer).
2. **Feature (B):** Index increments **within the feature’s scope** only:
   - Boundary: 1..3 for three corners.
   - 321 plane: 1..N for measured plane points in that buffer.
   - BF: 1..M for BF selection list.
3. **User override:** Any suggested ID is editable in UI unless read-only by design; persistence wins over suggestion.
4. **Stability:** Re-opening a dialog with existing non-blank IDs **must not** rename them; suggestions apply when `point_id` is blank or after explicit reset.
5. **Avoid `inc_id` for new feature IDs** — prefer explicit index from model state (`m_iCurrentPoint`, `m_iPointsCount + 1`, grid `points_count + 1`).

---

## 6. Implementation direction (non-binding checklist)

1. Add **`suggestSurveyPointId`** — narrow refocus of current `get_suggested_next_point_id` for `defaultPrefix == "P"` (or dedicated entry) with clear “survey only” contract.
2. Add **`suggestFeaturePointId(prefix, middlefix, index)`** — returns one string, enforces length, optional validation.
3. Replace ad hoc `default_pid` strings with **named constants** mapping to `(prefix, middlefix)` pairs.
4. Extend **JSON / LIST filters** (`isInternal321NavPlaceholderId`, etc.) for any **new** middlefix patterns so survey tape stays clean.
5. Document **deprecation** of mixed styles (e.g. migrate `ScBd_` → `sc_bd_` only if product agrees; otherwise keep alias in doc).

---

## 7. Exceptions and legacy

Some IDs are **fixed symbols**, not `prefix_middlefix_index`:

- **`rp-p1`, `rp-p2`, `rp-p3`** — horizontal plane reference; excluded from job flat views.
- **Midpoint** defaults like **`Mp1`** — may stay special-case or move to `mp_1` in a major version.
- **User-supplied scan base** — grid may remain `{user}{n}` (e.g. `SC1`) for field habits; middlefix can be implicit “grid”.

Treat these as **documented exceptions** to the general pattern, not as a third infinite family.

---

## 8. Summary

- **Two mechanisms:** (A) **survey** sequence, (B) **feature** IDs built as **`prefix[_middlefix]_index`** with separators and length limits.
- **Coherence** comes from **one builder for (B)** and **one for (A)**, not from every dialog inventing its own `default_pid` format ad hoc.
- Migration is **incremental**: new code and refactors adopt the builder; old strings remain valid until explicitly aligned.

---

## See also

- `include/dcp06/core/Defs.hpp` — `DCP_POINT_ID_LENGTH`, `DCP_SCAN_BOUNDARY_DEFAULT_PID_PREFIX`
- `src/core/Common.cpp` — `get_suggested_next_point_id`, `inc_id`
- `src/core/Measure.cpp` — `default_pid`, blank `point_id` fill, `add_point`
- `Docs/DCP06_Agent_Handoff_Summary.md` — historical note on `321_`, `OFF`, `BF`, `REF`
