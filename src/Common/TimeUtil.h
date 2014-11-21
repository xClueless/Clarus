#ifndef TIMEUTIL_H
#define TIMEUTIL_H

#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdint.h>

void sleepSeconds(uint32_t numSeconds);

#endif // TIMEUTIL_H
