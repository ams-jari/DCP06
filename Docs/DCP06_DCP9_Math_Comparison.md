# DCP06 vs DCP9 Math Functions Comparison Report

**Document purpose:** Detailed analysis for aligning DCP06 math with DCP9 math, with a single shared implementation. Similar to the database/ADF replacement strategy.

**Date:** March 2025  
**DCP06 base:** DCP05 math  
**DCP9 base:** PMP9 math (modern design)

---

## 1. Executive Summary

| Aspect | DCP06 | DCP9 |
|--------|-------|------|
| **Origin** | DCP05 (legacy) | PMP9 (modern) |
| **Language style** | C-style functions, global structs | C++, classes, Eigen |
| **Data representation** | `double*`, `ams_vector`, `plane`, `line` | `Point`, `Plane`, `Line`, `Circle` classes |
| **Algorithms** | Custom/iterative, 3-point/exact | SVD, PCA, Kabsch (linear algebra) |
| **Quality metrics** | Minimal (return codes) | RMS, residuals, validation |
| **Testing** | Minimal | Comprehensive (Google Test) |
| **Status** | Functional but dated | Production-ready, PMP9-aligned |

**Recommendation:** Replace DCP06 math with DCP9-style math (ideally shared implementation or thin adapter) for consistency, maintainability, and single fix-point for defects.

---

## 2. Design Comparison

### 2.1 DCP06 Design (DCP05 Heritage)

**Location:** `src/math/`, `Src/math/`, `include/dcp06/math/calc.h`

**Characteristics:**
- C-style free functions
- Global types: `ams_vector`, `sphvect`, `line`, `plane`
- Raw arrays (`double *p_mat`, `double mea[]`, etc.)
- Fixed-size buffers (e.g. `k_table[20*3]`, `MAX_POINTS 20`)
- Return codes: `short` (1=OK, -1=error, -2=user abort)
- No namespaces
- Direct matrix indexing (`*(p_mat+i*3+j)`)

**Key types (`calc.h`):**
```c
struct ams_vector { double x, y, z; };
struct sphvect   { double r, theta, fii; };
struct line     { double px,py,pz, ux,uy,uz; };  // point + direction
struct plane    { double px,py,pz, nx,ny,nz; };  // point + normal
```

**Data flow example (FitLine):**
```c
short FitLine(double *p_mat, short points, double *a, double *b, double *c);
// p_mat: row-major N×3 matrix
// a,b,c: output direction vector (unit)
```

---

### 2.2 DCP9 Design (PMP9 Heritage)

**Location:** `src/core/geometry/`, `include/core/geometry/`, `include/geometry/`

**Characteristics:**
- C++ classes with encapsulation
- Eigen for matrices/vectors
- `std::vector<Point>` for point sets
- Result structs: `LineFitResult`, `PlaneFitResult`, `CircleFitResult`, `RigidTransformResult`, `AlignmentResult`
- Return types: structs with `isValid`, `rms`, `residuals`
- Namespaces: `DCP9::Core::Geometry`, `DCP9::Geometry`
- Comprehensive validation and error handling

**Key classes:**
- `DCP9::Geometry::Point` – x, y, z, id, precision, deviation
- `DCP9::Geometry::Plane` – point + normal, `projectPoint()`
- `DCP9::Geometry::Line` – point + direction
- `DCP9::Geometry::Circle` – center, radius, normal

**Data flow example (bestFitLine):**
```cpp
LineFitResult bestFitLine(const std::vector<DCP9::Geometry::Point>& points);
// Returns: point, direction, rms, residuals, isValid
```

---

## 3. Function Mapping: DCP06 → DCP9

### 3.1 Fitting Functions

