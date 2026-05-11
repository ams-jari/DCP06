# DCP06 build: post-build (MkEdit) and simulator

Visual Studio may report **zero compile/link errors** while the **post-build** scripts still log failures. This note explains the usual cases and what to do.

---

## 1. `File open failed` / `mkPhyObjOutOfSystem1300(...)` on `Plugin\DCP06\DCP06.dll`

### What it means

MkEdit’s **“Install application on local machine”** step copies or updates:

`…\Leica Geosystems\Leica Captivate\System\Plugin\DCP06\DCP06.dll`

(resolved from registry / `DeviceRootPathRelease` on your PC).

If Windows **cannot open** that file for update, MkEdit prints errors such as:

- `Error mkPhyObjOutOfSystem1300(...): opening szPhyObjFile: ...\DCP06.dll`
- `Error: File open failed`

The **new** `DCP06.dll` is still produced under the SDK tree, for example:

`%SYSTEM1500_ROOT%\Binary\RCL\Win32\Release\DCP06.dll`

### Typical cause: file lock

The DLL is **locked** while the **DCP06 plugin is loaded** inside the Captivate simulator session.

**Confirmed workflow:** With **Leica Captivate TS** still running, **exit the DCP06 application** (unload the plugin). That is often enough so the next build can overwrite `DCP06.dll`. If the error persists, **fully close** the TS simulator and rebuild.

### Workaround

1. Close DCP06 inside the simulator (and the simulator if needed).  
2. Rebuild, **or** manually copy `Release\DCP06.dll` into `…\System\Plugin\DCP06\`.

### Why Explorer shows an old timestamp on `DCP06.dll`

If MkEdit cannot write the destination file, `Plugin\DCP06\DCP06.dll` keeps its **previous** date — the build did not refresh that copy even though compile/link succeeded.

---

## 2. `can't build applications without valid dongle!` (second MkEdit / `.dxx`)

### What it means

The project runs **two** MkEdit passes in post-build (see `DCP06-VS2008.vcproj`): one with **`-I:…`** (install to local Captivate System), and a second pass to **generate loadable application output (`.dxx`)**. The **dongle** check applies to **that packaging/signing step**, not to compiling `DCP06.dll`.

### Development vs release

| Situation | Expectation |
|-----------|--------------|
| **Day-to-day dev / simulator** | **Dongle not required.** You mainly need a successful **`DCP06.dll`** (and **`DCP06.LEN`** when strings change). It is normal for the `.dxx` step to **fail without a dongle**. |
| **Official release builds** | Use a machine with a **valid Leica dongle** (colleague laptop, build PC, etc.) so MkEdit can complete the **release** `.dxx` workflow. |

MSVC ending with **“0 error(s)”** only reflects **compile/link**; MkEdit failures appear in **Build** output **after** linking.

---

## 3. UI still shows **old English text** after you changed tokens

### Source of truth (do not edit only `.men` by hand long-term)

On each Release build, **`scripts/build_lang.bat`** runs **`scripts/hpp_to_men.py`**, which **regenerates** `Text/Languages/en/DCP06.men` from **`Text/DCP06_TOK.HPP`**.  

The readable string for each token is the part in **`// "…"`** on that line in the HPP. Editing `DCP06.men` without updating the HPP comment is **overwritten** on the next build.

### `DCP06.LEN` not reaching the simulator you actually run

`TextTool` produces `DCP06.LEN`; the batch file copies it to:

1. Registry **`DeviceRootPathRelease`** → `…\Plugin\DCP06\en\`
2. A short list of **fixed paths** in `build_lang.bat` (Public Documents, and desktop “Captivate … simulator” folders)

If your TS install lives under a **different folder** (e.g. `Captivate simulator` vs `Captivate 10 simulator`), the **DLL** may update (MkEdit uses the registry path) while **`en\DCP06.LEN` stays old** — then you still see obsolete messages.

**Check:** In **the same** `…\Plugin\DCP06\en\` that your running simulator uses, confirm **`DCP06.LEN` modified time** matches your last build. If not, add that path to `build_lang.bat` or copy `Text\Languages\en\DCP06.LEN` there by hand. **Fully restart** the TS simulator after replacing `.LEN`.

---

## Related

- [DCP_SDK_Setup_Instructions.md](DCP_SDK_Setup_Instructions.md) — `SYSTEM1500_ROOT`, installers, simulator  
- [DCP06_Simulator_Fixes.md](DCP06_Simulator_Fixes.md) — TextTool `.LEN`, paths, fonts/icons  
- Post-build helpers: `scripts/post_build_only.bat`, `scripts/build_lang.bat`
