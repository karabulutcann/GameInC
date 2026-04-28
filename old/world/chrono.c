#include "core/chrono.h"
#include "core/result.h"
#include <time.h>

struct Clock {
    clock_t startTime;
};

static struct Clock staticClock;

struct Result clockCreate()
{
    staticClock = (struct Clock){0};
    staticClock.startTime = clock();
    return ok();
}

clock_t clockGetTime(f4 *dest)
{
}

struct Result clockDestroy()
{
    return ok();
}