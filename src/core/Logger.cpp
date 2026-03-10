#include "stdafx.h"
#include <dcp06/core/Logger.hpp>
#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#endif

namespace DCP {

char Logger::s_path[256] = "";
Logger::Level Logger::s_level = Logger::Debug;  // DEBUG for initial testing (full visibility)
bool Logger::s_enabled = true;
bool Logger::s_inited = false;

void Logger::init() {
    if (s_inited) return;
    s_inited = true;

    // Default: DCP06.log in current directory (works for simulator)
    if (s_path[0] == '\0')
        snprintf(s_path, sizeof(s_path), "DCP06.log");

    // Clear log on each DCP06 start - only current run in file
    FILE* f = fopen(s_path, "w");
    if (f) fclose(f);
}

void Logger::setLogPath(const char* path) {
    if (!path || !path[0]) return;
    snprintf(s_path, sizeof(s_path), "%s", path);
    s_inited = true;
    // Clear log on each DCP06 start
    FILE* f = fopen(s_path, "w");
    if (f) fclose(f);
}

void Logger::ensureInit() {
    if (!s_inited) init();
}

void Logger::writeLine(const char* levelStr, const char* msg) {
    ensureInit();
    if (s_path[0] == '\0') return;

    FILE* f = fopen(s_path, "a");
    if (!f) return;

    time_t t = time(0);
    struct tm buf;
#ifdef _WIN32
    if (localtime_s(&buf, &t) != 0) { fclose(f); return; }
#else
    if (!localtime_r(&t, &buf)) { fclose(f); return; }
#endif

    char timeBuf[32];
    snprintf(timeBuf, sizeof(timeBuf), "%04d-%02d-%02d %02d:%02d:%02d",
        buf.tm_year + 1900, buf.tm_mon + 1, buf.tm_mday,
        buf.tm_hour, buf.tm_min, buf.tm_sec);

    fprintf(f, "[%s] [%s] %s\n", timeBuf, levelStr, msg);
    fflush(f);
    fclose(f);
}

void Logger::log(Level l, const char* fmt, ...) {
    if (!s_enabled || l < s_level) return;
    ensureInit();

    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    const char* levelStr = "???";
    switch (l) {
        case Debug:   levelStr = "DBG"; break;
        case Info:    levelStr = "INF"; break;
        case Warning: levelStr = "WRN"; break;
        case Error:   levelStr = "ERR"; break;
    }
    writeLine(levelStr, buf);
}

void Logger::debug(const char* fmt, ...) {
    if (!s_enabled || Debug < s_level) return;
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    writeLine("DBG", buf);
}

void Logger::info(const char* fmt, ...) {
    if (!s_enabled || Info < s_level) return;
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    writeLine("INF", buf);
}

void Logger::warn(const char* fmt, ...) {
    if (!s_enabled || Warning < s_level) return;
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    writeLine("WRN", buf);
}

void Logger::error(const char* fmt, ...) {
    if (!s_enabled || Error < s_level) return;
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    writeLine("ERR", buf);
}

}  // namespace DCP
