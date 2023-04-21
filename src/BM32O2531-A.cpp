/******************************************************************
File:             BM12O2531_A.cpp
Author:           BESTMODULES
Description:      Define classes and required variables
History：         
V1.0.1-- initial version；2022-11-23；Arduino IDE : v1.8.13
******************************************************************/
#include "BM32O2531-A.h"

/**********************************************************
Description: Constructor
Parameters:   
        *theSerial : Wire object if your board has more than one UART interface       
Return:          
Others:     
**********************************************************/
BM32O2531_A::BM32O2531_A(HardwareSerial *theSerial)
{
     _softSerial = NULL;
     _serial = theSerial;
}


/**********************************************************
Description: Module Initial
Parameters:       
      band : Set the Module  band(Unique Value 115200)        
Return:          
Others:       
**********************************************************/
void BM32O2531_A::begin(uint32_t band)
{
  _serial->begin(band,SERIAL_9N1);//The module baud rate is 115200 and cannot be changed
	
	_serial->SetTxStatus(DISABLE);
	_serial->SetRxStatus(ENABLE);
}
/**********************************************************
Description: The module is required to reset. The module is in initial status after reset: the LCD module is clear, and the backlight is turned off and then turned on again.
Parameters:               
Return: Communication status（true:success ,false:fail） 
Others:   
**********************************************************/
bool BM32O2531_A::reset(uint8_t EID)
{
  uint8_t Cmd[] = {BM32O2531_A_MID,(uint8_t)((BM32O2531_A_TLEN<<4)|EID),BM32O2531_A_CMD_RESET};
  bool result = sendData(EID,Cmd,3);
  delay(100);
  return result;
}
/**********************************************************
Description: The module is required to reset to broadcast (uid=0) without responding to ACK
The module is in initial status after reset: the LCD module is clear, and the backlight is turned off and then turned on again.
Parameters:              
Return:  
Others:   
**********************************************************/
void BM32O2531_A::reset(void)
{
  uint8_t Cmd[] = {BM32O2531_A_SYNC,(BM32O2531_A_TLEN<<4),BM32O2531_A_SYS_CMD_RESET};
  sendData(BM32O2531_NO_CARE_EID,Cmd,3,BM32O2531_A_NO_ACK);
  delay(100);
}
/**********************************************************
Description: Broadcast command, requiring the module to enter standby without responding to ACK
Parameters:               
Return: 
Others:   
**********************************************************/
void BM32O2531_A::standby(void)
{
  uint8_t Cmd[] = {BM32O2531_A_SYNC,(BM32O2531_A_TLEN<<4),BM32O2531_A_SYS_CMD_STANDBY};
  sendData(BM32O2531_NO_CARE_EID,Cmd,3,BM32O2531_A_NO_ACK);
}
/**********************************************************
Description: Broadcast command, wake-up module
Parameters:             
Return:  
Others:   
**********************************************************/
void BM32O2531_A::wakeUp(void)
{
  uint8_t Cmd[3] = {BM32O2531_A_SYNC,(BM32O2531_A_TLEN<<4),BM32O2531_A_SYS_CMD_WAKEUP};
  sendData(BM32O2531_NO_CARE_EID,Cmd,3,BM32O2531_A_NO_ACK);
}
/**********************************************************
Description:  Set the Eid of the module.
Parameters:	 
       EID :  module ID         
Return:	   
Others:   
**********************************************************/
void BM32O2531_A::setEID(uint8_t EID)
{
	user_EID = EID&0x0f;
	uint8_t Cmd[] = {BM32O2531_A_SYNC,(BM32O2531_A_TLEN<<4),(uint8_t)(BM32O2531_A_SYS_CMD_SET_ID|user_EID)};
	sendData(BM32O2531_NO_CARE_EID,Cmd,3);
}

/**********************************************************
Description: Broadcast command, which requires the module to execute the received "suspend execution" command without responding to the ACK
Parameters:             
Return: 
Others:   
**********************************************************/
void BM32O2531_A::syncAction(void)
{
	uint8_t Cmd[] = {BM32O2531_A_SYNC,(BM32O2531_A_TLEN<<4),BM32O2531_A_CMD_ACTION};
	sendData(BM32O2531_NO_CARE_EID,Cmd,3,BM32O2531_A_NO_ACK);
}

