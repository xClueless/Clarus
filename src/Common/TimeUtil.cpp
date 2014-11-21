#include "TimeUtil.h"

#if defined(_WIN32)
void sleepSeconds(uint32_t numSeconds)
{
    Sleep(numSeconds * 1000);
}
#else
void sleepSeconds(uint32_t numSeconds)
{
    sleep(numSeconds * 1000);
}
#endif
