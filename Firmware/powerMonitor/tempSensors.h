#ifndef tempSensors_h
#define tempSensors_h

#include "Arduino.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "common.h"

#define TEMPERATURE_PRECISION 10
#define NS 11
#define MAXTEMP 100

class tempSensors
{
  private:   
    OneWire* oneWireMaster;
    void printData(DeviceAddress deviceAddress);
  public:
    tempSensors(DallasTemperature* dallTemp, OneWire* wire);
    void readSensors(); //read all the sensors
    void grabData();
    #ifdef DEBUG
    void printAddress(DeviceAddress deviceAddress);
    void printTemperature(DeviceAddress deviceAddress);
    void printResolution(DeviceAddress deviceAddress);
    uint8_t findDevices();    
    void debug();
    void setAlarms();
    #endif
    bool alarm();
    bool checkTemperature();    
    float highestZone1();
    float highestZone2();
    short noSensors = NS;
    DallasTemperature* sensors;
    void begin();

    double data[NS] = {0.00};
    String DS18B20locations[NS] = {
      "SVR1 ST1",
      "SVR2 ST1",
      "SVR1 ST2",
      "SVR2 ST2",
      "SVR1 HV",      
      "SVR2 HV",
      "TRD TRN",
      "OUT CH1",
      "OUT CH2",
      "OUT CH3",
      "OUT CH4" 
    };
    uint8_t DS18B20sensors[NS][8] = {
      {0x28, 0xFF, 0xD8, 0x9F, 0x63, 0x15, 0x02, 0xBD},
      {0x28, 0xFF, 0x11, 0x96, 0x63, 0x15, 0x02, 0xFF},
      {0x28, 0xFF, 0x0A, 0xE8, 0x63, 0x15, 0x02, 0x4E},
      {0x28, 0xFF, 0xFD, 0xB4, 0x63, 0x15, 0x02, 0x73},
      {0x28, 0xFF, 0x82, 0xA9, 0x63, 0x15, 0x02, 0xD4},     
      {0x28, 0xFF, 0x92, 0xD3, 0x63, 0x15, 0x02, 0x1B},      
      {0x28, 0xFF, 0x56, 0x95, 0x63, 0x15, 0x02, 0xCF},
      {0x28, 0xFF, 0xCA, 0x93, 0x63, 0x15, 0x02, 0x50},
      {0x28, 0xFF, 0xF1, 0xD3, 0x63, 0x15, 0x02, 0x44},
      {0x28, 0xFF, 0x34, 0xB5, 0x63, 0x15, 0x02, 0x2D},
      {0x28, 0xFF, 0x5F, 0x26, 0x6D, 0x14, 0x04, 0xC8}      
      };

};

#endif
