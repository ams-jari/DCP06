# DCP06 Boot Crash – Analysis (Build Log + Dump)

## Summary

**Crash:** Access violation (0xc0000005) in HALTools.dll during resource loading when Captivate boots.

**Root cause (build log):** SWXResBuilder **fails** to convert `DCP06_logo.svg` → PNG. The logo icons are not regenerated, and the packaged `DCP06_Res.pob` may contain missing or invalid resources.

**Not caused by 3dmeas changes:** All recent changes are in `3dmeas.cpp` and run only when the user opens Measurement → 3D-MEAS. The crash occurs at Captivate startup, before any DCP06 UI code runs.

---

## Build Log Evidence

```
Error converting c:\Users\dell\Desktop\AMS\Development\DCP06\Project\MSVS\../../SWXRes_Source\DCP06_logo.svg 
to c:\Users\dell\Desktop\AMS\Development\DCP06\Project\MSVS\..\..\SWXRes\DCP06_logo_2X.png!
Prosessia ei voi aloittaa, koska tiedoston nimeä ei ole määritetty.
```

**Translation:** "The process cannot be started because the file name has not been specified."

**Meaning:** SWXResBuilder uses Inkscape to convert SVG → PNG. The `%INCSCAPE%` environment variable (path to Inkscape) is not set or is empty, so the conversion never runs. The logo PNGs are not updated and may be missing or stale.

---

## Dump Analysis

- **Exception:** Access violation (0xc0000005)
- **Fault:** `HALTools+0x61910: mov ecx, [eax]` with **eax = 0** (NULL)
- **Context:** `ntdll!LdrpResSearchResourceMappedFile` – loading resources from a mapped file

Captivate loads DCP06 and its resources (including the logo) from `DCP06_Res.pob`. If the logo resource is missing or corrupt, the loader can return NULL, and HALTools dereferences it → crash.

---

## Why 3dmeas Changes Are Not the Cause

| What runs at boot | What runs when opening 3D-MEAS |
|-------------------|--------------------------------|
| Captivate loads DCP06.dll | User opens Measurement → 3D-MEAS |
| Captivate loads DCP06_Res.pob (icons) | Meas3DController::OnControllerActivated |
| Crash occurs in HALTools during resource load | 3dmeas job/add-point code runs |

The crash happens before any 3dmeas or job-related code executes.

---

## Fix: SWXResBuilder / Logo Generation

### Option A: Set INCSCAPE (recommended)

1. Install Inkscape: https://inkscape.org/
2. Set the environment variable, e.g.:
   ```cmd
   set INCSCAPE=C:\Program Files\Inkscape\bin\inkscape.exe
   ```
3. Rebuild. SWXResBuilder will regenerate logo PNGs from the SVG.

### Option B: Rely on existing logo PNGs

If `SWXRes` already has valid `DCP06_logo_1X.png` and `DCP06_logo_2X.png` (or `DCP06_LOGO_2X.png`), ensure they are present in the **folder used by MkEdit** for packaging.

MkEdit uses `%SYSTEM1500_ROOT%\Applications\Common\DCP06\SWXRes\`. If your DCP06 project is elsewhere, the packaged content may come from a different `SWXRes` folder. Confirm that the logo files in that folder are valid PNGs and not corrupt.

### Option C: Disable SVG conversion

If you no longer need to regenerate logos from SVG, you can adjust the post-build step so SWXResBuilder is skipped and only the logo copy step runs.

---

## Verifying the fix

1. Set `INCSCAPE` (or ensure valid logo PNGs) and rebuild.
2. Confirm the post-build step runs without the SWXResBuilder error.
3. Run Captivate and check that it starts without crashing.

---

---

## Update: Malformed logo file found (March 2026)

**Root cause identified:** A file `DCP06_logo_2X.png.png` (double `.png` extension) existed in SWXRes. This can confuse resource packaging/loading. DCP06.sys expects `DCP06_logo_2X.png`.

**Fix applied:** Removed `DCP06_logo_2X.png.png` from both:
- `DCP06\SWXRes\`
- `Captivate_PluginSDK...\Applications\Common\DCP06\SWXRes\`

On Windows (case-insensitive), `DCP06_LOGO_2X.png` and `DCP06_logo_2X.png` refer to the same file, so the correct logo was already present. The malformed `.png.png` file was the problematic extra.

**Next step:** Rebuild and test Captivate.

---

## Dump Limitation: Resource Name Not Available

**Question:** Does the dump tell us which resource is missing?

**Answer:** No. The dump shows:
- **What:** NULL pointer read in `HALTools+0x61910`
- **When:** During resource loading (call chain involves `LdrpResSearchResourceMappedFile`)
- **Which resource:** Not available. HALTools is closed-source; without its symbols we cannot decode the resource name from the stack or registers. The fault is generic: something returned NULL and HALTools used it without checking.

**What we know from config:** `DCP06.sys` requires `DCP06_logo_$SCALEFACTOR$.png` (1X and 2X) at boot. The logo files exist in the simulator `Plugin\DCP06\Res\` folder. If the crash persists, possible causes:
- Corrupt or invalid PNG (wrong format, dimensions, or color depth)
- Different resource failing (logo is just the first we expect)
- Another plugin’s resource causing the fault while loading DCP06

---

## Diagnostic: Isolate DCP06

To confirm the crash is DCP06-specific:

1. Rename the DCP06 folder in the simulator, e.g.  
   `Plugin\DCP06` → `Plugin\DCP06_disabled`
2. Start Captivate.
3. If it boots: the problem is with DCP06 or its resources.
4. If it still crashes: the problem may be elsewhere (another plugin or base system).

---

*Analysis date: March 2026*
