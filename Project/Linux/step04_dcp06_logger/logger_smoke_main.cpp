// Step 4: exercise DCP06 Logger on Linux (writes /tmp/dcp06_step04.log).

#include "stdafx.h"
#include <dcp06/core/Logger.hpp>

#include <cstdio>
#include <cstring>

int main()
{
    const char* logPath = "/tmp/dcp06_step04.log";
    DCP::Logger::setLogPath(logPath);
    DCP::Logger::setLevel(DCP::Logger::Debug);
    DCP::Logger::info("DCP06 Logger POSIX step 4 smoke test");

    FILE* f = fopen(logPath, "r");
    if (!f)
    {
        std::printf("FAIL: could not read log file: %s\n", logPath);
        return 1;
    }

    char line[256];
    if (!fgets(line, sizeof(line), f))
    {
        fclose(f);
        std::printf("FAIL: log file empty: %s\n", logPath);
        return 1;
    }
    fclose(f);

    if (std::strstr(line, "DCP06 Logger POSIX step 4") == 0)
    {
        std::printf("FAIL: unexpected log line: %s", line);
        return 1;
    }

    std::printf("DCP06 Logger smoke test OK (%s)\n", logPath);
    std::printf("  %s", line);
    return 0;
}