| DCP06 Function | DCP9 Equivalent | Algorithm Diff | Notes |
|----------------|-----------------|----------------|-------|
| `FitLine(p_mat, points, a, b, c)` | `bestFitLine(points)` | **Different** | DCP06: weighted average of segment directions. DCP9: PCA/SVD on covariance matrix. DCP9 is statistically optimal. |
| `FitPlane(m_mat, points, a, b, c)` | `bestFitPlane(points)` | **Different** | DCP06: iterative cross-product + normal summation from 3-point plane. DCP9: SVD on covariance matrix. DCP9 is statistically optimal. |
| `center_of_circle(x1..y3, radius, cx, cy)` | `bestFitCircle2D()` or explicit 3-point | **Different** | DCP06: 3-point exact circle (matrix inverse). DCP9: least-squares 2D fit + `bestFitCircle()` for 3D. DCP9 supports N points. |
| `pns_ymp()` (DCP_calccircle) | `bestFitCircle2D()` | **Equivalent** | Both use least-squares 2D circle (AtA\*x=Atb). DCP9 uses Eigen SVD. |
| `calc_circle()` (3-point combos) | `center_of_circle()` or `bestFitCircle2D()` | Partial | DCP06 averages many 3-point circles. DCP9 `bestFitCircle2D` is cleaner for N points. |
| `equation_of_plane(a,b,c,d)` | `Plane(p1,p2,p3)` constructor | **Equivalent** | Both compute normal from 3 points. DCP9 uses class. |
| `equation_of_line(a,b,c)` | `Line` from 2 points | **Equivalent** | Same geometric definition. |

---

### 3.2 Transformation Functions

| DCP06 Function | DCP9 Equivalent | Algorithm Diff | Notes |
|----------------|----------------|----------------|-------|
| `CoordTransf6(mea, nom, points, par, M, k, Zdd)` | `solveRigidTransform(source, target)` | **Different** | DCP06: iterative least-squares (6 DoF). DCP9: Kabsch (SVD). DCP9 is direct, non-iterative. |
| `htransm(p0,p1,p2,iyz,hm)` | `rotationMatrixFromAxisAngle()` + `htrans6`-style | **Different** | DCP06: 3-point → homogeneous matrix. DCP9: axis-angle → rotation. Need mapping for DOM/POM workflows. |
| `htrans6(p0..p5, hm)` | Similar to `solveRigidTransform` + 4×4 | **Different** | DCP06: 6-point registration. DCP9: Kabsch with N points. |
| `ptrans(v, a, w)` | `transformPoint(point, transformMatrix)` | **Equivalent** | 4×4 × point. |
| `rotate_about_x/y/z_axis` | `rotationMatrixFromAxisAngle(axis, theta)` | **Equivalent** | DCP9 generalizes to arbitrary axis. |
| `HROTAX/Y/Z` | `rotationMatrixFromAxisAngle()` | **Equivalent** | Same concept, DCP9 more generic. |

---

### 3.3 Distance & Geometry Helpers

| DCP06 Function | DCP9 Equivalent | Notes |
|----------------|----------------|-------|
| `calc_point_dist_from_line(a, line)` | `distancePointToLine(point, linePoint, direction)` | Same formula. |
| `calc_point_dist_from_plane(a, plane)` | `Plane::distanceTo(point)` or `calculatePlaneResiduals` | Same formula. |
| `calc_point_dist_from_point(a, b)` | `Point::distanceTo(other)` | Same formula. |
| `PDIST`, `PDISL`, `PDISPL` | As above + `calculate*Residuals` | DCP9 adds RMS/residuals. |
| `dot_product(a,b)` | `Eigen::Vector3d::dot()` or manual | Trivial. |
| `vector_product(a,b,c)` | `Eigen::Vector3d::cross()` | Trivial. |
| `vector_length(a)` | `Eigen::Vector3d::norm()` | Trivial. |
| `angle(a,b)` | Dot product + arccos | Trivial. |

---

### 3.4 Matrix & Utility Functions

| DCP06 Function | DCP9 Approach | Notes |
|----------------|---------------|-------|
| `matinv_n`, `matinv4x4` | Eigen `MatrixXd::inverse()` / `FullPivLU` | Eigen handles it. |
| `matmul_n`, `matmul4x4` | Eigen `operator*` | Eigen handles it. |
| `mattra_n`, `mattra` | Eigen `transpose()` | Eigen handles it. |
| `matcopy_n`, `matsum_n`, `reset_matrix` | Eigen or `std::` | Trivial. |
| `matvecmul_n`, `matvec` | Eigen matrix×vector | Trivial. |
| `degtorad`, `radtodeg` | `std` or inline | Trivial. |
| `cartsph`, `sphcart` | DCP9 does not have (low priority in PMP9) | Optional. |
| `dir_vector`, `unit_vector` | Eigen normalized vector | Trivial. |

---

## 4. Algorithm Comparison (Critical Functions)

### 4.1 Line Fitting

