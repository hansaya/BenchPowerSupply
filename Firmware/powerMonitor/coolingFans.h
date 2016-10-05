#ifndef coolingFans_h
#define coolingFans_h

#include "tempSensors.h"
#include "Arduino.h"

class coolingFans
{
  private:   
    tempSensors* tempSen;
    unsigned long previousMillis = 0;
    int counter = 0;
    bool ledGoingHigh = true;
    bool fanzone1 = false;
    bool fanzone2 = false;
    bool spinup = false;

  public:
    coolingFans(tempSensors* sensors);
    void begin();
    void setSpeed(short fan, short speed);
    void debug();
    void setPwmFrequency(int pin, int divisor);
    void regulation();
    int ledBlink();
    void powerLED(bool on);
    bool fade = true;

};

#endif
