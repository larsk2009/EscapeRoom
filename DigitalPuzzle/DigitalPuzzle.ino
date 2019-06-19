#include <ShiftRegister74HC595.h>
#include <EEPROM.h>
#include "rfid1.h"
#include "portReader.h"
#include "LedStrip.h"

const int AMOUNT_OF_WIRES = 16;
const int AMOUNT_OF_READERS = 8;

portReader readers[AMOUNT_OF_READERS];
LedStrip wires[AMOUNT_OF_WIRES];


const int ReaderResultPins[8] = { 38,40,42,44,46,48,50,52 };
const int StartValuePins[6] = { 2,3,4,5,6,7 };

const int PinA = 31;
const int PinB = 33;
const int PinC = 35;
const int PinD = 37;
const int ClearDisplay = 39;

int count = 0;

//(number of shift registers, data pin, clock pin, latch pin)
ShiftRegister74HC595 sr(4, 7, 5, 6);
RFID1 rfid;      //Create a variable type to read the RFID scanner 

int StartValues[6] = { 1,1,1,1,1,1 };
bool ReaderResult[8] = { 0,0,0,0,0,0,0,0 };
bool TagForReader[8] = { 0,0,0,0,0,0,0,0 };

uchar IDStorage[5]; //Array to store your UID of the tag


/*************************************************************
 * Function：ReadRFID
 * Description：read the UID of the given tag
 * Input parameter：number of the reader that needs to be read
 * Return：returns the read value (array of four values)
 *************************************************************/
uchar *ReadRFID(int ReaderSelect)
{
	switch (ReaderSelect) //Select one of the eight readers 
	{                     //Pin Configuration of RFID.begin (IRQ_PIN,SCK_PIN,MOSI_PIN,MISO_PIN,SDA_PIN,RST_PIN)
	case 0:
		rfid.begin(12, 13, 9, 24, 10, 11);
		break;
	case 1:
		rfid.begin(12, 13, 9, 26, 10, 11);
		break;
	case 2:
		rfid.begin(12, 13, 9, 28, 10, 11);
		break;
	case 3:
		rfid.begin(12, 13, 9, 30, 10, 11);
		break;
	case 4:
		rfid.begin(12, 13, 9, 22, 10, 11);
		break;
	case 5:
		rfid.begin(12, 13, 9, 34, 10, 11);
		break;
	case 6:
		rfid.begin(12, 13, 9, 32, 10, 11);
		break;
	case 7:
		rfid.begin(12, 13, 9, 36, 10, 11);
		break;
	}
	delay(10);  //delay 100ms
	rfid.init(); //initialize the RFID

	uchar status;
	uchar str[MAX_LEN];
	// Search card, return card types
	status = rfid.request(PICC_REQIDL, str);
	if (status != MI_OK)
	{
		for (int i; i < 5; i++)
		{
			IDStorage[i] = 0; //Set IDStorage to 0 when no card is detected
		}
    TagForReader[ReaderSelect] = 0;
		return;
	}
	//Prevent conflict, return the 4 bytes Serial number of the card
	status = rfid.anticoll(str);
	if (status == MI_OK)
	{
		memcpy(IDStorage, str, 5);
		Serial.print("Reader ");
		Serial.print(ReaderSelect);
		Serial.print(": ");
		rfid.showCardID(IDStorage);//show the card 
		Serial.println();
        TagForReader[ReaderSelect] = 1;
		delay(10);  //delay of 100ms
		rfid.halt(); //command the card into sleep mode 
		return IDStorage;
	}
}

/*************************************************************
 * Function：CompareNFC
 * Description：Compares a read tag with the a gate tag that
   has been saved in the EEPROM of the Arduino
 * Input parameter：unsigned char array with the tagID
 * Return：returns integer; 0 = UNKOWN; 1 = AND; 2 = OR;
   3 = NAND; 4 = NOR; 5 = XOR; 6 = NOT; 7 = WIRE;
 *************************************************************/
