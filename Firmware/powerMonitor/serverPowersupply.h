#ifndef serverPowersupply_h
#define serverPowersupply_h

#include "Arduino.h"
#include "powerSensors.h"

#define serverPowerOnRelay 11
#define mainRelay 6

class serverPowersupply
{
  private:
  	powerSensors* Vsensors;   
  
  public:
    serverPowersupply(powerSensors* sensors);
    bool turnOn();
    void turnOff();
    bool health();
};

#endif
