#include "serverPowersupply.h"

serverPowersupply :: serverPowersupply(powerSensors* sensors)
{
	Vsensors = sensors;
//	pinMode(serverPowerOnRelay, INPUT_PULLUP);
//  pinMode(mainRelay, INPUT_PULLUP);
  
	pinMode(serverPowerOnRelay, OUTPUT);
  pinMode(mainRelay, OUTPUT);
//  pinMode(mainRelay, LOW);
}
bool serverPowersupply :: turnOn()
{
  Vsensors->readSensors();
  if(Vsensors->hvHealth())
    return true;
	digitalWrite(mainRelay,HIGH);
  delay(1500);  
  digitalWrite(serverPowerOnRelay,HIGH);
  //delay(100);	
	return health();

}
void serverPowersupply :: turnOff()
{
  digitalWrite(serverPowerOnRelay,LOW);
  delay(250);
  digitalWrite(mainRelay,LOW);	 
}
bool serverPowersupply :: health()
{
  if (Vsensors->checkServerVoltages())
  {
    EEPROM.write(1,4); // error type
    EEPROM.write(4,1); // error indication
    return true;
  }
  return false;
}
