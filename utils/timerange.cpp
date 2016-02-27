//
// Developed by Alexander Sholohov <ra9yer@yahoo.com>
//
#include "timerange.h"
#include <Arduino.h>

//--------------------------------------------------------
CTimeRange::CTimeRange()
{
    resetToNow();
}

//--------------------------------------------------------
void CTimeRange::resetToNow()
{
    _stamp = millis();
}


//--------------------------------------------------------
long CTimeRange::millisecondsElapsed()
{
    unsigned long current = millis();
    return current - _stamp;
}

//--------------------------------------------------------
long CTimeRange::secondsElapsed()
{
    return millisecondsElapsed() / 1000;
}