int CompareNFC(unsigned char tagID[]) {
	int gate = 0;

	if (tagID[0] == EEPROM.read(0)) {
		if (tagID[1] == EEPROM.read(1)) {
			if (tagID[2] == EEPROM.read(2)) {
				if (tagID[3] == EEPROM.read(3)) {
					gate = 1; //and gate
					Serial.println("AND GATE");
				}
			}
		}
	}
	if (tagID[0] == EEPROM.read(4)) {
		if (tagID[1] == EEPROM.read(5)) {
			if (tagID[2] == EEPROM.read(6)) {
				if (tagID[3] == EEPROM.read(7)) {
					gate = 2; // and gate
					Serial.println("AND GATE");
				}
			}
		}
	}
	if (tagID[0] == EEPROM.read(8)) {
		if (tagID[1] == EEPROM.read(9)) {
			if (tagID[2] == EEPROM.read(10)) {
				if (tagID[3] == EEPROM.read(11)) {
					gate = 3; //or gate
					Serial.println("OR GATE");
				}
			}
		}
	}
	if (tagID[0] == EEPROM.read(12)) {
		if (tagID[1] == EEPROM.read(13)) {
			if (tagID[2] == EEPROM.read(14)) {
				if (tagID[3] == EEPROM.read(15)) {
					gate = 4; // nand gate
					Serial.println("NAND GATE");
				}
			}
		}
	}
	if (tagID[0] == EEPROM.read(16)) {
		if (tagID[1] == EEPROM.read(17)) {
			if (tagID[2] == EEPROM.read(18)) {
				if (tagID[3] == EEPROM.read(19)) {
					gate = 5; //nor gate
					Serial.println("NOR GATE");
				}
			}
		}
	}
	if (tagID[0] == EEPROM.read(20)) {
		if (tagID[1] == EEPROM.read(21)) {
			if (tagID[2] == EEPROM.read(22)) {
				if (tagID[3] == EEPROM.read(23)) {
					gate = 6; //xor gate
					Serial.println("XOR GATE");
				}
			}
		}
	}
	if (tagID[0] == EEPROM.read(24)) {
		if (tagID[1] == EEPROM.read(25)) {
			if (tagID[2] == EEPROM.read(26)) {
				if (tagID[3] == EEPROM.read(27)) {
					gate = 7; //not gate
					Serial.println("NOT GATE");
				}
			}
		}
	}
	if (tagID[0] == EEPROM.read(28)) {
		if (tagID[1] == EEPROM.read(29)) {
			if (tagID[2] == EEPROM.read(30)) {
				if (tagID[3] == EEPROM.read(31)) {
					gate = 8; //wire
					Serial.println("WIRE");
				}
			}
		}
	}
	return gate;
}


/*************************************************************
 * Function：ReadSerial
 * Description：Reads the incoming serial message and process
   this.
 * Input parameter：none
 * Return：none
 *************************************************************/
void ReadSerial()
{
	String Q;
	String ReadString;

	while (Serial.available())
	{
		delay(10);
		if (Serial.available() > 0)
		{
			char c = Serial.read();
			if (isControl(c))
			{
				break;
			}
			ReadString += c;
		}
	}
	Q = ReadString;
	//-------Checking Serial Read---------
	if (Q == "CAL")
	{
		CalibrateTags();
		Serial.println("Calibrating Done!");
	}
	else if (Q == "RESTART")
	{
		RandomizeStartValues();
	}
	else if (Q == "HELP")
	{
		Serial.println("--------------------------------------");
		Serial.println("---------        MENU        ---------");
		Serial.println("--------------------------------------");
		Serial.println("CAL               ->    Calibrate Tags");
		Serial.println("HELP              ->         Main Menu");
		Serial.println("RESTART           ->  Restart the game");
		Serial.println("--------------------------------------");
	}
}


/*************************************************************
 * Function：PutTagInEEPROM
 * Description：Stores an UID Tag at a given address
 * Input parameter：tagID in uchar and address where the UID
   needs to be saved.
 * Return：none
 *************************************************************/
void PutTagInEEPROM(unsigned char tagID[], int address) {
	for (int i = 0; i <= 3; i++) {
		EEPROM.put(4 * address + i, tagID[i]);
	}
}


