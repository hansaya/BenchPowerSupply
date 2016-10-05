#include "toroid.h"

toroid :: toroid(short A, short B, short C, short D, powerSensors* sensors){


  Vsensors = sensors;
  relayA = A; //temperrory turn on to ovoid inrush current
  relayB = B;
  relayC = C;
  relayD = D;
  highVoltage = false;
  pinMode(relayA, INPUT_PULLUP);
  pinMode(relayB, INPUT_PULLUP);
  pinMode(relayC, INPUT_PULLUP);
  pinMode(relayD, INPUT_PULLUP);
  pinMode(relayA, OUTPUT);
  pinMode(relayB, OUTPUT);
  pinMode(relayC, OUTPUT);
  pinMode(relayD, OUTPUT);
  digitalWrite(relayA,HIGH);
  digitalWrite(relayB,HIGH);
  digitalWrite(relayC,HIGH);
  digitalWrite(relayD,HIGH);
}
void toroid :: begin(){
  
}
bool toroid :: turnOn(){
  Vsensors->readSensors();
  if(Vsensors->hvHealth())
    return true;
  checkMainVoltage();
  if(highVoltage){
    //turn the middle on
    digitalWrite(relayB,LOW);
    digitalWrite(relayC,LOW);
    delay(200);
    //turn the temp relay on
    digitalWrite(relayD,LOW);    
    delay(500);
    //turn on power
    digitalWrite(relayA,LOW);
  }
  else{
    //turn the middle on
    digitalWrite(relayB,HIGH);
    digitalWrite(relayC,HIGH);
    delay(200);
    //turn the temp relay on
    digitalWrite(relayD,LOW);
    delay(500);
    //turn on power
    digitalWrite(relayA,LOW);    
  }
  delay(200);
  return health();
}
void toroid :: turnOff(){
  digitalWrite(relayA,HIGH);
  digitalWrite(relayB,HIGH);
  digitalWrite(relayC,HIGH);
  digitalWrite(relayD,HIGH);
}
void toroid :: voltage110v(){
  highVoltage = false;
}
void toroid :: voltage220v(){
  highVoltage = true;
}
void toroid :: checkMainVoltage(){
  if(Vsensors->getMainVoltage() > 150)
    highVoltage = true;
  else if(Vsensors->getMainVoltage() > 105)
    highVoltage = false;
}
bool toroid :: health()
{
  if (Vsensors->checkToroidVoltages()){
    EEPROM.write(1,3); // error type
    EEPROM.write(4,1); // error escilator
    turnOff();
    return true;
  }
  return false;
}