/**********************************************************
Description: Retrieve module firmware version information
Parameters:  EID :  module ID                
Return: The format of the returned version number is: xx.xx
Others:   
**********************************************************/
uint16_t BM32O2531_A::getFWVer(uint8_t EID)
{
  uint8_t Cmd[] = {BM32O2531_A_MID,(uint8_t)((BM32O2531_A_TLEN<<4)|EID),BM32O2531_A_CMD_GET_FW_VER};
  bool result = sendData(EID,Cmd,3);

  uint16_t FWVer=0;
  if(result)
  {
    FWVer=recBuf[4]<<8|recBuf[3];
  }
  return FWVer;
}

/**********************************************************
Description: Clear all contents of the screen
Parameters: EID :  module ID   
            syncAction :Delay execution? 0:no ,1:yes        
Return: Communication status（true:success ,false:fail） 
Others:   
**********************************************************/
bool BM32O2531_A::clearScreen(uint8_t EID,uint8_t syncAction )
{
	uint8_t Cmd[] = {BM32O2531_A_MID,(uint8_t)((BM32O2531_A_TLEN<<4)|EID),BM32O2531_A_CMD_CLEAR,0};
	if(syncAction ==BM32O2531_A_CMD_syncAction )
	Cmd[2]|=BM32O2531_A_CMD_syncAction ;
	bool result = sendData(EID,Cmd,3);
	delay(60);//LCD clear screen time
	return result;
}
/**********************************************************
Description: Turn the display on
Parameters:      
      EID: module EID
			syncAction :Delay execution? 0:no ,1:yes        
Return: Communication status（true:success ,false:fail） 
Others:   
**********************************************************/
bool BM32O2531_A::setScreenON(uint8_t EID,uint8_t syncAction )
{
	uint8_t Cmd[4] = {BM32O2531_A_MID,(uint8_t)((BM32O2531_A_TLEN<<4)|EID),0,0};
	Cmd[2]=BM32O2531_A_CMD_DISPLAY_ON;
	if(syncAction ==BM32O2531_A_CMD_syncAction )
	Cmd[2]|=BM32O2531_A_CMD_syncAction ;
	bool result = sendData(EID,Cmd,3);
	return result;
  
}
/**********************************************************
Description: Turn the display off
Parameters:      
      EID: module EID
     syncAction :Delay execution? 0:no ,1:yes        
Return: Communication status（true:success ,false:fail） 
Others:   
**********************************************************/
bool BM32O2531_A::setScreenOFF(uint8_t EID,uint8_t syncAction )
{
  uint8_t Cmd[] = {BM32O2531_A_MID,(uint8_t)((BM32O2531_A_TLEN<<4)|EID),0,0};
  Cmd[2]=BM32O2531_A_CMD_DISPLAY_OFF;
  if(syncAction ==BM32O2531_A_CMD_syncAction )
  {
    Cmd[2]|=BM32O2531_A_CMD_syncAction ;
  }
  bool result = sendData(EID,Cmd,3);
  return result;
  
}
/**********************************************************
Description: Batch write ASCII characters with different positions (rows and columns)
Parameters: 
  EID: module EID
  in_ascii: row, column and ASCII code array of characters.
  For example, the input parameter is 
  uint8_t in_ascii[][3] = {
  {0,0,'a'},
  {1,3,'b'},
  {2,5,'c'},
  {3,8,'d'},
  }; 
  length：in_sizeof(in_ascii)
  font: font size of the character to be displayed
  5x7 characters: font_5x7
  8x16 characters: font_8x16
  syncAction :Delay execution? 0:no ,1:yes        
Return: Communication status（true:success ,false:fail） 
Others:   
**********************************************************/
bool BM32O2531_A::showMultipleChars(uint8_t EID,uint8_t in_ascii[][3],uint8_t length,uint8_t font,uint8_t  syncAction)
{
  uint8_t Instr_name=0;
    //Serial.print(size,DEC);
  if(font==FONT_5x7)
  {
    Instr_name=BM32O2531_A_CMD_5X7_CHAR;//
  }
  else if(font==FONT_8x16)
  {
    Instr_name=BM32O2531_A_CMD_8X16_CHAR;//
  }
  else
  {
    return false;
  }
  if(syncAction ==BM32O2531_A_CMD_syncAction )
  Instr_name|=BM32O2531_A_CMD_syncAction ;
  uint8_t sendbuff[256]={0};
  
  sendbuff[0]=BM32O2531_A_MID;
  

  uint8_t Instr_extend=0;
  uint8_t Instr_index=2;
  uint8_t param_size_index=3;
  uint8_t len=3;

  uint8_t param_size=1+length;
  //Single step Param includes Param0 (number of characters to be displayed)+ParamX (row, col, char)
  //When Param is greater than 13, extended instruction mode shall be adopted.
  if(param_size>13)
  {
    len=4;
    Instr_index=3;
    param_size_index=4;
    Instr_extend=1;
  }
  
  uint8_t char_size=length/3;//(*in_ascii)[3]Consortium occupies 3 bytes
  sendbuff[param_size_index]=char_size;
  sendbuff[Instr_index]=Instr_name;
  
  for(uint8_t i=0;i<char_size;i++){
    uint8_t *info=in_ascii[i];
    uint8_t n_row=info[0];//row
    uint8_t n_col=info[1];//col
    if(font==FONT_5x7)
    {
      if(n_col>122)n_col=122;
      if(n_row>7)n_row=7;
    }
    else if(font==FONT_8x16)
    {
      if(n_col>119)n_col=119;
      if(n_row>3)n_row=3;
    }
  
    len++;
    sendbuff[len]=n_row;//row
    len++;
    sendbuff[len]=n_col;//col
    len++;
    sendbuff[len]=info[2];
  }
  if(Instr_extend)
  {
    sendbuff[1]=EID;
    sendbuff[2]=len-1;
  }
  else
  {
    sendbuff[1]=(len<<4)|EID;
  }
  uint8_t send_length=len+1;
  bool result = sendData(EID,sendbuff,send_length);
  return result;

}
/**********************************************************
Description: Batch write ASCII characters with different positions (rows and columns)
Parameters: 
  EID: module EID
  row:row
  xCoordinate:X coordinate
  in_ascii:Array of stored characters
  size：in_sizeof(in_ascii)
  font: font size of the character to be displayed
  5x7 characters: font_5x7
  8x16 characters: font_8x16
  syncAction :Delay execution? 0:no ,1:yes        
Return: Communication status（true:success ,false:fail） 
Others:   
**********************************************************/
bool BM32O2531_A::showChar(uint8_t EID,uint8_t  row,uint8_t  xCoordinate , char asciiChar ,uint8_t font,uint8_t  syncAction)
{
  uint8_t Instr_name=0;
    //Serial.print(size,DEC);
  if(font==FONT_5x7)
  {
    Instr_name=BM32O2531_A_CMD_5X7_CHAR;//
  }
  else if(font==FONT_8x16)
  {
    Instr_name=BM32O2531_A_CMD_8X16_CHAR;//
  }
  else
  {
    return false;
  }
  if(syncAction ==BM32O2531_A_CMD_syncAction )
  {
    Instr_name|=BM32O2531_A_CMD_syncAction ;
  }
  uint8_t sendbuff[256]={0};
  
  sendbuff[0]=BM32O2531_A_MID;
     
  uint8_t Instr_index=2;
  uint8_t param_size_index=3;
  uint8_t len=3;

  sendbuff[param_size_index]=1;
  sendbuff[Instr_index]=Instr_name;
    len++;
    sendbuff[len]=row;//row
    len++;
    sendbuff[len]=xCoordinate;//x Coordinate
    len++;
    sendbuff[len]=asciiChar;
  
    sendbuff[1]=(len<<4)|EID;
  uint8_t send_length=len+1;
  bool result = sendData(EID,sendbuff,send_length);
  return result;
}
/**********************************************************
Description: Start writing a series of consecutive ASCII characters at the specified position (row and column)
Parameters: 
  EID: module EID 
  row: line number
  xCoordinate: x Coordinate
  in_ascii: string array, such as uint8_t in_ ascii[]="BM32O2531-A Module"; 
  font: font size of the character to be displayed
  5x7 characters: font_5x7
  8x16 characters: font_8x16
  syncAction :Delay execution? 0:no ,1:yes         
Return: Communication status（true:success ,false:fail） 
Others:   
**********************************************************/
bool BM32O2531_A::showString(uint8_t EID,uint8_t row,uint8_t  xCoordinate ,char in_ascii[],uint8_t font,uint8_t  syncAction)
{
  uint8_t in_ascii_length=0;
  while(in_ascii_length<255)
  {
    in_ascii_length++;
    if(in_ascii[in_ascii_length]=='\0')
    {
      break;
    }
  }
  uint8_t size=in_ascii_length;
  if(size==0)
  return false;
  //FONT_8x16 not greater than 119
  uint8_t font_size=0;
  uint8_t col_size=21;
  uint8_t row_size=3;
  if(font==FONT_5x7)
  {
    font_size=6;
    if(xCoordinate>122)xCoordinate=122;
    if(row>7)row=7;
    row_size=7;
    col_size=21;//The maximum cannot exceed 22
  }
  else if(font==FONT_8x16)
  {
    font_size=8;
    if(xCoordinate>119)xCoordinate=119;
    if(row>3)row=3;
    row_size=3;
    col_size=16;//The maximum cannot exceed 16
  }
  else
  {
    return false;
  }
  
  uint8_t o_row=row;
  uint8_t current_col=xCoordinate;
  
  uint8_t col_start_offset=xCoordinate/font_size;//
  if((xCoordinate%font_size)!=0)
  {
    col_start_offset++;
  }
  
  uint8_t str_index=0;
  uint8_t str_first_row=0;
  uint8_t str_length=(size+col_start_offset);
  uint8_t send_count=str_length/col_size;
  uint8_t last_send_size=str_length%col_size;
  if(str_length%col_size!=0)
  {
    send_count++;
  }
  else 
  {
    last_send_size=col_size;
  }
  
  uint8_t result = true;
  do{
    uint8_t dest[256]={0};
    uint8_t cpy_length=0;
    if(str_first_row==0)//First line
    {
      str_first_row=1;
      cpy_length=col_size-col_start_offset;
      if(cpy_length>size)
      {
        cpy_length=size;
      }
      memcpy(dest, in_ascii+str_index, cpy_length);
      uint8_t t_result=writeSingleRowCharacter(EID,o_row,current_col,dest,cpy_length,font,syncAction );
      if(t_result==false)
      {
        result=false;//There was a fall, and this operation was equivalent to a foil
      }
      str_index+=(col_size-col_start_offset);
      
    }
    else if(send_count==1)//last line
    {
      memcpy(dest, in_ascii+str_index, last_send_size+col_start_offset);
      uint8_t t_result=writeSingleRowCharacter(EID,o_row,current_col,dest,last_send_size+col_start_offset,font,syncAction );
      if(t_result==false)
      {
        result=false;//There was a fall, and this operation was equivalent to a foil
      }
    }
    else
    {
      memcpy(dest, in_ascii+str_index, col_size);
      uint8_t t_result=writeSingleRowCharacter(EID,o_row,current_col,dest,col_size,font,syncAction );
      if(t_result==false)
      {
        result=false;//There was a fall, and this operation was equivalent to a foil
      }
      str_index+=(col_size);
      
    }
    current_col=0;//After the line change, it is implied in column 1
    if(o_row<row_size)
    {
      o_row++;
    }
    else
    {
      break;
    }
  }
  while  
    (--send_count);
     
  return result;
}
/**********************************************************
Description: Display integers on the screen
Parameters: 
  EID: module EID
  row:Lines to be written
  xCoordinate:xCoordinate to be written
  num:Number to be written
  font: font size of the character to be displayed
  5x7 characters: font_5x7
  8x16 characters: font_8x16
  syncAction :Delay execution? 0:no ,1:yes        
Return: Communication status（true:success ,false:fail） 
Others:   
**********************************************************/