**DCP06 `FitLine`:**
1. Compute direction vectors between consecutive points: `v[i] = p[i+1] - p[i]`
2. Weight each by segment length
3. Sum weighted directions, normalize
4. Returns unit direction; point is implicit (first point or centroid not clearly defined)
5. Fixed buffer 20×3 points
6. No RMS or residuals

**DCP9 `bestFitLine`:**
1. Compute centroid
2. Center points, build 3×3 covariance matrix
3. SVD → eigenvector with largest eigenvalue = direction
4. Returns centroid as point, direction, RMS, residuals
5. No fixed limit
6. Statistically optimal (minimizes sum of squared perpendicular distances)

**Verdict:** DCP9 is superior and standard (PCA). DCP06 algorithm is heuristic.

---

### 4.2 Plane Fitting

**DCP06 `FitPlane`:**
1. Use first 3 points to get initial plane via `equation_of_plane`
2. For each point, find farthest point, iterate over all other points
3. Cross-products and normal accumulation with length weighting
4. Normalize final normal
5. No RMS or residuals
6. Complex, O(n²) per point

**DCP9 `bestFitPlane`:**
1. Centroid and centered points
2. 3×3 covariance matrix
3. SVD → eigenvector with smallest eigenvalue = normal
4. Returns plane, RMS, residuals
5. Statistically optimal, O(n)

**Verdict:** DCP9 is superior and standard (SVD). DCP06 is legacy and harder to maintain.

---

### 4.3 Rigid Transformation (CoordTransf6 vs solveRigidTransform)

**DCP06 `CoordTransf6`:**
1. Iterative least-squares (6 parameters: rx, ry, rz, tx, ty, tz)
2. Linearization, 6×6 normal matrix, iterations until convergence
3. Supports weighting `wmea`, `wnom`
4. Returns `par`, `M` (3×3), `k` (translation), `Zdd` (covariance)
5. Embedded UI/debug output (commented)

**DCP9 `solveRigidTransform`:**
1. Kabsch: centroids, centered points, H = A^T B, SVD of H
2. R = V U^T (with reflection handling)
3. t = centroid_B - R * centroid_A
4. Non-iterative, numerically stable
5. Returns rotation, translation, 4×4 matrix, RMS, residuals
6. Optional `sortPoints` for correspondence

**Verdict:** DCP9 Kabsch is standard and robust. DCP06 iterative approach may still be used for specific weighting; that could be added to DCP9 if needed.

---

### 4.4 Circle Fitting

**DCP06:**
- `center_of_circle`: 3-point exact (matrix inverse for circle through 3 points)
- `calc_circle`: average of all 3-point combinations
- `pns_ymp`: 2D least-squares (AtA, Atb, solve for cx, cy, radius)
- Points are projected to plane first via `convert_points_to_plane`

**DCP9:**
- `bestFitCircle`: Fit plane → project points → 2D least-squares
- `bestFitCircle2D`: 2D least-squares in XY
- `bestFitCircleInPlane`: 2D least-squares in given plane
- Returns circle, RMS, residuals
- Uses Eigen SVD for linear algebra

**Verdict:** DCP9 design is cleaner and matches PMP9. DCP06 `pns_ymp` is similar to DCP9 2D fit.

---

## 5. Usage in DCP06 (Call Sites)

| Caller | DCP06 Function | Usage |
|--------|----------------|-------|
| `DCP_HiddenPoint.cpp` | `FitLine` | Hidden point line fitting |
| `DCP_CalcPom.cpp` | `CoordTransf6` | POM alignment (point-based) |
| `DCP_CalcChst.cpp` | `CoordTransf6` | Change station |
| `DCP_calccircle.cpp` | `center_of_circle`, `pns_ymp`, `convert_points_to_plane` | Circle center/diameter |
| `DCP_CalcCircleController`, `DCP_ResCircle`, `DCP_ResShaft` | `calc_center_of_circle` | Circle results |
| `DCP_PlaneScanning.cpp` | `equation_of_line` | Plane scanning geometry |
| `DCP_HiddenPoint.cpp` | `equation_of_line` | Hidden point line |
| `DCP_CalcDom.cpp` | `htransm` | DOM alignment |
| `DCP_CalcPom.cpp`, `DCP_CalcChst.cpp` | `htransm` | POM/ChSt transforms |
| `DCP_calccircle.cpp` | `htransm` | Circle coordinate transform |

