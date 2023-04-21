/******************************************************************
File:             BM12O2531_A.h
Author:           BESTMODULES
Description:      Define classes and required variables
History：         
V1.0.1-- initial version；2022-11-23；Arduino IDE : v1.8.13
******************************************************************/
#ifndef _BM32O2531_A_H_
#define _BM32O2531_A_H_

#include "Arduino.h"
#include <SoftwareSerial.h>


#define BM32O2531_NO_CARE_EID 0xFF
#define BM32O2531_A_SYNC 0x00
#define BM32O2531_A_TLEN 0x02
#define BM32O2531_A_MID 0x23
#define BM32O2531_A_EID 8

#define BM32O2531_A_NO_ACK 0x00 
#define BM32O2531_A_ACK 0x01 
#define BM32O2531_A_SYS_ACK 0x02 

#define BM32O2531_A_SYS_CMD_RESET 0x00 
#define BM32O2531_A_SYS_CMD_STANDBY 0x01 
#define BM32O2531_A_SYS_CMD_WAKEUP 0x03 
#define BM32O2531_A_SYS_CMD_SET_ID 0x80 

#define BM32O2531_A_CMD_ACTION 0x02 
#define BM32O2531_A_CMD_syncAction  0x20 
#define BM32O2531_A_CMD_RESET 0x00 

#define BM32O2531_A_CMD_GET_FW_VER 0x03 
#define BM32O2531_A_CMD_5X7_CHAR 0x09 //Displays 5×7 characters 0x09 displays 5×7 characters as ASCII encoding
#define BM32O2531_A_CMD_8X16_CHAR 0x0A //Displays 8×16 characters 0x0A 8×16 characters in ASCII encoding
#define BM32O2531_A_CMD_5X7_STRING 0x0B //Display 5×7 strings 0x0B Displays 5×7 strings in ASCII encoding
#define BM32O2531_A_CMD_8X16_STRING 0x0C //Display 8×16 strings 0x0C 8×16 strings in ASCII encoding
#define BM32O2531_A_CMD_CLEAR 0x10 //Clear Screen 0x10 Clear Screen
#define BM32O2531_A_CMD_DISPLAY_ON 0x11 //The LCD display is on 0x11 lights up the LCD display
#define BM32O2531_A_CMD_DISPLAY_OFF 0x12 //LCD display off 0x12 Turn off the LCD display

#define BAUD_115200 115200
#define FONT_5x7 0
#define FONT_8x16 1
//#define FONT_16x16 2
#define BM32O2531_A_DISPLAY_ON 1
#define BM32O2531_A_DISPLAY_OFF 0


#define CHECK_OK        0
#define CHECK_ERROR     1
#define TIMEOUT_ERROR   2

#define MAX 10


class BM32O2531_A{
  public:
    BM32O2531_A(HardwareSerial *theSerial  = &Serial);
    void begin(uint32_t band = BAUD_115200);
    bool reset(uint8_t EID);
    void reset();
    void standby(void);  
    void wakeUp(void);
    void setEID(uint8_t EID);
    void syncAction(void);
    uint16_t getFWVer(uint8_t EID);
    bool clearScreen(uint8_t EID,uint8_t syncAction =0);      	
    bool setScreenON(uint8_t EID,uint8_t  syncAction =0);
    bool setScreenOFF(uint8_t EID,uint8_t  syncAction =0);
  	//row xCoordinate
    bool showMultipleChars(uint8_t EID,uint8_t in_ascii[][3],uint8_t length,uint8_t font,uint8_t syncAction =0);
    bool showChar(uint8_t EID,uint8_t  row,uint8_t  xCoordinate , char asciiChar ,uint8_t font,uint8_t  syncAction =0);
    bool showString(uint8_t EID,uint8_t row,uint8_t  xCoordinate ,char in_ascii[],uint8_t font,uint8_t  syncAction =0);
    bool showNum(uint8_t EID,uint8_t  row,uint8_t  xCoordinate , int num , uint8_t font,uint8_t  syncAction =0);
     
 	
  private:
	uint8_t user_EID;
    HardwareSerial *_serial = NULL;
    SoftwareSerial *_softSerial = NULL ;
    uint8_t recBuf[MAX] = {0};
	  bool showFloatNum(uint8_t EID,uint8_t  row,uint8_t  xCoordinate , float num,uint8_t numLen,uint8_t pointNumLen, uint8_t font,uint8_t  syncAction =0);
  	bool sendData(uint8_t EID,uint8_t data[],uint8_t len,uint8_t ack=BM32O2531_A_ACK);
  	bool writeSingleRowCharacter(uint8_t EID,uint8_t row,uint8_t col,uint8_t in_ascii[],uint8_t size,uint8_t font,uint8_t syncAction =0);
  	bool readData(uint8_t EID,uint8_t read_len);
  	uint8_t readBytes(uint8_t recBuf[],uint8_t rlen,uint8_t waitTime = 100);
    void writeBtyes(uint16_t writeData[], uint8_t len);

};


#endif
