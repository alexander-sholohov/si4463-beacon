#pragma once

//
// Developed by Alexander Sholohov <ra9yer@yahoo.com>
//

#include <Arduino.h>
#include <Stream.h>

class CSi4463
{
    enum SiResultCode {
      rcSuccess = 0,
      rcWrongCmd,
      rcWrongLength,
      rcNoCTS
    };

public:
    CSi4463(uint8_t nSEL, uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t ook);

    void setDebugSerial(Stream* serial);

    bool initialize();
    void activateTX();
    void activateRX();
    void controlOOK(bool value);

    SiResultCode execCommand(size_t numBytesToWrite, const byte* writeData, size_t numBytesToRead, byte* readData);

private:
    uint8_t _nSEL;
    uint8_t _mosi;
    uint8_t _miso;
    uint8_t _sck;
    uint8_t _ook;
    Stream* _debugSerial;

    void beginCommunication();
    void endCommunication();
    byte writeByte(byte out);
    bool isSendCommandAlowed();
    bool waitForCTS();
    SiResultCode clearIntStatus();
    SiResultCode execCmdChain(const byte *data);

};
