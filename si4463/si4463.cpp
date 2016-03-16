
//
// Developed by Alexander Sholohov <ra9yer@yahoo.com>
//

#include "si4463.h"


#include "radio/radio_config.h"
// TODO: switch data segment PROGMEM
const byte Radio_Configuration_Data_Array[]  = RADIO_CONFIGURATION_DATA_ARRAY;


#define MYNOP _NOP

#ifndef NO_MY_DEBUG
#define MYDEBUG(STR) do{ if(_debugSerial) { _debugSerial->println(F(STR)); } }while(0)
#else
#define MYDEBUG(STR) do{ } while(0)
#endif

//------------------------------------------------------------------------------------------
CSi4463::CSi4463(uint8_t nSEL, uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t ook)
    : _nSEL(nSEL)
    , _mosi(mosi)
    , _miso(miso)
    , _sck(sck)
    , _ook(ook)
    , _debugSerial( NULL )
{
}

//------------------------------------------------------------------------------------------
void CSi4463::setDebugSerial(Stream* serial)
{
    _debugSerial = serial;
}

//------------------------------------------------------------------------------------------
bool CSi4463::initialize()
{
    SiResultCode rc = execCmdChain(Radio_Configuration_Data_Array);
    if( rc == rcSuccess )
    {
        MYDEBUG("si init OK");
    }
    else
    {
        MYDEBUG("si init Error");
    }
    return ( rc == rcSuccess );
}

//------------------------------------------------------------------------------------------
void CSi4463::activateTX()
{
    clearIntStatus();
    
    byte channel = 0;
    byte packet[] = { 0x31, channel,0,0, 0,0,0 };
    SiResultCode rc = execCommand(7, packet, 0, NULL);
    if( rc == rcSuccess )
    {
        MYDEBUG("tx OK");
    }
    else
    {
        MYDEBUG("tx Error");
    }
}

//------------------------------------------------------------------------------------------
void CSi4463::activateRX()
{
    clearIntStatus();
    
    byte state = 3;
    byte packet[] = { 0x34, state };
    SiResultCode rc = execCommand(2, packet, 0, NULL);
    if( rc == rcSuccess )
    {
        MYDEBUG("ready OK");
    }
    else
    {
        MYDEBUG("ready err");
    }
}

//------------------------------------------------------------------------------------------
void CSi4463::controlOOK(bool value)
{
    digitalWrite(_ook, (value)? HIGH : LOW ) ;
}

//------------------------------------------------------------------------------------------
void CSi4463::beginCommunication()
{
    digitalWrite(_mosi, LOW);
    digitalWrite(_sck, LOW);
    digitalWrite(_nSEL, LOW);
}

//------------------------------------------------------------------------------------------
void CSi4463::endCommunication()
{
    digitalWrite(_mosi, LOW);
    digitalWrite(_sck, LOW);
    digitalWrite(_nSEL, HIGH);
}

//------------------------------------------------------------------------------------------
byte CSi4463::writeByte(byte out)
{
    byte res = 0;
    for(size_t i=0; i<8; i++ )
    {
        if(out&0x80)
        {
            digitalWrite(_mosi, HIGH);
        }
        else
        {
            digitalWrite(_mosi, LOW);
        }

        digitalWrite(_sck, HIGH);
        int inState = digitalRead(_miso);
        res = (res << 1) | (inState & 0x1);
        MYNOP();
        digitalWrite(_sck, LOW);
        out <<= 1;
    }
    return res;
}

//------------------------------------------------------------------------------------------
bool CSi4463::isSendCommandAlowed()
{
    beginCommunication();
    writeByte(0x44);
    byte r = writeByte(0);
    endCommunication();
    return (r==0xff);
}

//------------------------------------------------------------------------------------------
bool CSi4463::waitForCTS()
{
    for( size_t i=0 ;i<100; i++ )
    {
        if( isSendCommandAlowed() )
        {
            return true;
        }
        delay(10);
    }

    MYDEBUG("NOCTS");
    return false;
}

//------------------------------------------------------------------------------------------
CSi4463::SiResultCode CSi4463::execCommand(size_t numBytesToWrite, const byte* writeData, size_t numBytesToRead, byte* readData)
{
    if( numBytesToWrite == 0 )
    {
        return rcWrongCmd;
    }
    if( !waitForCTS() )
    {
        return rcNoCTS;
    }

    beginCommunication();
    for( size_t i=0; i< numBytesToWrite; i++ )
    {
        writeByte(writeData[i]);
    }
    endCommunication();

    if( numBytesToRead == 0 )
    {
        return rcSuccess;  
    }

    if( !waitForCTS() )
    {
        return rcNoCTS;
    }

    beginCommunication();
    writeByte(0x44);
    writeByte(0); // read dummy CTS
    for( size_t i=0; i< numBytesToRead; i++ )
    {
        readData[i] = writeByte(0);
    }
    endCommunication();


    return rcSuccess;
}

//------------------------------------------------------------------------------------------
CSi4463::SiResultCode CSi4463::clearIntStatus()
{
    byte buf[] = { 0x20, 0,0,0 };    
    SiResultCode rc = execCommand(4, buf, 0, NULL);
    if( rc != rcSuccess )
    {
        return rc;
    }

    return rcSuccess;
}

//------------------------------------------------------------------------------------------
CSi4463::SiResultCode CSi4463::execCmdChain(const byte *data)
{
    while(*data != 0)
    {
        unsigned char len = *data;
        if( len > 16 )
        {
            return rcWrongLength;
        }
        data++;
        byte buf[16];
        memcpy(buf, data, len);
        data += len;

        SiResultCode rc;
        rc = execCommand(len, buf, 0, NULL);
        if( rc != rcSuccess )
        {
            return rc;
        }

        rc = clearIntStatus();
        if( rc != rcSuccess )
        {
            return rc;
        }
    }

    return rcSuccess;
}
