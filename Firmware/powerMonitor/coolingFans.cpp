#include "coolingFans.h"

#define fan1Pin 5
#define fan2Pin 6
#define fan3Pin 9
#define fan4Pin 10

#define MINZ1 45
#define MINZ2 38
#define MAX 100

coolingFans :: coolingFans(tempSensors* sensors){
  tempSen = sensors;
  pinMode(fan1Pin, OUTPUT);
  pinMode(fan3Pin, OUTPUT);
  pinMode(fan4Pin, OUTPUT);
}
void coolingFans :: begin(){
  digitalWrite(fan3Pin,HIGH);
  digitalWrite(fan4Pin,HIGH);
  delay(500);
  digitalWrite(fan3Pin,LOW);
  digitalWrite(fan4Pin,LOW);
  setPwmFrequency(10, 1);
}
void coolingFans :: setSpeed(short fan, short speed){
  switch(fan){
//    case 1:
//      analogWrite(fan1Pin, speed);
//      break;
//    case 2:
//      analogWrite(fan2Pin, speed);
//      break;
    case 3:
      analogWrite(fan3Pin, speed);
      if(speed>65 && !spinup)
      {
        spinup = true;
        fanzone1 = false;
      }
      else if(speed<60 && spinup)
        spinup = false;
      break;
    case 4:
      analogWrite(fan4Pin, speed);
      break;
  }
}
void coolingFans :: debug(){
  analogWrite(fan3Pin, 25);
  analogWrite(fan4Pin, 0);
}
void coolingFans :: regulation(){
  //therer are two zones. one for main two power supplys
  //the other one is for switching regulators
  if(tempSen->highestZone1()>MINZ1)
  {
    if(!fanzone1)
    {
      fanzone1=true;
      digitalWrite(fan3Pin,HIGH);
      delay(500);
    }
    setSpeed(3,map(tempSen->highestZone1(), MINZ1, MAX, 20, 255)); //zone 1
  }
  else if(tempSen->highestZone1()<(MINZ1-5))
  {
      digitalWrite(fan3Pin,LOW);
      fanzone1 = false;
  }
  
  if(tempSen->highestZone2()>MINZ2)
  {
    if(!fanzone2)
    {
      fanzone2=true;
      digitalWrite(fan4Pin,HIGH);
      delay(500);
    }
    setSpeed(4,map(tempSen->highestZone2(), MINZ2, MAX, 20, 255)); //zone 2
  }
  else if(tempSen->highestZone2()<(MINZ2-5))
  {
      digitalWrite(fan4Pin,LOW);
      fanzone2 = false;
  }
}
int coolingFans :: ledBlink(){
  unsigned long currentMillis = millis();
  if(fade){
    if(currentMillis - previousMillis >= 10) {
      previousMillis = currentMillis;
      if(ledGoingHigh){
        ++counter;
        if(counter == 255){
          //counter = 5;
          ledGoingHigh = false;
          }
      }
      else{
        --counter;      
        if(counter == 0)
          ledGoingHigh = true;    
      }
      analogWrite(fan1Pin,counter);
    }
  }
  else{
    if(currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;
      if(ledGoingHigh){
        ledGoingHigh = false;
        digitalWrite(fan1Pin,LOW);
      }
      else{
        ledGoingHigh = true;
        digitalWrite(fan1Pin,HIGH);
      }
    }
  }
  return counter;
}
void coolingFans :: powerLED(bool on){
  if(on)
    digitalWrite(fan1Pin,HIGH);
  else
    digitalWrite(fan1Pin,LOW);
}
void coolingFans :: setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
