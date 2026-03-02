# DCP05 vs DCP06: SDK Handling Analysis

**Purpose:** Document how DCP05 handles the Leica System 1500 SDK so DCP06 can align.

---

## 1. DCP05 Approach: No Fallbacks

DCP05 **does not** provide any stub or fallback headers. It assumes:

- `SYSTEM1500_ROOT` (Win32) or `SYSTEM1500_ROOT_x64` (x64) is set
- The full Leica Captivate System 1500 SDK is installed at that path
- All SDK headers (`UTL_Macro.hpp`, `Onboard_Tok.hpp`, `TXT_EXP.HPP`, `GUI_*`, etc.) come from the SDK

**There is no "build without SDK" mode in DCP05.**

---

## 2. Include Path Order (DCP05)

### Win32 (DCP05.vcproj / DCP05-CS.vcxproj)
```
..\..\Hdr;..\..\Text;..\..\Src;
$(System1500_ROOT)\Projects\;
$(System1500_ROOT)\Projects\3rdParty\boost\boost-dist;
$(System1500_ROOT)\Projects\ABL_Base\Source\API_Hdr;
... (all SDK API_Hdr paths)
```

### x64 (DCP05-CS.vcxproj)
```
..\..\Hdr;..\..\Text;..\..\Src;
$(System1500_ROOT_x64)\Projects\;
$(System1500_ROOT_x64)\Projects\3rdParty\WinCEAdapte;
$(System1500_ROOT_x64)\Projects\3rdParty\;
... (all SDK API_Hdr paths)
```

**Order:** Project paths first (Hdr, Text, Src), then SDK paths.

---

## 3. stdafx.h (DCP05)

```cpp
#if defined( WIN32 ) || defined( WIN64 )
    #include <windows.h>
    // ... DEBUG_NEW etc.
#else
    #include <vxworks.h>
#endif

// Global GUI includes (always, for Win/WinCE; vxworks has different flow)
#include <UTL_Macro.hpp>
#include <Onboard_Tok.hpp>
#include <DCP05_TOK.HPP>
#endif
```

- Uses `WIN32` / `WIN64` (not `_WIN32` / `_WIN64`)
- No conditional inclusion of UTL_Macro/Onboard_Tok
- DCP05_TOK.HPP is in project `Text/`; it `#include "TXT_EXP.HPP"` — TXT_EXP comes from the SDK (not in DCP05 repo)

---

## 4. Environment Variables

| Variable | DCP05 Usage |
|----------|-------------|
| SYSTEM1500_ROOT | Win32: include paths, OutDir, libraries, post-build tools |
| SYSTEM1500_ROOT_x64 | x64: same |
| INCSCAPE | Optional, for SWXResBuilder SVG conversion |

DCP05 does **not**:
- Read from `$(Env.SYSTEM1500_ROOT)` as fallback
- Define `System1500_ROOT` from env in the project file

The project expects the user to set these before opening Visual Studio (or via a Developer Command Prompt / batch that sets them).

---

## 5. SDK Header Location (DCP05 assumption)

Headers like `UTL_Macro.hpp`, `Onboard_Tok.hpp`, `TXT_EXP.HPP` are expected under:

- `$(System1500_ROOT)\Projects\` (root)
- `$(System1500_ROOT)\Projects\Utilities\Source\API_Hdr` (UTL_*)
- `$(System1500_ROOT)\Projects\GuiPlus\Source\API_Hdr` (GUI_*)

`TXT_EXP.HPP` is typically in a text/resource subsystem under the SDK (e.g. a text or RSC project).

---

## 6. DCP06 Differences (Current)

| Aspect | DCP05 | DCP06 (current) |
|--------|-------|------------------|
| Stubs | None | `include/sdk_fallback/` (UTL_Macro, Onboard_Tok), `Text/TXT_EXP.HPP` |
| Env fallback | None | `System1500_ROOT` from `$(Env.SYSTEM1500_ROOT)` |
| Include order | Project first, then SDK | SDK first, then project, then sdk_fallback |
| stdafx platform check | WIN32/WIN64 | _WIN32/_WIN64 |
| Project layout | Hdr/, Src/, Text/ | include/dcp06/, src/, Text/ |

---

## 7. Aligning DCP06 with DCP05

To match DCP05’s SDK handling:

### 7.1 Remove SDK fallbacks

1. Delete `include/sdk_fallback/` (UTL_Macro.hpp, Onboard_Tok.hpp)
2. Delete `Text/TXT_EXP.HPP`

These are not part of DCP05 and add divergence.

### 7.2 Include path order

Match DCP05: project paths first, then SDK:

```
..\..\include;..\..\include\dcp06\math;..\..\src\core;..\..\Text;
$(System1500_ROOT)\Projects\;
$(System1500_ROOT)\Projects\3rdParty\...;
... (rest of SDK paths)
```

Remove `..\..\include\sdk_fallback` from the end.

### 7.3 Environment variables

- Remove the PropertyGroup that sets `System1500_ROOT` from `$(Env.SYSTEM1500_ROOT)`
- Document that `SYSTEM1500_ROOT` and `SYSTEM1500_ROOT_x64` must be set before build (as in DCP05)

### 7.4 stdafx.h

- Optionally switch back to `WIN32` / `WIN64` for strict DCP05 parity (DCP05 uses these)
- Or keep `_WIN32` / `_WIN64` (MSVC defines these; more robust)

### 7.5 Prerequisites (same as DCP05)

- Leica Captivate System 1500 SDK installed
- `SYSTEM1500_ROOT` set for Win32, `SYSTEM1500_ROOT_x64` for x64
- Build only from an environment where these variables are set

---

## 8. Summary

DCP05 treats the SDK as mandatory: no stubs, no fallbacks. DCP06 currently adds stub headers and env fallbacks to allow building without the SDK.

To align DCP06 with DCP05:

1. Remove all SDK stubs and fallbacks.
2. Restore DCP05-style include order (project first, then SDK).
3. Stop deriving `System1500_ROOT` from the environment in the project.
4. Require `SYSTEM1500_ROOT` / `SYSTEM1500_ROOT_x64` to be set and the full SDK to be present.

If DCP06 needs to build without the SDK (e.g. for CI or developers without the license), that would remain a DCP06-specific extension and should be clearly separated from the “DCP05-compatible” configuration.
