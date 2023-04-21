/*************************************************
File:             displayLCD.ino
Description:      Display ASCII characters and numbers in the size of 5x7 characters on the LCD screen.
Note:               
**************************************************/
#include "BM32O2531-A.h"
BM32O2531_A LCD(&Serial1);//HardwareSerial of BMduino BMCOM1
uint8_t module_EID=6;
uint8_t displayCount = 1;
char cdata[100];
String sdata;
void setup() 
{
  Serial.begin(115200);
  LCD.begin();
  LCD.clearScreen(module_EID);
  sdata="Name:BM32O2531-A";
  sdata.toCharArray(cdata,17);
  LCD.showString(module_EID,1,12,cdata,FONT_5x7);
  sdata="Data:2022/06/15";
  sdata.toCharArray(cdata,16);
  LCD.showString(module_EID,2,12,cdata,FONT_5x7);
  uint16_t re_ver=LCD.getFWVer(module_EID);
  sdata="Ver:";
  sdata.toCharArray(cdata,4);
  LCD.showString(module_EID,3,12,cdata,FONT_5x7);
  LCD.showNum(module_EID,3,36,(re_ver>>8),FONT_5x7);
  sdata=".";
  sdata.toCharArray(cdata,1);
  LCD.showString(module_EID,3,42,cdata,FONT_5x7);
  LCD.showNum(module_EID,3,48,(re_ver&0x00ff),FONT_5x7);
  sdata="LCD size:128*64";
  sdata.toCharArray(cdata,16);
  LCD.showString(module_EID,4,12,cdata,FONT_5x7);
}

void loop() 
{
  for(uint8_t i=0;i<21;i++)
  {
    LCD.showChar(module_EID,7,i*6,'.',FONT_5x7);
    delay(100);
  }
  for(uint8_t i=0;i<21;i++)
  {
    LCD.showChar(module_EID,7,i*6,' ',FONT_5x7);
  }

}
