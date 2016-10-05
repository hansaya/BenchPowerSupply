#ifndef powerSensors_h
#define powerSensors_h

#include "Arduino.h"
#include "common.h"

#define normalSV 11.0
#define abnormalSV 13.5

#define normalTV 21
#define abnormalTV 40

#define lowHV 105
#define highHV 240

#define maxP 1500

class powerSensors
{
  private:

    unsigned long previousMillis = 0;
    float amp;
    float mainV;

    //for torroid board ADC
//    int16_t InputA;
//    int16_t InputB;
    int16_t InputC;
    int16_t InputD;

    //for the main board ADC
    int16_t InputE;
    int16_t InputF;
//    int16_t InputG;
    int16_t curRawSensor;

    //for the internal ADC
    int16_t InputI;
    int16_t InputJ;

  
  public:
    powerSensors();
    float getTorroidUpperVoltage();
    float getTorroidLowerVoltage();

    //for the main board sensors
    float getServerCh1UpperVoltage();
    float getServerCh2UpperVoltage();
    float getServerCh1LowerVoltage();
    float getServerCh2LowerVoltage();

    bool checkServerVoltages();
    bool checkToroidVoltages();

    float getMainVoltage(); //sensor board that connected to the main controller

    float getInputAMP(); //coming from the troid adc
    float calculatePower(); //mix of torroid input and main board voltage sensing
    void readSensors(); //read all the sensors
    bool hvHealth();
    bool pwHealth();
    float rmsAmps();
    float getVPP();
    
    #ifdef DEBUG
    void debug(); //for debuging only
    #endif
};

#endif