---

## 6. Gaps and Coverage

### DCP06-Only (No Direct DCP9 Equivalent Today)

| DCP06 | Notes |
|-------|-------|
| `CoordTransf6` weight parameters `wmea`, `wnom` | DCP9 `solveRigidTransform` has no weights; could be extended |
| `htransm(p0,p1,p2,iyz)` | 3-point → 4×4 matrix; DCP9 has axis-angle, `solveRigidTransform` |
| `htrans6` (6-point) | Special case of rigid transform; could use `solveRigidTransform` with 6 points |
| `cartsph`, `sphcart` | Spherical coords; DCP9 does not provide (low PMP9 priority) |
| `ranglz` | Angle between 2 points; trivial from dot product |

### DCP9-Only (Not in DCP06)

| DCP9 | Notes |
|------|-------|
| `AlignmentCalculator::solveAlignment` | BestFit + 3-2-1; DCP06 does alignment via CoordTransf6 + htransm |
| `solve321Alignment` | Explicit 3-2-1; DCP06 may do similar via DOM |
| `findCorrespondingPoints` | Point matching; DCP06 uses manual ordering |
| `Plane::projectPoint(s)` | Projection; DCP06 has `convert_points_to_plane` |
| `distancePointToCircle` | Point–circle distance; DCP06 computes per-point via `calc_point_dist_from_point` |
| Batch RMS/residuals | DCP9 returns RMS and residuals; DCP06 mostly does not |

---

## 7. Migration Done (7B - Direct DCP9 Usage)

**Completed 2025-03:** DCP06 now uses DCP9 math directly (no wrappers):

| Area | Change |
|------|--------|
| **Line fitting** | DCP_HiddenPoint uses `bestFitLine` and `Line(p1,p2).direction()` |
| **Plane fitting** | DCP_calcplane uses `bestFitPlane` and `Plane(p1,p2,p3)` |
| **Rigid transform** | DCP_CalcPom, DCP_CalcChst use `solveRigidTransform` |
| **Removed** | FITLINE.cpp, FITPLANE.cpp, COORD6TR.cpp from build |
| **Added** | DCP9 geometry (Point, Plane, Line, Circle, fitting, transformations) copied into DCP06 |

**Build prerequisite:** Install Eigen via vcpkg before building:
```bash
cd DCP06
vcpkg install eigen3:x86-windows    # for Win32
vcpkg install eigen3:x64-windows    # for x64 (DCP06-CS)
```
Or use manifest mode: open the project in Visual Studio with vcpkg integration enabled; it will run `vcpkg install` automatically.

---

## 8. Migration Strategy: Single Implementation (for future 7A)

### 7.1 Recommended Approach (Same as Database/ADF)

1. **Option A: Shared library**
   - Create `AMS-Math` (or equivalent) with DCP9-style API.
   - DCP06 links to it; DCP9 links to it.
   - One implementation, one fix-point.

2. **Option B: DCP9 as source of truth**
   - DCP06 adds dependency on DCP9 geometry (or a copy of the math subset).
   - DCP06 call sites use thin adapters: `double*` → `std::vector<Point>`, results → legacy structs.

3. **Option C: Gradual in-place replacement**
   - Replace DCP06 math module by module.
   - Start with: FitLine, FitPlane, center_of_circle / bestFitCircle, CoordTransf6.
   - Keep adapter layer for `double*` and legacy structs during transition.

### 7.2 Priority Order for Migration

| Priority | Function/Area | Reason |
|----------|---------------|--------|
| 1 | FitLine | Used in hidden point; algorithm upgrade |
| 2 | FitPlane | Used in plane fitting; algorithm upgrade |
| 3 | CoordTransf6 → solveRigidTransform | POM, ChSt; core alignment |
| 4 | Circle fitting (center_of_circle, pns_ymp) | Circle measurements |
| 5 | htransm / htrans6 | DOM, POM, ChSt transforms |
| 6 | equation_of_plane, equation_of_line | Used by FitPlane, PlaneScanning, HiddenPoint |
| 7 | Distance helpers (PDIST, PDISL, PDISPL) | Many call sites |
| 8 | Matrix utilities | Can use Eigen or replace incrementally |

### 7.3 Adapter Pattern Example

