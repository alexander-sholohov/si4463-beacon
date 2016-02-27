#pragma once

//
// Developed by Alexander Sholohov <ra9yer@yahoo.com>
//

class CTimeRange
{
public:
    CTimeRange();
    long millisecondsElapsed();
    long secondsElapsed();
    void resetToNow();
private:
    unsigned long _stamp;

};
