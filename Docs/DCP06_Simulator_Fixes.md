# DCP06 Simulator Fixes (XX strings, icon, DCP06.log)

## TextTool / DCP06.LEN fix (March 2026)

**Problem:** TextTool was not producing DCP06.LEN; only DCP06.L (32 bytes) was updated. DCP06.LEN stayed at an old timestamp.

**Cause:** TextTool requires `LangAndTypes.xml` in the same folder as the .men file to produce .LEN. Without it, only .L and HPP/IPP/LUA are generated.

**Fix:** Added `Text/Languages/en/LangAndTypes.xml` (from SDK HelloWorld sample) to the repo. The build now produces a fresh DCP06.LEN on each run.

## Summary of changes

### 1. "XX" strings (missing localization)

**Cause:** Several modules used `AT_DCP05` (15750) instead of `AT_DCP06` (15751), so they loaded strings from DCP05.LEN. DCP06.LEN was also only copied to the registry simulator path, which might not match where you run the simulator.

**Fixes:**
- Replaced all `AT_DCP05` with `AT_DCP06` in: `3dmeas.cpp`, `LineFitting.cpp`, `ResCircle.cpp`, `3dfbs.cpp`, `ResShaft.cpp`, `ResLine.cpp`, `BestFit.cpp`, `Chst.cpp`, `Alignment321UserDef.cpp`, `Shaft.cpp`, `Calculation321.cpp`, `CalculationPlane.cpp`, `Common.cpp`, `CalculationCircleCore.cpp`, `CalculationLine.cpp`, `MeasXYZ.cpp`, `ScanMeasXYZ.cpp`
- Updated `build_lang.bat` to copy `DCP06.LEN` to multiple simulator paths:
  - Registry: `DeviceRootPathRelease\Leica Geosystems\Leica Captivate\System\Plugin\DCP06\en\`
  - `C:\Users\Public\Documents\Leica Captivate\CS_x64\...\Plugin\DCP06\en\`
  - `C:\Users\Public\Documents\Leica Captivate\CS_32\...\Plugin\DCP06\en\`

**Action:** Rebuild the project so `build_lang.bat` runs and copies `DCP06.LEN` to all paths. Ensure your simulator uses one of these locations.

### 2. DCP06 icon on Captivate entry

**Cause:** `DCP06.sys` expects `DCP06_logo_1X.png` and `DCP06_logo_2X.png`, but the build may produce `DCP06_LOGO_2X.png` (uppercase).

**Fix:** Added a post-build step in `DCP06-VS2022.vcxproj` that copies `DCP06_LOGO_1X.png` → `DCP06_logo_1X.png` and `DCP06_LOGO_2X.png` → `DCP06_logo_2X.png` when the lowercase versions are missing.

### 3. DCP06.log location (debug traces)

**Cause:** When `CPI::SensorC::GetInstance()->GetPath()` fails (e.g. on simulator), the log used `DCP06.log` in the current working directory, which can be hard to find.

**Fixes:**
- Fallback path: `%TEMP%\DCP06.log` (e.g. `C:\Users\<user>\AppData\Local\Temp\DCP06.log`)
- Startup log line: `DCP06.log path: <path>` is written as the first log entry so you can see where the file is.

**Where to find DCP06.log:**
1. If `GetPath` succeeds: `<storage_path>/DCP06/DCP06.log`
2. Otherwise (simulator): `%TEMP%\DCP06.log` — open `%TEMP%` in Explorer or run `echo %TEMP%` in cmd
3. Last resort: `DCP06.log` in the process current directory (often the Captivate install folder)

The first line in the log file will show the resolved path.