```cpp
// DCP06 adapter for FitLine using DCP9
short FitLine(double *p_mat, short points, double *a, double *b, double *c) {
    std::vector<DCP9::Geometry::Point> pts;
    for (short i = 0; i < points; ++i)
        pts.emplace_back(p_mat[i*3+0], p_mat[i*3+1], p_mat[i*3+2]);
    
    auto result = DCP9::Core::Geometry::bestFitLine(pts);
    if (!result.isValid) return -1;
    
    *a = result.direction[0];
    *b = result.direction[1];
    *c = result.direction[2];
    return 1;
}
```

---

## 8. Future AMS Math Library / API

**Current:** DCP9 math is self-contained; DCP06 has its own math.

**Future idea:** Central “AMS Math” library/API:
- Single implementation for fitting, transforms, alignment.
- Used by DCP06, DCP9, and potentially other AMS products.
- API in style of DCP9 (Point, Plane, Line, Circle, result structs).
- C or C++ API, depending on consumer needs.
- Not required immediately; can evolve from current DCP9 math.

---

## 9. Summary Table

| Category | DCP06 | DCP9 | Action |
|----------|-------|------|--------|
| Line fitting | Heuristic | PCA/SVD | Replace with DCP9 |
| Plane fitting | Iterative/cross-product | SVD | Replace with DCP9 |
| Circle fitting | 3-point + 2D LS | Plane+2D LS, full API | Replace with DCP9 |
| Rigid transform | Iterative LS | Kabsch | Replace with DCP9 |
| Alignment | CoordTransf6 + htransm | AlignmentCalculator | Map to DCP9 workflows |
| Distance/helpers | Multiple small functions | Classes + free functions | Use DCP9 or adapters |
| Matrix ops | Manual | Eigen | Use Eigen |
| RMS/Residuals | Minimal | Full | Adopt DCP9 style |

---

## 10. Conclusion

- DCP06 math is C-style, legacy from DCP05, with weaker algorithms and limited quality metrics.
- DCP9 math is modern, uses SVD/PCA/Kabsch, and returns RMS and residuals.
- Aligning DCP06 with DCP9 math (or a shared AMS math) is recommended for:
  - Single implementation and fix-point
  - Better algorithms
  - Clearer APIs and easier maintenance
- Migration can be incremental with adapters from `double*` buffers to `std::vector<Point>` and back.
- A future AMS math library can grow from the DCP9 math implementation.

---

## Appendix A: DCP06 Math File Inventory

| File | Purpose |
|------|---------|
| FITLINE.cpp | Line fitting |
| FITPLANE.cpp | Plane fitting |
| ccircle.cpp | `center_of_circle` (3-point) |
| COORD6TR.cpp | CoordTransf6 |
| EQOFPL.cpp | equation_of_plane |
| EQOFLI.cpp | equation_of_line |
| HROTAX/Y/Z.cpp | Rotation matrices (X/Y/Z axes) |
| HTRANSM.cpp | htransm (3-point → 4×4) |
| PTRANS.cpp | ptrans (point × 4×4) |
| PDIST, PDISL, PDISPL | Point distances |
| DOTPROD, VECPROD, VECLNTH | Dot, cross, length |
| CANGLE | Angle between vectors |
| MATINV_N, MATMUL_N, MATTRA_N, MATCOP_N, MATSUM_N | Matrix ops |
| Minv4x4, MMUL4X4 | 4×4 matrix ops |
| VECMAT, matvec | Matrix × vector |
| DEGTORAD, RADTODEG | Angle conversion |
| DIRVEC, UNIVEC | Direction/unit vector |
| SPHCART, CART | Spherical/Cartesian |
| RESETMAT | Matrix reset |

---

## Appendix B: DCP9 Geometry File Inventory

| File | Purpose |
|------|---------|
| PlaneFitting.h/cpp | bestFitPlane, residuals, RMS |
| LineFitting.h/cpp | bestFitLine, residuals, RMS |
| CircleFitting.h/cpp | bestFitCircle, 2D, in-plane |
| Transformations.h/cpp | solveRigidTransform, transformPoints, rotateAboutAxis |
| AlignmentCalculator.h/cpp | solveAlignment, BestFit, 3-2-1 |
| Point.h/cpp, Plane.h/cpp, Line.h/cpp, Circle.h/cpp | Geometry primitives |