/*************************************************************
 * Function：CalibrateTags()
 * Description：Performs the PutTagInEEPROM function x amount
   of times. This 'x' is the amount of readers
 * Input parameter：none
 * Return：none
 *************************************************************/
void CalibrateTags() {
	for (int i = 0; i < 8; i++) {
		PutTagInEEPROM(ReadRFID(i), i);
	}
}


/*************************************************************
 * Function：RandomizeStartValues()
 * Description：Generates random startvalues. This way, the
   puzzle will be different everytime. 64 options.
 * Input parameter：none
 * Return：none
 *************************************************************/
void RandomizeStartValues()
{
	for (int i; i < 6; i++)
	{
		StartValues[i] = random() % 2;
		if (StartValues[i] == 0) {
			sr.set(2*i, HIGH);
			sr.set(2*i+1, LOW);
		}
		else
		{
			sr.set(2 * i, LOW);
			sr.set(2 * i + 1, HIGH);
		}
	}
}

void RandomSSNumber()
{
  int RandomNumber = 0;
  RandomNumber = random(0,9);

  switch(RandomNumber)
  {
    case 0:
      digitalWrite(PinA, LOW);
      digitalWrite(PinB, LOW);
      digitalWrite(PinC, LOW);
      digitalWrite(PinD, LOW);
      break;
    case 1:
      digitalWrite(PinA, HIGH);
      digitalWrite(PinB, LOW);
      digitalWrite(PinC, LOW);
      digitalWrite(PinD, LOW);
      break; 
    case 2:
      digitalWrite(PinA, LOW);
      digitalWrite(PinB, HIGH);
      digitalWrite(PinC, LOW);
      digitalWrite(PinD, LOW);
      break;
    case 3:
      digitalWrite(PinA, HIGH);
      digitalWrite(PinB, HIGH);
      digitalWrite(PinC, LOW);
      digitalWrite(PinD, LOW);
      break; 
    case 4:
      digitalWrite(PinA, LOW);
      digitalWrite(PinB, LOW);
      digitalWrite(PinC, HIGH);
      digitalWrite(PinD, LOW);
      break;
    case 5:
      digitalWrite(PinA, HIGH);
      digitalWrite(PinB, LOW);
      digitalWrite(PinC, HIGH);
      digitalWrite(PinD, LOW);
      break; 
    case 6:
      digitalWrite(PinA, LOW);
      digitalWrite(PinB, LOW);
      digitalWrite(PinC, HIGH);
      digitalWrite(PinD, LOW);
      break;
    case 7:
      digitalWrite(PinA, HIGH);
      digitalWrite(PinB, HIGH);
      digitalWrite(PinC, HIGH);
      digitalWrite(PinD, LOW);
      break; 
    case 8:
      digitalWrite(PinA, LOW);
      digitalWrite(PinB, LOW);
      digitalWrite(PinC, LOW);
      digitalWrite(PinD, HIGH);
      break;
    case 9:
      digitalWrite(PinA, HIGH);
      digitalWrite(PinB, LOW);
      digitalWrite(PinC, LOW);
      digitalWrite(PinD, HIGH);
      break; 
  }
}





