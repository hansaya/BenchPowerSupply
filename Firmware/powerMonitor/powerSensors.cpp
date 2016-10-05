#include "powerSensors.h"
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 adcTo;
Adafruit_ADS1115 adcM(0x49);

// set variables for AC readings
//const unsigned long sampleTime = 1002000UL;                           
const unsigned long numSamples = 20;                               
//const unsigned long sampleInterval = sampleTime/numSamples;

double offsetI;
double filteredI;
double sqI,sumI;
int16_t sampleI;
double Irms;
extern bool SPPStarted;

powerSensors :: powerSensors(){
	  adcTo.begin();
  	adcM.begin();
    //adcM.setGain(GAIN_ONE);
}

void powerSensors :: readSensors(){

    unsigned long currentMillis = millis();
    
	  //InputA = adcTo.readADC_SingleEnded(0);
  	//InputB = adcTo.readADC_SingleEnded(1);
    InputC = adcTo.readADC_SingleEnded(2);
  	InputD = adcTo.readADC_SingleEnded(3);

  	InputE = adcM.readADC_SingleEnded(0);
  	InputF = adcM.readADC_SingleEnded(1);
  	
  	InputI = analogRead(0);
  	InputJ = analogRead(1);
    amp = rmsAmps();
    mainV = getVPP();
}

float powerSensors :: getTorroidUpperVoltage(){
	return InputC/258.20;
}

float powerSensors :: getTorroidLowerVoltage(){
	return InputD/553.881;
}

float powerSensors :: getInputAMP(){
	//return amp/1391.26;
  return amp;
}

float powerSensors :: getServerCh1UpperVoltage(){
	return InputF/952.906;
}

float powerSensors :: getServerCh2UpperVoltage(){
	return InputE/954.509;
}

float powerSensors :: getServerCh1LowerVoltage(){
	return InputI/64.9161;
}

float powerSensors :: getServerCh2LowerVoltage(){
	return InputJ/64.9161;
}

float powerSensors :: getMainVoltage(){
	return mainV;
}

float powerSensors :: calculatePower(){
  
	return mainV*amp;
}

bool powerSensors :: checkServerVoltages(){
  readSensors();
  if(getServerCh1LowerVoltage() < normalSV)
  {
    EEPROM.write(3,0); //error label
    EEPROM.writeDouble(6,getServerCh1LowerVoltage());
    return true;
  }
  else if(getServerCh2LowerVoltage() < normalSV)
  {
    EEPROM.write(3,1); //error label
    EEPROM.writeDouble(6,getServerCh2LowerVoltage());
    return true;
  }
  else if(getServerCh1UpperVoltage() < (normalSV*2))
  {
    EEPROM.write(3,2); //error label
    EEPROM.writeDouble(6,getServerCh1UpperVoltage());
    return true;
  }
  else if(getServerCh2UpperVoltage() < (normalSV*2))
  {
    EEPROM.write(3,3); //error label
    EEPROM.writeDouble(6,getServerCh2UpperVoltage());
    return true;
  }
  else if(getServerCh1LowerVoltage() > abnormalSV)
  {
    EEPROM.write(3,0); //error label
    EEPROM.writeDouble(6,getServerCh1LowerVoltage());
    return true;
  }
  else if(getServerCh2LowerVoltage() > abnormalSV)
  {
    EEPROM.write(3,1); //error label
    EEPROM.writeDouble(6,getServerCh2LowerVoltage());
    return true;
  }
  else if(getServerCh1UpperVoltage() > (abnormalSV*2))
  {
    EEPROM.write(3,2); //error label
    EEPROM.writeDouble(6,getServerCh1UpperVoltage());
    return true;
  }
  else if(getServerCh2UpperVoltage() > (abnormalSV*2) )
  {
    EEPROM.write(3,3); //error label
    EEPROM.writeDouble(6,getServerCh2UpperVoltage());
	  return true;
  }
  else
    return false;
}
bool powerSensors :: checkToroidVoltages(){
  readSensors();
  if(getTorroidLowerVoltage() < normalTV)
  {
    EEPROM.write(3,4); //error label
    EEPROM.writeDouble(6,getTorroidLowerVoltage());
    return true;
  }
  else if(getTorroidUpperVoltage() < (normalTV*2))
  {
    EEPROM.write(3,5); //error label
    EEPROM.writeDouble(6,getTorroidUpperVoltage());
    return true;
  }
  else if(getTorroidLowerVoltage() > abnormalTV)
  {
    EEPROM.write(3,4); //error label
    EEPROM.writeDouble(6,getTorroidLowerVoltage());
    return true;
  }
  else if(getTorroidUpperVoltage() > (abnormalTV*2))
  {
    EEPROM.write(3,5); //error label
    EEPROM.writeDouble(6,getTorroidUpperVoltage());
    return true;
  }    
  else
    return false;
}
#ifdef DEBUG
void powerSensors :: debug()
{
    //readSensors();
    Serial.print("A = ");Serial.print(getInputAMP());
    Serial.print("  Power = ");Serial.print(calculatePower()); 
    Serial.print("\t UT: "); Serial.print(getTorroidUpperVoltage());
    Serial.print("\t LT: "); Serial.print(getTorroidLowerVoltage());
    Serial.print("\t c1U: "); Serial.print(getServerCh1UpperVoltage());
    Serial.print("\t c1L: "); Serial.print(getServerCh1LowerVoltage());
    Serial.print("\t c2U: "); Serial.print(getServerCh2UpperVoltage());
    Serial.print("\t c2L: "); Serial.print(getServerCh2LowerVoltage());
    Serial.print("\t hv: "); Serial.print(getMainVoltage());
    Serial.println();
}
#endif
bool powerSensors :: hvHealth()
{
   if(getMainVoltage()>highHV || getMainVoltage()<lowHV)
   {
    EEPROM.write(3,6); //error label
    EEPROM.writeDouble(6,getMainVoltage()); //error value
    EEPROM.write(1,1); // error type
    EEPROM.write(4,1); // error indication
    return true;
   }
   else
    return false;
}
bool powerSensors :: pwHealth()
{
   if(calculatePower()>maxP)
   {
    EEPROM.write(3,7); //error label
    EEPROM.writeDouble(6,calculatePower()); //error value
    EEPROM.write(1,5); // error type
    EEPROM.write(4,1); // error indication
    return true;
   }
   else
    return false;
}
float powerSensors :: rmsAmps(){
  float output = 0;
  for (unsigned int n = 0; n < numSamples; n++)
  {
    int16_t acInput = adcTo.readADC_SingleEnded(0)-13213;
//    uint16_t temp = acInput >> 15;     // make a mask of the sign bit
//    acInput ^= temp;                   // toggle the bits if value is negative
//    acInput += temp & 1;
    if(output < acInput){
      output = acInput;
    }
  }
  return (output)/745.0;
}
float powerSensors :: getVPP()
{
  float output = 0;
  for (unsigned int n = 0; n < numSamples; n++)
  {
    int16_t acInput;
    if(SPPStarted)
      acInput = adcM.readADC_SingleEnded(2)-13333;
    else
      acInput = adcM.readADC_SingleEnded(2)-12624;
      
      if(output < acInput){
        output = acInput;
      }
  }
  return output/20.2122;
}
