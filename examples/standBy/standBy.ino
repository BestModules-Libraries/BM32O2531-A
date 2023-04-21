 /*************************************************
File:             standBy.ino
Description:      Set the module to standby mode after 10s.
Note:               
**************************************************/
#include "BM32O2531-A.h"

BM32O2531_A LCD(&Serial1);//HardwareSerial of BMduino BMCOM1

uint32_t timer_count1=0;
uint8_t timer_count2=10;
uint8_t timer_running=1;
uint8_t timer_trigger=1;
uint8_t timer_initila=1;
uint8_t module_EID=6;
void setup() {
  Serial.begin(115200);
  LCD.begin();
  LCD.reset(module_EID);
  LCD.clearScreen(module_EID);
  
}
void loop() {
  if((timer_running&&millis()-timer_count1>=1000)||timer_trigger)
  {
    timer_count1=millis();
    timer_trigger=0;
    if(timer_initila)
    {
      timer_initila=0;
      uint16_t re_ver=LCD.getFWVer(module_EID); 
      LCD.showString(module_EID,0,0,"Name: BM32O2531-A",FONT_5x7);
      LCD.showString(module_EID,1,0,"Ver:",FONT_5x7);
      LCD.showNum(module_EID,1,24,(re_ver>>8),FONT_5x7);      
      LCD.showString(module_EID,1,36,".",FONT_5x7);
      LCD.showNum(module_EID,1,42,(re_ver&0x00ff),FONT_5x7);
    }
    LCD.showString(module_EID,3,0,"The module will go into sleep after 10 s ",FONT_5x7);
    if(timer_count2==0){
      timer_running=0;  
      LCD.standby();
      Serial.println("standby");
      delay(2000);
      LCD.wakeUp();
      Serial.println("wakeup");
      LCD.setScreenON(module_EID);
    }
    timer_count2--;
  }
 
}