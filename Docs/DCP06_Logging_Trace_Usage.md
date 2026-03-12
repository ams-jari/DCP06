# DCP06 Logging and Trace Usage

How to add debug traces without hitting MSVC build errors.

---

## Quick reference

| Use case | Macro | Example |
|----------|-------|---------|
| Function entry | `DCP06_TRACE_ENTER` | At start of function |
| Function exit | `DCP06_TRACE_EXIT` | Before return |
| Message with format args | `DCP06_TRACE_POINT(fmt, ...)` | `DCP06_TRACE_POINT("x=%d", x)` |
| Message without format args | `DCP06_LOG_DEBUG("-- %s: msg", __FUNCTION__)` | See below |

---

## MSVC compatibility: DCP06_TRACE_POINT

`DCP06_TRACE_POINT` uses a variadic macro. **MSVC (VS2008, Win32) does not handle empty `##__VA_ARGS__` correctly**, which causes:

```
error C2059: syntax error : ')'
```

### Safe usage (always works)

```cpp
DCP06_TRACE_POINT("selected=%d", (int)unId);
DCP06_TRACE_POINT("lExitCode=%d", lExitCode);
DCP06_TRACE_POINT("value=%d name=%s", id, name);
```

**Rule:** Always pass at least one extra argument after the format string.

### Unsafe usage (causes build error on MSVC)

```cpp
DCP06_TRACE_POINT("calling ShowMidPointDlg");   // NO – no format args
DCP06_TRACE_POINT("done");                      // NO – no format args
```

### Alternative for messages without format args

Use `DCP06_LOG_DEBUG` directly:

```cpp
DCP06_LOG_DEBUG("-- %s: calling ShowMidPointDlg", __FUNCTION__);
DCP06_LOG_DEBUG("-- %s: done", __FUNCTION__);
```

---

## Enabling logs

- Set `Logger::setEnabled(true)` (e.g. from `m_nAmsLog` or simulator config).
- Set `Logger::setLevel(Logger::Debug)` for full trace visibility.
- Log file: typically `DCP06.log` in the application data path.

---

## See also

- `include/dcp06/core/Logger.hpp` – macro definitions and inline comments
- `Docs/DCP06_Debugging_Crash_Dumps.md` – crash dump analysis