bool BM32O2531_A::showNum(uint8_t EID,uint8_t  row,uint8_t  xCoordinate , int num ,uint8_t font,uint8_t  syncAction)
{
  char numString[10] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
  char temp[10] = {0};
  uint8_t  index = 0;
  uint8_t i = 0;
  
  do
  {
    temp[index] = num % 10 + '0';
    num = num /10;
    index++;
  }while(num);
  uint8_t numLen=index;
  for(i = 0;i < numLen;i++)
  {
    if(i >= index)
    {
      numString[numLen - i - 1] = ' ';
    }
    else
    {
      numString[numLen - i - 1] = temp[i];
    }
  }
  numString[numLen] = '\0';

  showString(EID,row,xCoordinate,numString,font,syncAction);
  return true;
}

/**********************************************************
Description: Display floating point on screen
Parameters: 
  EID: module EID
  row:Lines to be written
  xCoordinate:xCoordinate to be written
  num:Number to be written
  numLen:Length of the number to be written
  pointNumLen:Number of decimal places
  font: font size of the character to be displayed
  5x7 characters: font_5x7
  8x16 characters: font_8x16
  syncAction :Delay execution? 0:no ,1:yes        
Return: Communication status（true:success ,false:fail） 
Others:   
**********************************************************/

bool BM32O2531_A::showFloatNum(uint8_t EID,uint8_t  row,uint8_t  xCoordinate , float num,uint8_t numLen,uint8_t pointNumLen,uint8_t font,uint8_t  syncAction)
{
  char numString[10] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
  char temp[10] = {0};
  unsigned char value = 0;
  unsigned char i = 0;
  unsigned long numTemp;
  
  for(i = 0;i < pointNumLen;i++)
  {
    num = num * 10;
  }
  numTemp = num;
  if(numLen == 0)
  {
    return false;
  }

  do
  {
    temp[value] = numTemp % 10 + '0';
    numTemp = numTemp /10;
    value++;
  }while(numTemp);

  if(value > (numLen - 1))
  {
    return false;
  }
  
  for(i = 0;i < numLen;i++)
  {
    if(i >= value)
    {
      if(i <= pointNumLen)
      {
        numString[numLen - i - 1] = '0';
      }
      else
      {
        numString[numLen - i - 1] = ' ';
      }
    }
    else
    {
      numString[numLen - i - 1] = temp[i];
    }
  }

  for(i = 0;i < (numLen - pointNumLen - 1);i++)
  {
    numString[i] = numString[i + 1];
  }
  numString[numLen - pointNumLen - 1] = '.';
  
  numString[numLen] = '\0';

  showString(EID,row,xCoordinate,numString,font,syncAction);
  return true;
}
	

