#include "tempSensors.h"

tempSensors :: tempSensors(DallasTemperature* dallTemp, OneWire* wire){
  sensors = dallTemp;
  oneWireMaster = wire;
}

void tempSensors :: begin(){
  oneWireMaster->deviceReset();
  #ifdef DEBUG
    setAlarms();
  #endif
  
  for(int i=0; i<NS;i++){
    sensors->setResolution(DS18B20sensors[i], TEMPERATURE_PRECISION);
  }
}

void tempSensors :: readSensors()
{
  sensors->setWaitForConversion(false);  // makes it async
  sensors->requestTemperatures();
  sensors->setWaitForConversion(true);

}

void tempSensors :: grabData()
{
  for(short i=0; i<NS; i++)
  {
    data[i]=sensors->getTempC(DS18B20sensors[i]);
  }

}

#ifdef DEBUG
// function to print a device address
void tempSensors :: printAddress(DeviceAddress deviceAddress)
{
  Serial.print("{");
  for (uint8_t i = 0; i < 8; i++)
  {
    Serial.print("0x");
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
    if(i!=7)
      Serial.print(", ");
  }
  Serial.print("}");
}

// function to print the temperature for a device
void tempSensors :: printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors->getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print("/t");
//  Serial.print(" Temp F: ");
//  Serial.print(DallasTemperature::toFahrenheit(tempC));
}

// function to print a device's resolution
void tempSensors :: printResolution(DeviceAddress deviceAddress)
{
  Serial.print("Resolution: ");
  Serial.print(sensors->getResolution(deviceAddress));
  Serial.println();    
}

// main function to print information about a device
void tempSensors :: printData(DeviceAddress deviceAddress)
{
  Serial.print("Requesting temperatures...");
  sensors->requestTemperatures();
  Serial.print("Device Address: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
  printResolution(deviceAddress);
}

uint8_t tempSensors :: findDevices()
{

  Serial.println("Checking for I2C devices...:");
  if (oneWireMaster->checkPresence())
  {
    Serial.println("DS2482-100 present");
    
    oneWireMaster->deviceReset();
    
    Serial.println("\tChecking for 1-Wire devices...");
    if (oneWireMaster->wireReset())
    {
      Serial.println("\tDevices present on 1-Wire bus");
      
      uint8_t currAddress[8];
      
      Serial.println("\t\tSearching 1-Wire bus...");
      
      while (oneWireMaster->wireSearch(currAddress))
      {
        Serial.print("\t\t\tFound device: ");
        printData(currAddress);
        Serial.println();
      }
      
      oneWireMaster->wireResetSearch();
      
    }
    else
      Serial.println("\tNo devices on 1-Wire bus");
  }
  else
    Serial.println("No DS2482-100 present");

  return sensors->getDeviceCount();
}

void tempSensors :: debug(){
  Serial.print(sensors->getDeviceCount(), DEC);
  Serial.println(" devices.");
  Serial.print("Parasite power is: "); 
  if (sensors->isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  //delay(750/ (1 << (12-TEMPERATURE_PRECISION)));
  for(int i=0; i<NS;i++){
    printTemperature(DS18B20sensors[i]);
    Serial.print(" alarm high: ");
    Serial.println(sensors->getHighAlarmTemp(DS18B20sensors[i]), DEC);
//    Serial.println(" Resolution: ");
//    Serial.println(sensors->getResolution(DS18B20sensors[i]));
  }
  if(alarm())
    Serial.println("DANGER! DANGER!");
  else
    Serial.println("");
}
void tempSensors :: setAlarms(){
  for(int i=0; i<NS;i++){
    sensors->setHighAlarmTemp(DS18B20sensors[i],MAXTEMP);    
  }
  Serial.println("Alarms set");
}
#endif
bool tempSensors :: alarm(){
  if(sensors->hasAlarm()){
    EEPROM.write(3,7); //error label
    EEPROM.writeDouble(6,0.0); //error value
    EEPROM.write(1,2); // error type
    EEPROM.write(4,1); // error escilation
    return true;
  }
  return false;
}
bool tempSensors :: checkTemperature(){

}
//change this ****
float tempSensors :: highestZone1(){
  float high = 0; 
  for(int i=0; i<6;i++){
    high = (data[i] > high)? data[i]:high;
  }
  return high;
}
float tempSensors :: highestZone2(){
  float high = 0; 
  for(int i=6; i<NS;i++){
    high = (data[i] > high)? data[i]:high;
  }
  return high;
}


