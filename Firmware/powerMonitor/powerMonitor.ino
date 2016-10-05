#include "toroid.h"
#include "powerSensors.h"
#include "tempSensors.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "lcdController.h"
#include "serverPowersupply.h"
#include "coolingFans.h"
#include <avr/sleep.h>
#include <avr/power.h>
#include "common.h"

#define MENADDR 0
#define MENUE 3
#define wakePin 4
#define up 2
#define down 3
bool SPPStarted = false;
bool onIdle = false;
bool sleeping = false;
bool error = false;
bool poweredOff = false;
bool firstStart = true;

OneWire oneWire;
DallasTemperature sensors(&oneWire);
powerSensors pSen;
tempSensors tempSen(&sensors, &oneWire);
toroid toriodT(7,8,12,13,&pSen);
serverPowersupply SPSupply(&pSen);
lcdController lcdD(&pSen, &tempSen);
coolingFans fans(&tempSen);

unsigned long previousMillis = 0; 
unsigned long fanMillis = 0;
//unsigned long last_interrupt_time = 0;

void turnOff(){
  toriodT.turnOff();
  delay(200);
  SPSupply.turnOff();
  SPPStarted = false;
}

void setup() {
  toriodT.begin();
  pinMode(wakePin, INPUT);  
  lcdD.begin();
  Serial.begin(9600);
  
  sensors.begin();  
  //startupPowersupply();
  //fans.begin();
  tempSen.begin();
  //checkInput();
  attachInterrupt(0, pressedUp, RISING);
  attachInterrupt(1, pressedDown, RISING);
}

void loop() {
  checkInput();
  checkForErrors();
  if(onIdle || error)
  {
    fans.ledBlink();
    if(error)
    {
      lcdD.error();
      pSen.readSensors();
      if(!poweredOff)
      {
        fans.fade = false;
        poweredOff = true;
        turnOff();
      }
      #ifdef DEBUG
      pSen.debug();
      #endif
    }
  }
  else
  {
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= 2000) {
      previousMillis = currentMillis;   
      #ifdef DEBUG
        //pSen.debug();
//        tempSen.debug();
//        Serial.println();
//        Serial.print("Menue number: ");
//        Serial.println(EEPROM.read(MENADDR));
        //tempSen.findDevices();
      #endif
      tempSen.grabData();
      fans.regulation();
      pSen.readSensors();
      checkHealth();
      tempSen.readSensors();      
    }
    if(currentMillis - fanMillis >= 500) {
       fanMillis = currentMillis; 
       switch(EEPROM.read(MENADDR)){
        case 0:
          lcdD.printVoltages();
          break;
        case 1:
          lcdD.printTemps();
          break;
        case 2:
          lcdD.powerOutput();
          break;
        default:
          EEPROM.write(MENADDR,0);
          break;  
      }
    }
  }
}
void startupPowersupply(){
  fans.powerLED(true);
  poweredOff = false;
  lcdD.start();
  lcdD.printToScreen("Self Check.....");
  pSen.readSensors();
  tempSen.readSensors();  
  
  #ifdef LIVE
  if(!SPSupply.turnOn())
  {
    SPPStarted = true;
    delay(500);
    toriodT.turnOn();
    fans.begin();
  }
  #endif    
}
void checkInput(){
  if(digitalRead(wakePin)){
    if(!sleeping){
      if(!firstStart){
        lcdD.printToScreen("Turning off bye bye..");
        delay(500);
      }
      firstStart = false;
      sleeping =true;
      putToIdle();
      //sleepNow();
    }
  }
  else if(firstStart || sleeping){
    sleeping = false;
    firstStart = false;
    wakeUpNow();
  }
}
void wakeUpNow() {
  // execute code here after wake-up before returning to the loop() function  
  // timers and code using timers (serial.print and more...) will not work here.  
  // we don't really need to execute any special functions here, since we  
  // just want the thing to wake up  
  if(onIdle){
    #ifdef IDLEON
      //ADCSRA = 1;
      power_adc_enable(); // ADC converter
      power_spi_enable(); // SPI
      power_usart0_enable(); // Serial (USART)
      power_timer0_enable(); // Timer 0
      power_timer1_enable(); // Timer 1
      power_timer2_enable(); // Timer 2
      power_twi_enable(); // TWI (I2C)
    #endif
    onIdle =  false;
  }
  startupPowersupply();
} 

void putToIdle(){
  onIdle =  true;
  turnOff();
  lcdD.lcdClear();
  lcdD.backlightOff();
  fans.powerLED(false);
  fans.fade = true;
  error = false;
  #ifdef IDLEON
    //ADCSRA = 0;
    power_adc_disable(); // ADC converter
    power_spi_disable(); // SPI
    power_usart0_disable();// Serial (USART) 
    //power_timer0_disable();// Timer 0
    power_timer1_disable();// Timer 1
    //power_timer2_disable();// Timer 2
    power_twi_disable(); // TWI (I2C)
  #endif  
}
void pressedUp(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 300ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 500) 
  {
    short mn = EEPROM.read(MENADDR);
    if(MENUE == mn+1)
      mn = 0;
    else
      mn++;
    EEPROM.write(MENADDR,mn);
    lcdD.intro();
  }
  last_interrupt_time = interrupt_time;
}
void pressedDown(){

  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 300ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 500) 
  {
    short mn = EEPROM.read(MENADDR);
    if(0 > mn-1)
      mn = MENUE-1;
    else
      mn--;
    EEPROM.write(MENADDR,mn);
    lcdD.intro();
  }
  last_interrupt_time = interrupt_time;
}
void checkHealth()
{
  #ifdef HEALTHCHECK
  if(tempSen.alarm() || SPSupply.health() || toriodT.health() || pSen.pwHealth())
  {
    SPPStarted = false;
    error = true;
  }
  #endif
}
void checkForErrors()
{
  if(EEPROM.read(4)==1)
  {
    EEPROM.write(4,0);
    #ifdef HEALTHCHECK
    error = true;
    #endif
  }
}