void setup()
{
	Serial.begin(115200);
	randomSeed(analogRead(0));
	delay(10);

  pinMode(PinA, OUTPUT);
  pinMode(PinB, OUTPUT);
  pinMode(PinC, OUTPUT);
  pinMode(PinD, OUTPUT);
  pinMode(ClearDisplay, OUTPUT);


  //sr.set(0, HIGH);
  //sr.set(2, HIGH);
  //sr.set(4, HIGH);
  //sr.set(6, HIGH);
  //sr.set(8, HIGH);
  //sr.set(10, HIGH);
  //sr.set(12, HIGH);
  //sr.set(16, HIGH);
  //sr.set(18, HIGH);
  //sr.set(20, HIGH);
  //sr.set(22, HIGH);
  //sr.set(24, HIGH);
  //sr.set(26, HIGH);
  //sr.set(28, HIGH);
  sr.setAllLow();
  //RandomizeStartValues();
  Serial.print(StartValues[0]);
  Serial.print(StartValues[1]);
  Serial.print(StartValues[2]);
  Serial.print(StartValues[3]);
  Serial.print(StartValues[4]);
  Serial.println(StartValues[5]);
  Serial.println("Setup Done!");

  //READER 0
  readers[0] = portReader(portReader::AND_GATE);

  wires[0] = LedStrip(L, 11, 10);
  wires[1] = LedStrip(H, 9, 8);
  wires[2] = LedStrip(Z, 19, 18);

  readers[0].Inputs[0] = &wires[0];
  readers[0].Inputs[1] = &wires[1];
  readers[0].Output = &wires[2];

  //READER 1
  readers[1] = portReader(portReader::AND_GATE);

  wires[3] = LedStrip(H, 7, 6);
  wires[4] = LedStrip(L, 5, 4);
  wires[5] = LedStrip(Z, 17, 16);
  readers[1].Inputs[0] = &wires[3];
  readers[1].Inputs[1] = &wires[4];
  readers[1].Output = &wires[5];

  //READER 2
  readers[2] = portReader(portReader::WIRE);

  wires[6] = LedStrip(H, 1, 0);
  wires[7] = LedStrip(NC, -1, -1);
  wires[8] = LedStrip(Z, 13, 12);
  readers[2].Inputs[0] = &wires[6];
  readers[2].Inputs[1] = &wires[7];
  readers[2].Output = &wires[8];

  //READER 3
  readers[3] = portReader(portReader::WIRE);

  wires[9] = LedStrip(NC, -1, -1);
  wires[10] = LedStrip(Z, 23, 22);
  readers[3].Inputs[0] = readers[0].Output;
  readers[3].Inputs[1] = &wires[9];
  readers[3].Output = &wires[10];

  //READER 4
  readers[4] = portReader(portReader::AND_GATE);

  wires[11] = LedStrip(H, 3, 2);
  wires[12] = LedStrip(Z, 21, 20);
  readers[4].Inputs[0] = readers[1].Output;
  readers[4].Inputs[1] = &wires[11];
  readers[4].Output = &wires[12];

  //READER 5
  readers[5] = portReader(portReader::AND_GATE);

  wires[13] = LedStrip(Z, 27, 26);
  readers[5].Inputs[0] = readers[3].Output;
  readers[5].Inputs[1] = readers[1].Output;
  readers[5].Output = &wires[13];

  //READER 6
  readers[6] = portReader(portReader::AND_GATE);

  wires[14] = LedStrip(Z, 25, 24);
  readers[6].Inputs[0] = readers[4].Output;
  readers[6].Inputs[1] = readers[2].Output;
  readers[6].Output = &wires[14];

  //READER 7
  readers[7] = portReader(portReader::NAND_GATE);

  wires[15] = LedStrip(Z, 29, 28);
  readers[7].Inputs[0] = readers[5].Output;
  readers[7].Inputs[1] = readers[6].Output;
  readers[7].Output = &wires[15];

  for (int i = 0; i < AMOUNT_OF_READERS; i++) {
	  readers[i].CalculateOutput();
  }


  for (int i = 0; i < 32; i++) {
	  sr.set(i, HIGH);
	  delay(10);
  }

  for (int i = 0; i < 32; i++) {
	  sr.set(i, LOW);
	  delay(10);
  }

}

void loop()
{
  ReadSerial();
  //ApplyResults();

  for (int i = 0; i < AMOUNT_OF_WIRES; i++) {
	  LedStrip strip = wires[i];
	  if (strip.RedPin == -1 || strip.GreenPin == -1) {
		  continue;
	  }
	  strip.CalcualtePinValues();
	  sr.set(strip.GreenPin, strip.GreenValue);
	  sr.set(strip.RedPin, strip.RedValue);
  }

  //Serial.println("TagForReader:");
  //for(int i; i<8; i++)
  //{
  //  Serial.println(TagForReader[i]);
  //}
}