#ifndef toroid_h
#define toroid_h

#include "Arduino.h"
#include "powerSensors.h"

class toroid
{
  private:
    bool highVoltage;
    short relayA;
    short relayB;
    short relayC;
    short relayD;
    powerSensors* Vsensors;  
  //static uint8_t USI_BytesAvail;
  
  public:
    toroid(short A, short B, short C, short D, powerSensors* sensors);
    void begin();
    bool turnOn();
    void turnOff();
    void voltage110v();
    void voltage220v();
    void checkMainVoltage();
    bool health();
};

#endif
