# DCP06 Code Cleanup Status

## Completed

### 1. Defines and constants (Defs.hpp)
- **Added buffer size constants**: `STRING_BUFFER_SMALL` (20), `STRING_BUFFER_MEDIUM` (100), `STRING_BUFFER_LARGE` (512), `STRING_BUFFER_PATH` (1024)
- **Added geometry limits**: `MIN_POINTS_FOR_DISTANCE`, `MIN_POINTS_FOR_LINE`, `MIN_POINTS_FOR_PLANE`, `MIN_POINTS_FOR_CIRCLE`, `BOUNDARY_PLANE_POINTS`, `DEFAULT_BOUNDARY_SIZE`
- **Renamed**: `DCP06_CUSTOMER` → `CUSTOMER_BUILD`, `DCP06_JOBS` → `FILE_TYPE_JOBS`

### 2. Magic numbers replaced with defines
- `char temp[20/100/512/1024]` → `STRING_BUFFER_*` constants
- `m_iMinPoint = 3` → `MIN_POINTS_FOR_DISTANCE`, `MIN_POINTS_FOR_LINE`, `MIN_POINTS_FOR_PLANE`, `MIN_POINTS_FOR_CIRCLE`, `BOUNDARY_PLANE_POINTS`
- `500.0` boundary → `DEFAULT_BOUNDARY_SIZE`

### 3. Cryptic variable names improved
| Old | New |
|-----|-----|
| `temp` | `asciiBuffer`, `formatBuffer`, `outputBuffer`, `lineBuffer`, `serialBuffer`, `keyCodeBuffer` |
| `temp1`, `temp2` | `userBuffer`, `valueBuffer`, `noteBuffer`, `stationBuffer` |
| `para`, `parb`, `parc` | `directionX`, `directionY`, `directionZ` |
| `des_ok` | `designValuesValid` |
| `ff` | `planeFitValid` |
| `xtemp`, `ytemp`, `ztemp` | `sumX`, `sumY`, `sumZ` |
| `tmp` | `distanceOffset`, `swapValue` |
| `ttt` | `asciiBuffer` |

### 4. Files modified
- **Defs.hpp**: New constants, renamed DCP06_* macros
- **CalculationDist.cpp**, **CalculationAngle.cpp**: Buffer renames, MIN_POINTS
- **CalculationLine.cpp**, **CalculationPlane.cpp**: para/parb/parc → directionX/Y/Z, des_ok → designValuesValid
- **PlaneScanning.cpp**: Boundary constants, buffer renames
- **HiddenPoint.cpp**: Coordinate and parameter renames
- **Application.cpp**: serialBuffer, hashValues, keyCodeBuffer
- **LineFitting.cpp**: filePathBuffer, lineBuffer, userBuffer, valueBuffer
- **BestFit.cpp**, **Chst.cpp**: formatBuffer, note buffers
- **MeasXYZ.cpp**: logFilenameBuffer, logLineBuffer, pointNoBuffer
- **DefinePlane.cpp**, **DefinePlaneUserDef.cpp**, **Circle.cpp**, **CalculationCircle.cpp**: MIN_POINTS
- **File.cpp**, **SelectFile.cpp**: FILE_TYPE_JOBS

---

## Remaining (large tasks)

### 1. DCP06-prefixed class renames (~120+ classes)
| Current | Suggested |
|---------|-----------|
| `DCP06ModelC` | `Model` |
| `DCP06CalcDistDlgC` | `CalculationDistanceDialog` |
| `DCP06CalcAngleDlgC` | `CalculationAngleDialog` |
| `DCP06InitDlgC` | `InitializationDialog` |
| `DCP06SelectMultiPointsControllerC` | `SelectMultiPointsController` |
| ... | ... |

**Scope**: Affects ~100+ files. Requires coordinated header/cpp/usage updates. Recommend doing in batches (e.g. core model first, then dialogs, then controllers).

### 2. ANSI C → C++
- `char[N]` buffers → `std::string` or `std::array` where safe
- C-style casts → `static_cast`/`dynamic_cast`/`reinterpret_cast`
- `sprintf` → `std::ostringstream` or `snprintf` (buffer-safe)
- `memcpy`/`memset` → consider `std::copy`, `std::fill` where appropriate

### 3. Member variable naming
- `m_pDCP06Model` → `m_pModel`
- `pDCP06Model` → `pModel`
- `GetDCP06Model()` → `GetModel()` (when class is renamed)

---

## Build verification

After changes, rebuild with Visual Studio:
- **Configuration**: Release | Win32
- **Project**: DCP06-VS2022.vcxproj (or DCP06-VS2008.vcproj)

If any file fails to compile due to missing `Defs.hpp` or undefined constants, add:
```cpp
#include <dcp06/core/Defs.hpp>
```