/**********************************************************
Description: Writes a sequence of consecutive ASCII characters starting at a single line at a specified position (row and column)
Parameters: 
  EID: module EID
  row: line number
  col: column number
  in_ascii: string array, such as uint8_t in_ ascii[]="BM32O2531-A Module"; 
  size:Number of characters to be displayed
  font: font size of the character to be displayed
  5x7 characters: font_5x7
  8x16 characters: font_8x16
  syncAction :Delay execution? 0:no ,1:yes        
Return: Communication status（true:success ,false:fail） 
Others:   
**********************************************************/
bool BM32O2531_A::writeSingleRowCharacter(uint8_t EID,uint8_t row,uint8_t col,uint8_t in_ascii[],uint8_t size,uint8_t font,uint8_t syncAction ){
  if(size==0)
  return false;
  uint8_t Instr_name=0;
  if(font==FONT_5x7)
  {
    Instr_name=BM32O2531_A_CMD_5X7_STRING;//
    if(col>122)
    {
      col=122;
    }
    if(row>7)
    {
      row=7;
    }
  }
  else if(font==FONT_8x16)
  {
    Instr_name=BM32O2531_A_CMD_8X16_STRING;//
    if(col>119)
    {
      col=119;
    }
    if(row>3)
    {
      row=3;
    }
  }
  else
  {
    return false;
  }
  if(syncAction ==BM32O2531_A_CMD_syncAction )
  {
    Instr_name|=BM32O2531_A_CMD_syncAction ;
  }
  uint8_t sendbuff[256]={0};
  
  sendbuff[0]=BM32O2531_A_MID;
  uint8_t Instr_extend=0;
  uint8_t Instr_index=2;
  uint8_t param_size_index=3;
  uint8_t len=3;

  //Param greater than13
  if(size>13-3)//contain param、row，col，larger than 13 strokes, requires an extension command
  {
    len=4;
    Instr_index=3;
    param_size_index=4;
    Instr_extend=1;
  }
  
  sendbuff[param_size_index]=size;
  sendbuff[Instr_index]=Instr_name;
  
  len++;
  sendbuff[len]=row;//row line coordinates
  len++;
  sendbuff[len]=col;//col column coordinates
  for(uint8_t i=0;i<size;i++)
  {
    len++;
    sendbuff[len]=in_ascii[i];
  }
  
  if(Instr_extend)
  {
    sendbuff[1]=EID;
    sendbuff[2]=len-1;
  }
  else
  {
    sendbuff[1]=(len<<4)|EID;
  }
  //Serial.println(len,HEX);
  uint8_t send_length=len+1;
  bool result = sendData(EID,sendbuff,send_length);
  return result;
} 

