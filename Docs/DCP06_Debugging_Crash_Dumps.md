# DCP06 / Captivate – Debugging Crash Dumps

This guide explains how to capture and analyze crash dumps when Leica Captivate or DCP06 crashes.

---

## 1. Capturing a Dump

### When the crash dialog appears

The error dialog often asks:

> **"'LeicaCaptivateTS.exe' error! Do you want to write a dump file to C:\Users\<user>\AppData\Local\Temp\"**

- Click **"Yes" (Kyllä)** to write the dump.
- The dump is usually written to:  
  `C:\Users\<username>\AppData\Local\Temp\`

### Expected dump files

- `LeicaCaptivateTS.dmp` – minidump (~400 KB)
- `LeicaCaptivateTS_FULLMEM.dmp` – full memory dump (~500+ MB), if created

### If you didn’t find the dump

1. Open `%TEMP%` in Explorer (Win+R → `%TEMP%`).
2. Sort by **Date modified** (newest first).
3. Look for `LeicaCaptivateTS.dmp` or `*.dmp` from the crash time.

---

## 2. Prerequisites

### Windows Debugging Tools (WinDbg / CDB)

1. Install **Windows SDK** or **Windows Driver Kit (WDK)**:
   - https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/
   - Or via Visual Studio Installer → “Individual components” → Windows SDK

2. Default location of CDB:
   - **x86:** `C:\Program Files (x86)\Windows Kits\10\Debuggers\x86\cdb.exe`
   - **x64:** `C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\cdb.exe`

   Captivate is usually 32-bit; use the **x86** debugger for `LeicaCaptivateTS.dmp`.

### Alternative: Visual Studio

- Open the `.dmp` file directly in Visual Studio.
- VS will offer to debug it and often shows a summary of the crash.

---

## 3. Quick Analysis with CDB (Command Line)

### Check dump validity (dumpchk)

```cmd
"C:\Program Files (x86)\Windows Kits\10\Debuggers\x86\dumpchk.exe" "C:\Users\dell\AppData\Local\Temp\LeicaCaptivateTS.dmp"
```

This verifies the dump and prints basic info (exception, threads, streams).

### Get exception and stack (CDB)

```cmd
"C:\Program Files (x86)\Windows Kits\10\Debuggers\x86\cdb.exe" -z "C:\Users\dell\AppData\Local\Temp\LeicaCaptivateTS.dmp" -c "kP;q"
```

- `-z` = open crash dump
- `-c "kP;q"` = run command `kP` (stack with parameters) and quit

### Full automated analysis

```cmd
"C:\Program Files (x86)\Windows Kits\10\Debuggers\x86\cdb.exe" -z "C:\path\to\LeicaCaptivateTS.dmp" -c "!analyze -v;q"
```

This can take a minute. Output includes:
- Faulting module
- Exception code
- Call stack
- Suggested cause

---

## 4. Interactive Analysis with CDB

### Start CDB on the dump

```cmd
cdb -z "C:\Users\dell\AppData\Local\Temp\LeicaCaptivateTS.dmp"
```

### Useful commands

| Command | Description |
|---------|-------------|
| `.ecxr` | Switch to exception context (faulting thread) |
| `k` | Show call stack (short) |
| `kP` | Call stack with parameters |
| `kL` | Call stack with source lines (if symbols loaded) |
| `!analyze -v` | Full automated analysis |
| `lm` | List loaded modules |
| `lm v m DCP06*` | List modules matching DCP06 |
| `q` | Quit |

### Example flow

```
0:021> .ecxr
0:021> kP
... (stack trace) ...
0:021> !analyze -v
... (analysis) ...
0:021> q
```

---

## 5. Common Exception Codes

| Code | Name | Typical cause |
|------|------|----------------|
| 0xc0000005 | Access violation | NULL pointer, invalid address, buffer overrun |
| 0xc0000409 | STATUS_STACK_BUFFER_OVERRUN | Stack corruption |
| 0xc000007b | STATUS_INVALID_IMAGE_FORMAT | Wrong bitness (32 vs 64) or corrupt DLL |
| 0xe0434352 | CLR exception | .NET exception in managed code |

---

## 6. Interpreting a Crash

### Focus on

1. **Faulting instruction** – What instruction crashed?  
   - `mov ecx, [eax]` with eax=0 → NULL pointer dereference  
   - Access to a low/invalid address → often NULL or bad pointer  

2. **Faulting module** – Which DLL or exe?
   - `LeicaCaptivateTS.exe` – main app
   - `DCP06.dll` – DCP06 plugin
   - `HALTools.dll`, `GUIPlus.dll` – Leica framework
   - `ntdll.dll` – Windows; often appears when resources are missing or invalid

3. **Stack trace** – Call chain leading to the crash  
   - `LdrpResSearchResourceMappedFile` → loading resources (icons, etc.)
   - DCP06-related frames → problem in DCP06 or its resources

### Resource/icon-related crashes

If the stack shows resource loading (e.g. `LdrpResSearchResourceMappedFile`) and the fault is in `HALTools` or similar:

- Check that all icons in `SWXRes` exist and are valid PNGs.
- Ensure `DCP06_logo_1X.png` and `DCP06_logo_2X.png` are present for boot.
- Do a clean rebuild so `SWXResBuilder` and `MkEdit` regenerate and package resources correctly.

---

## 7. Symbol Path (Optional)

For better stack traces (function names instead of raw addresses):

1. Set symbol path in CDB:
   ```
   .sympath srv*c:\symbols*https://msdl.microsoft.com/download/symbols
   ```
2. Or via environment variable:
   ```
   set _NT_SYMBOL_PATH=srv*c:\symbols*https://msdl.microsoft.com/download/symbols
   ```

---

## 8. Quick Reference – One-Liner

To get a quick crash summary:

```cmd
"C:\Program Files (x86)\Windows Kits\10\Debuggers\x86\cdb.exe" -z "%TEMP%\LeicaCaptivateTS.dmp" -c ".ecxr;kP;!analyze -v;q" > crash_report.txt
```

Then open `crash_report.txt` and look for “FAULTING_IP”, “EXCEPTION_RECORD”, and “STACK_TEXT”.

---

*Document version: 1.0 | March 2026*
