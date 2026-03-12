#pragma once

// DCP06 logging for simulator debugging (DCP9-style)
// When testing on simulator: no call stack, no debugger - just this log.
// Enable in release builds for simulator testing.
//
// Trace usage: see Docs/DCP06_Logging_Trace_Usage.md (MSVC-safe patterns).
//
// Trace levels:
//   DEBUG   = All possible info: entry, exit, function critical info (most verbose).
//             Use for initial testing and debugging. DCP06_TRACE_ENTER/EXIT/POINT use this.
//   INFO    = Function entry and exit only (less verbose).
//   WARNING = Notable conditions that may need attention.
//   ERROR   = Failures, exceptions, unexpected states.
//
// For initial testing: set level to DEBUG for full visibility.

#include <dcp06/core/Defs.hpp>
#include <cstdio>
#include <ctime>
#include <cstring>

namespace DCP {

class Logger {
public:
    enum Level { Debug = 0, Info = 1, Warning = 2, Error = 3 };

    /// Initialize logger (call early, e.g. at app start). Sets log path.
    static void init();

    /// Set log file path explicitly (e.g. from CPI::SensorC::GetPath). Call before first log.
    static void setLogPath(const char* path);

    /// Set minimum level to log. Debug=full visibility; Info=entry/exit only; Warning/Error=obvious.
    static void setLevel(Level l) { s_level = l; }

    /// Check if logging is enabled (e.g. via m_nAmsLog or always for simulator)
    static void setEnabled(bool en) { s_enabled = en; }
    static bool isEnabled() { return s_enabled; }

    /// Log message (printf-style). Thread-unsafe; single-threaded Captivate.
    static void log(Level l, const char* fmt, ...);

    /// Convenience macros
    static void debug(const char* fmt, ...);
    static void info(const char* fmt, ...);
    static void warn(const char* fmt, ...);
    static void error(const char* fmt, ...);

    /// Get current log file path (for diagnostics)
    static const char* getLogPath() { return s_path; }

private:
    static char s_path[256];
    static Level s_level;
    static bool s_enabled;
    static bool s_inited;

    static void ensureInit();
    static void writeLine(const char* levelStr, const char* msg);
};

}  // namespace DCP

// Macros for easy use (similar to DCP9)
#define DCP06_LOG_INFO(...)   do { if (DCP::Logger::isEnabled()) DCP::Logger::info(__VA_ARGS__); } while(0)
#define DCP06_LOG_DEBUG(...) do { if (DCP::Logger::isEnabled()) DCP::Logger::debug(__VA_ARGS__); } while(0)
#define DCP06_LOG_WARN(...)  do { if (DCP::Logger::isEnabled()) DCP::Logger::warn(__VA_ARGS__); } while(0)
#define DCP06_LOG_ERR(...)   do { if (DCP::Logger::isEnabled()) DCP::Logger::error(__VA_ARGS__); } while(0)

// Entry/exit tracing (DCP9-style) - use at function start and before each return.
// These use DEBUG level; set Logger::setLevel(Logger::Debug) for full visibility.
#define DCP06_TRACE_ENTER    DCP06_LOG_DEBUG(">> %s", __FUNCTION__)
#define DCP06_TRACE_EXIT     DCP06_LOG_DEBUG("<< %s", __FUNCTION__)

// DCP06_TRACE_POINT: logs "-- function_name: <fmt>" with optional printf-style args.
// MSVC (VS2008/Win32) does not handle empty ##__VA_ARGS__ correctly - causes C2059 syntax error.
// SAFE:   DCP06_TRACE_POINT("value=%d", x);         // at least one format arg
// UNSAFE: DCP06_TRACE_POINT("msg only");            // no extra args -> build error
// For messages without format args, use: DCP06_LOG_DEBUG("-- %s: msg", __FUNCTION__);
#define DCP06_TRACE_POINT(fmt, ...) DCP06_LOG_DEBUG("-- %s: " fmt, __FUNCTION__, ##__VA_ARGS__)
