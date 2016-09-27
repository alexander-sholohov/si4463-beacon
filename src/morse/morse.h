#pragma once

//
// Developed by Alexander Sholohov <ra9yer@yahoo.com>
//

#include "../utils/timerange.h"

#define MORSE_MAX_TEXT_LENGTH (80)


class CMorse
{
    enum MorseState
    {
        msStopped = 0,
        msWarmUpPause,
        msSendingDit,
        msSendingDah,
        msSendingPauseBetweenDitDah,
        msSendingPauseBetweenLetters,
        msSendingSpace,
        msSendingLongCarrier,
        msEndOfText,
        msLongPauseBetweenRepeat
    };

public:
    CMorse();
    void setText(const char* text);
    void handleTimeout();
    bool isTransmittingActive() const;
    bool isToneActive() const;

    void start();
    void stop();

private:  
    static const unsigned char _morseTable[42*2];

    MorseState _state;

    char _text[MORSE_MAX_TEXT_LENGTH];
    int _currentPosInText;

    CTimeRange _time;
    long _currentTimeoutInMS;

    unsigned char _currentUtitsPattern;
    int _numUnitsRest;

    MorseState prepareNextChar();
    MorseState prepareNextDitDah();
    bool findCharInTable(char ch, int& length, unsigned char & pattern) const;
    void changeStateByTimeout();
    long getTimeoutForState(MorseState state) const;


};
