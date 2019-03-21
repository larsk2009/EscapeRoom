/*******************************************
* function:get the id of RFID key
* RFID   Uno r3ú
* VCC    3.3V
* RST    2
* GND    GND
* MISO   3
* MOSI   4
* SCK    5
* NSS    6
* IRQ    7
****************************************/

#include <EEPROM.h>
#include"rfid1.h"

const int RedPin[2] = {3,4};
const int GreenPin[2] = {2,5};

int count = 0;


RFID1 rfid;		 //Create a variable type to read the RFID scanner 

bool StartValues[6] = {1,1,1,0,0,0};
bool ReaderResult[8] = {0,0,0,0,0,0,0,0};

uchar serNum[5]; //Array to store your 
uchar AND_GATE[4] = {164, 175, 3, 11};


/*************************************************************
 * Function：ReadRFID 
 * Description：read the UID of the given tag 
 * Input parameter：number of the reader that needs to be read
 * Return：returns the read value (array of four values)
 *************************************************************/
uchar *ReadRFID(int ReaderSelect)
{
	switch (ReaderSelect) //Select one of the eight readers 
	{                     //Pin Configuration of RFID.begin (IRQ_PIN,SCK_PIN,MOSI_PIN,MISO_PIN,NSS_PIN,RST_PIN)
	  case 0: 
		  rfid.begin(9, 11, 10, 7, 12, 8);  
		  break;
	  case 1:
		  rfid.begin(9, 11, 10, 6, 12, 8);  
		  break;
	}
	delay(100);  //delay 100ms
	rfid.init(); //initialize the RFID

	uchar status;
	uchar str[MAX_LEN];
	// Search card, return card types
	status = rfid.request(PICC_REQIDL, str);
	if (status != MI_OK)
	{
		for (int i; i < 5; i++)
		{
			serNum[i] = 0; //Set serNum to 0 when no card is detected
		}
		return;
	}
	//Prevent conflict, return the 4 bytes Serial number of the card
	status = rfid.anticoll(str);
	if (status == MI_OK)
	{
		memcpy(serNum, str, 5);
		Serial.print("Reader ");
		Serial.print(ReaderSelect);
		Serial.print(": ");
		rfid.showCardID(serNum);//show the card 
        Serial.println();
    delay(100);  //delay of 100ms
    rfid.halt(); //command the card into sleep mode 
	  return serNum;
	}
	delay(100);
	rfid.halt(); //command the card into sleep mode 
}


bool *GetReaderResult(int ReaderSelect)
{
  switch(ReaderSelect)
  {
    case 0:
      switch(CompareNFC(ReadRFID(0)))
      {
        case 1:   //AND GATE
          ReaderResult[0] = Check_AND(StartValues[0], StartValues[1]);
          break;
        case 2:   //OR GATE
          ReaderResult[0] = Check_OR(StartValues[0], StartValues[1]);
          break;
        case 3:   //NAND GATE
          ReaderResult[0] = Check_NAND(StartValues[0], StartValues[1]);
          break;
        case 4:   //NOR GATE
          ReaderResult[0] = Check_NOR(StartValues[0], StartValues[1]);
          break;
        case 5:   //XOR GATE
          ReaderResult[0] = Check_XOR(StartValues[0], StartValues[1]);
          break;
        default:  //
          ReaderResult[0] = 0; 
          break;
      }
    case 1:
      switch(CompareNFC(ReadRFID(1)))
      {
        case 1:   //AND GATE
          ReaderResult[1] = Check_AND(StartValues[2], StartValues[3]);
          break;
        case 2:   //OR GATE
          ReaderResult[1] = Check_OR(StartValues[2], StartValues[3]);
          break;
        case 3:   //NAND GATE
          ReaderResult[1] = Check_NAND(StartValues[2], StartValues[3]);
          break;
        case 4:   //NOR GATE
          ReaderResult[1] = Check_NOR(StartValues[2], StartValues[3]);
          break;
        case 5:   //XOR GATE
          ReaderResult[1] = Check_XOR(StartValues[2], StartValues[3]);
          break;
        default:  //
          ReaderResult[1] = 0; 
          break;
      }
  }

  return ReaderResult;
}

void ApplyResults()
{
  if (GetReaderResult(0)[0] == true)
  {
	digitalWrite(GreenPin[0], HIGH);
	digitalWrite(RedPin[0], LOW);
  }
  else
  {
    digitalWrite(GreenPin[0], LOW);
	digitalWrite(RedPin[0], HIGH);
  }

  if (GetReaderResult(1)[1] == true)
  {
	  digitalWrite(GreenPin[1], HIGH);
	  digitalWrite(RedPin[1], LOW);
  }
  else
  {	  digitalWrite(GreenPin[1], LOW);
	  digitalWrite(RedPin[1], HIGH);
  }
}



bool Check_AND(bool A, bool B)
{
  if(A == 1 && B == 1)
  {
    return 1;
  }
  else 
  {
    return 0;
  }
}

bool Check_OR(bool A, bool B)
{
  if(A == 1 || B == 1)
  {
    return 1;
  }
  else 
  {
    return 0;
  }
}

bool Check_NAND(bool A, bool B)
{
  if(A == 1 && B == 1)
  {
    return 0;
  }
  else 
  {
    return 1;
  }
}

bool Check_NOR(bool A, bool B)
{
  if(A == 1 || B == 1)
  {
    return 0;
  }
  else 
  {
    return 1;
  }
}

bool Check_XOR(bool A, bool B)
{
  if(A == 0 && B == 1)
  {
    return 1;
  }
  else if(A == 1 && B == 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int CompareNFC (unsigned char tagID[]) {
  int gate = 0;

  if (tagID[0] == EEPROM.read(0)){
    if (tagID[1] == EEPROM.read(1)){
      if (tagID[2] == EEPROM.read(2)){
        if (tagID[3] == EEPROM.read(3)){
          gate = 1; //and gate
          Serial.println("AND GATE");
        }
      }
    }
  }
  if (tagID[0] == EEPROM.read(10)){
    if (tagID[1] == EEPROM.read(11)){
      if (tagID[2] == EEPROM.read(12)){
        if (tagID[3] == EEPROM.read(13)){
          gate = 2; // or gate
		  Serial.println("OR GATE");
        }
      }
    }
  }
  return gate;
}


void setup()
{
  Serial.begin(9600); 
  // EEPROM.put(0,164);
  // EEPROM.put(1,175); 
  // EEPROM.put(2,3); 
  // EEPROM.put(3,11); 
  // EEPROM.put(10,213); 
  // EEPROM.put(11,61); 
  // EEPROM.put(12,4); 
  // EEPROM.put(12,4); 
  // EEPROM.put(13,50);
  for(int i; i<2; i++)
  {
    pinMode(RedPin[i], OUTPUT);
    pinMode(GreenPin[i], OUTPUT);
  
    digitalWrite(RedPin[i], LOW);
    digitalWrite(GreenPin[i], LOW);
  }

  Serial.println("Setup Done!");
}

void loop()
{
	ApplyResults();
	//CompareNFC(ReadRFID(0));
	//CompareNFC(ReadRFID(1));
}