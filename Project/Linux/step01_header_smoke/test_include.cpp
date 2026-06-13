// Step 1: verify POSIX SDK headers compile on Linux (no link to Leica libs yet).

#include "SurveyDefines.hpp"
#include "CPI_Types.hpp"
#include "TPI_InstrumentInfo.hpp"

#include <cstdio>

int main()
{
    const int ts20 = static_cast<int>(TPI::InstrumentInfoC::IMO_TS20);
    std::printf("POSIX SDK header smoke test OK (IMO_TS20=%d)\n", ts20);
    return (ts20 == 50) ? 0 : 1;
}
