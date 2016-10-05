#include "lcdController.h"

lcdController :: lcdController(powerSensors* sensors, tempSensors* tempS) : lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE)
{
  sensorsV = sensors;
  sensorsT = tempS;
}
void lcdController :: begin()
{
  lcd.begin(16,2);
  lcd.setBacklight(0);
  lcd.home ();
  lcd.print("********************");
}
void lcdController :: start()
{
  lcd.setBacklight(1);
  lcd.begin(16,2); 
  lcd.home ();                   // go home
  lcd.print("WELCOME");  
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print ("Starting....");
  cleared = false;
}
void lcdController :: printToScreen(String str){
  lcd.clear();
  lcd.home ();
  lcd.print(str);
  cleared = false;
}
void lcdController :: printVoltages(){
  unsigned long currentMillis = millis();
  if(introS){
    if(!introPrinted)
    {
      introPrinted = true;
      printToScreen("Voltages");
      lcd.setCursor ( 0, 1 );
      lcd.print("********************");
    }
    if(currentMillis - mainMenueLabel >= 3000){
      mainMenueLabel = currentMillis;
      introS = false;
      introPrinted = false;
    }
  }
  else{
//    unsigned long currentMillis = millis();
    mainMenueLabel = currentMillis;
    if(showMenue && (currentMillis - lcdMenueLabel >= 2000)){
      lcdMenueLabel = currentMillis;
      showMenue = false;
      cleared = false;    
    }
    
    if(showMenue || (currentMillis - previousMillis >= 5000)) {
      previousMillis = currentMillis;
      if(!showMenue){
        lcd.clear();
        lcdMenueLabel = currentMillis;
      }
      lcd.setCursor ( 0, 0 );
      lcd.print("CH1L"); lcd.setCursor ( 5, 0 );
      lcd.print("CH2L"); lcd.setCursor ( 10, 0 );
      lcd.print("TDL"); lcd.setCursor ( 0, 1 ); 
      lcd.print("CH1H"); lcd.setCursor ( 5, 1 );
      lcd.print("CH2H"); lcd.setCursor ( 10, 1 );
      lcd.print("TDH");
      showMenue = true;
      //cleared = true;
    }
    else{
      lcdClear();
      lcd.setCursor ( 0, 0 );
      lcd.print(sensorsV->getServerCh1LowerVoltage(),1); lcd.setCursor ( 5, 0 );
      lcd.print(sensorsV->getServerCh2LowerVoltage(),1); lcd.setCursor ( 10, 0 );
      lcd.print(sensorsV->getTorroidLowerVoltage(),1); lcd.setCursor ( 0, 1 ); 
      lcd.print(sensorsV->getServerCh1UpperVoltage(),1); lcd.setCursor ( 5, 1 );
      lcd.print(sensorsV->getServerCh2UpperVoltage(),1); lcd.setCursor ( 10, 1 );
      lcd.print(sensorsV->getTorroidUpperVoltage(),1);
    }
  }
}
void lcdController :: printTemps(){
  unsigned long currentMillis = millis();
  if(introS){
    if(!introPrinted)
    {
      introPrinted =true;
      printToScreen("Tempuratures");
      lcd.setCursor ( 0, 1 );
      lcd.print("********************");
    }
    if(currentMillis - mainMenueLabel >= 2000){
      mainMenueLabel = currentMillis;
      introS = false;
      introPrinted = false;
    }
  }
  else{
//    unsigned long currentMillis = millis();
    mainMenueLabel = currentMillis;
    if(currentMillis - lcdMenueLabel >= 2000){
      lcdMenueLabel = currentMillis;
      lcd.clear();
      lcd.setCursor ( 0, 0 );
      lcd.print(sensorsT->DS18B20locations[tempIterator]); lcd.print(": "); lcd.print(sensorsT->data[tempIterator]); lcd.print("C");
      tempIterator++;
      if(sensorsT->noSensors == tempIterator)
        tempIterator = 0;
      else
      {
        lcd.setCursor ( 0, 1 );
        lcd.print(sensorsT->DS18B20locations[tempIterator]); lcd.print(": "); lcd.print(sensorsT->data[tempIterator]); lcd.print("C");
        tempIterator++;
      }
      if(sensorsT->noSensors == tempIterator)
        tempIterator = 0;    
    }
  }
}
void lcdController :: lcdClear()
{
  if(!cleared){
    lcd.clear();
    cleared = true;
  }
}
void lcdController :: backlightOff(){
  lcd.setBacklight(0);
}
void lcdController :: intro(){
  cleared = false;
  introS = true;;
}
void lcdController :: error(){
  unsigned long currentMillis = millis();
  
  if(showMenue && (currentMillis - lcdMenueLabel >= 2000)){
    lcdMenueLabel = currentMillis;
    showMenue = false;
    cleared = false;
//    lcd.setCursor(0, 1);
//    for(int i = 0; i < 16; i ++)
//      lcd.write(*smile);
  }
  
  if(showMenue || (currentMillis - previousMillis >= 5000)) {
    previousMillis = currentMillis;
    if(!showMenue){
      lcd.clear();
      lcdMenueLabel = currentMillis;
    }
    lcd.home ();                   // go home
//    lcd.print("**Catastrophic**");  
//    lcd.setCursor ( 0, 1 );        // go to the next line
    lcd.print("****Failure!****");
    showMenue = true;
//
//    lcd.setCursor(0, 1);
//    for(int i = 0; i < 16; i ++)
//       lcd.write(2);
    //cleared = true;
  }
  else{
    if(EEPROM.read(1)>=NE)
      EEPROM.write(10,0);
    lcdClear();
    lcd.setCursor ( 0, 0 );
    lcd.print(errorMessages[EEPROM.read(1)]); 
    lcd.setCursor ( 0, 1 );
    lcd.print(errorMessagesP2[EEPROM.read(3)]);
    lcd.print(" : ");
    lcd.print(EEPROM.readDouble(6)); 
  }
}
void lcdController :: powerOutput(){
  unsigned long currentMillis = millis();
  if(introS){
    if(!introPrinted)
    {
      introPrinted =true;
      printToScreen("Power input");
      lcd.setCursor ( 0, 1 );
      lcd.print("********************");
    }
    if(currentMillis - mainMenueLabel >= 2000){
      mainMenueLabel = currentMillis;
      introS = false;
      introPrinted =false;
    }
    cleared = false;
  }
  else{
    mainMenueLabel = currentMillis;
    lcdClear();
    lcd.setCursor ( 0, 0 );
    lcd.print("Main In: "); lcd.print(sensorsV->getMainVoltage(),1); lcd.print("V");
    lcd.setCursor ( 0, 1 );
    lcd.print(sensorsV->getInputAMP(),2); lcd.print("A "); lcd.print(sensorsV->calculatePower(),1); lcd.print("W");
  }
}