/**********************************************************
Description: Send len length data to the bus
Parameters:
      EID: moudle EID
      data:Array pointer
			len:Array length
			ack:Need ACK? 0:no ack ,1:need ack         
Return: Communication status（true:success ,false:fail） 
Others:   
**********************************************************/
bool BM32O2531_A::sendData(uint8_t EID,uint8_t data[],uint8_t len,uint8_t ack){
	uint8_t result=false;
	uint8_t i;
	uint8_t count=4;//Retry count
	uint16_t sendBuf[255]={0};
	sendBuf[0]= 0x0100|data[0]; 
	
	if(ack==BM32O2531_A_NO_ACK) count=0;
	do{ 
		uint8_t CheckSum = data[0];
		for(i=1;i<len;i++){
			sendBuf[i]=data[i]; 
			CheckSum += data[i];
		}  
		sendBuf[i] = (uint8_t)(~CheckSum); 
		
		writeBtyes(sendBuf,i+1);
		delay(10);
		uint8_t read_len=4;
		if(data[2]==BM32O2531_A_CMD_GET_FW_VER)
     {
  		read_len=6;//Version number needs 6 bytes
     }
		if(readData(EID,read_len))
		{
			result=true;
			break;
		}
		else
		{
			result=false;
		}
		if(count)
		count--;	
		else 
		break;
	}
	while(result==false||count);
	
	result=true;
	return result;
}
/**********************************************************
Description: Receiving bus sends data with length of len
Parameters: 
           EID:moudle EID
           read_len: Read data length            
Return: Communication status（true:success ,false:fail） 
Others:  
**********************************************************/
bool BM32O2531_A::readData(uint8_t EID,uint8_t read_len){
	memset(recBuf,0,sizeof(recBuf));//Clear zero
	uint8_t code=readBytes(recBuf,read_len);
	if((recBuf[0]&0xff) == BM32O2531_A_MID && (recBuf[1] & 0x0f)== EID&&code==CHECK_OK)
	{
		if(recBuf[2]==0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

/**********************************************************
Description: Receiving bus sends data with length of len
Parameters:
         rbuf:Store read data
          rlen: Read data length     
          waitTime: time out          
Return: Communication status（true:success ,false:fail） 
Others:   Data is saved in recBuf
**********************************************************/
uint8_t BM32O2531_A::readBytes(uint8_t rbuf[],uint8_t rlen,uint8_t waitTime){
	 uint8_t i = 0, delayCnt = 0, checkSum = 0;
	for (i = 0; i < rlen; i++)
    {
      delayCnt = 0;
      while (_serial->available() == 0)
      {
        if (delayCnt > waitTime)
        {
          return TIMEOUT_ERROR; // Timeout error
        }
        delay(1);
        delayCnt++;
      }
      rbuf[i] = _serial->read();
    }
	/* check Sum */
	for (i = 0; i < (rlen - 1); i++)
	{
		checkSum += rbuf[i];
	}
	checkSum = ~checkSum;
	if (checkSum == rbuf[rlen - 1])
	{
		return CHECK_OK; // Check correct
	}
	else
	{
		return CHECK_ERROR; // Check error
	}

}
/*************************************************
Description:  writeBtyes.
Parameters:   writeData[] : Data to be written.
              len : Data length.           
Return:             
Others:            
*************************************************/
void BM32O2531_A::writeBtyes(uint16_t writeData[], uint8_t len)
{
   while(_serial->available() > 0)
    {
       _serial->read();
    }
  _serial->SetTxStatus(ENABLE);
  _serial->SetRxStatus(DISABLE);

  for(uint8_t i = 0; i<len ;i++)
  {
    _serial->write(writeData[i]);
  }
   _serial->flush();
   
    _serial->SetTxStatus(DISABLE);
    _serial->SetRxStatus(ENABLE);
}
