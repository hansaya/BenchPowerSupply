#ifndef lcdController_h
#define lcdController_h

#include "Arduino.h"
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include "serverPowersupply.h"
#include "tempSensors.h"

#define NE 6

class lcdController
{
  private:   
    powerSensors* sensorsV;
    tempSensors* sensorsT;
    bool cleared;
    unsigned long previousMillis = 0;
    unsigned long lcdMenueLabel = 0;
    unsigned long mainMenueLabel = 0;
    bool showMenue = true;
    short tempIterator = 0;
    bool introS = true;
    bool introPrinted = false;

    String errorMessages[NE] = {
      "Unknown",
      "High Voltage",
      "High Temps",
      "TD Voltage",
      "SP Voltage",
      "Over Load"      
    };    
    String errorMessagesP2[9] = {
      "CH1L",
      "CH2L",
      "CH1H",
      "CH2H",
      "TDL",
      "TDH",
      "HV",
      "PW",
      "N/A"      
    };
//    byte smile[8] = 
//              {
//                0b00000,
//                0b00000,
//                0b01010,
//                0b00000,
//                0b10001,
//                0b01110,
//                0b00000,
//                0b00000
//              };
// 
//    byte cry[8] = 
//              {
//                0b00000,
//                0b00000,
//                0b01010,
//                0b01010,
//                0b00000,
//                0b00000,
//                0b01110,
//                0b10001
//              };       
  
  public:
    lcdController(powerSensors* sensors, tempSensors* tempS);
    void begin();
    void start();
    void printToScreen(String);
    void printVoltages();
    void printTemps();
    void lcdClear();
    void backlightOff();
    void intro();
    void error();
    void powerOutput();
        
    LiquidCrystal_I2C lcd;

};

#endif
