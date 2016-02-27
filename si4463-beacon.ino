

#include "morse/morse.h"
#include "si4463/si4463.h"


const int nSEL = 7; // out
const int siMOSI = 6; //out
const int siMISO = 5; //in
const int siSCK = 4; // out

const int pinOOK = 8; // out

const int pinLED = 13; // out

bool prevTransmittingActive = false;

CMorse morse;
CSi4463 transmitter(nSEL, siMOSI, siMISO, siSCK, pinOOK);

//------------------------------------------
void setup() {
  pinMode(nSEL, OUTPUT);
  pinMode(siMOSI, OUTPUT);
  pinMode(siMISO, INPUT);
  pinMode(siSCK, OUTPUT);

  pinMode(pinOOK, OUTPUT);
  pinMode(pinLED, OUTPUT);
  
  Serial.begin(9600);

  morse.setText("VVV SOME TEXT IN UPPERCASE"); // <---- place your text here

  transmitter.setDebugSerial(&Serial);
  transmitter.initialize();
  morse.start();
}




//------------------------------------------
void loop() {

  // TODO: Remove. Research purpose only.
  if (Serial.available()) {
    int inByte = Serial.read();
    if( inByte == 'o' )
    {
      Serial.println(F("do si initialize"));
      transmitter.initialize();
    }
    else if( inByte == 't' )
    {
      Serial.println(F("do tx"));
      transmitter.activateTX();
    }
    else if( inByte == 'r' )
    {
      Serial.println(F("do ready"));
      transmitter.activateRX();
    }
    else if( inByte == '1' )
    {
      Serial.println(F("o1"));
      digitalWrite(pinOOK, HIGH);
    }
    else if( inByte == '0' )
    {
      Serial.println(F("o0"));
      digitalWrite(pinOOK, LOW);
    }
    else if( inByte == '2' )
    {
      Serial.println(F("morse start"));
      morse.start();
    }
    else if( inByte == '3' )
    {
      Serial.println(F("morse stop"));
      morse.stop();
    }
    else
    {
      Serial.println(F("wrong command"));
    }
    
  }


  // We give CPU time to Morse subsystem.
  morse.handleTimeout();

  // Send commands to si4463 module based on morse state.
  
  bool txActive = morse.isTransmittingActive();

  // detect state changing
  if( prevTransmittingActive != txActive )
  {
    if( txActive )
    {
      transmitter.activateTX();
      prevTransmittingActive = true;
    }
    else
    {
      transmitter.activateRX();
      transmitter.controlOOK( false );
      prevTransmittingActive = false;
    }
  }

  if( txActive )
  {
    // if transmitting is active we handle CW on/off
    bool toneOn = morse.isToneActive();
    transmitter.controlOOK( toneOn );

    // LED duplicate
    digitalWrite(pinLED, (toneOn)? HIGH : LOW );
  }
  
  
  

}

