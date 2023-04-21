/*************************************************
File:             setEID.ino
Description:      EID setting of modules.
Note:               
**************************************************/
#include "BM32O2531-A.h"
BM32O2531_A LCD(&Serial1);//HardwareSerial of BMduino BMCOM1
uint8_t module_EID=6;
uint16_t FWversion = 0;
void setup() {
   LCD.begin();
  Serial.begin(115200);
  LCD.setEID(module_EID);
  delay(2000);
  
}



void loop() {
  // put your main code here, to run repeatedly:
    FWversion=LCD.getFWVer(module_EID);// get FW version of the module  whose id is 0x06
    Serial.print("FW version is V");
    Serial.println(FWversion,HEX);
}
