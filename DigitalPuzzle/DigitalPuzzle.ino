#include <ShiftRegister74HC595.h>
#include "portReader.h"
#include <EEPROM.h>
#include "rfid1.h"
#include "LedStrip.h"

const int AMOUNT_OF_WIRES = 16;
const int AMOUNT_OF_READERS = 8;

//RFID Pins
const int IRQ_PIN = 12;
const int SCK_PIN = 13;
const int MOSI_PIN = 9;
const int SDA_PIN = 10;
const int RST_PIN = 11;

portReader readers[AMOUNT_OF_READERS];
LedStrip wires[AMOUNT_OF_WIRES];

//7 segment display???
const int PinA = 31;
const int PinB = 33;
const int PinC = 35;
const int PinD = 37;
const int ClearDisplay = 39;

//(number of shift registers, data pin, clock pin, latch pin)
ShiftRegister74HC595 sr(4, 7, 5, 6);
RFID1 rfid;      //Create a variable type to read the RFID scanner 

int StartValues[6] = { 1,1,1,1,1,1 };

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
		rfid.begin(IRQ_PIN, SCK_PIN, MOSI_PIN, 24, SDA_PIN, RST_PIN);
		break;
	case 1:
		rfid.begin(IRQ_PIN, SCK_PIN, MOSI_PIN, 26, SDA_PIN, RST_PIN);
		break;
	case 2:
		rfid.begin(IRQ_PIN, SCK_PIN, MOSI_PIN, 28, SDA_PIN, RST_PIN);
		break;
	case 3:
		rfid.begin(IRQ_PIN, SCK_PIN, MOSI_PIN, 30, SDA_PIN, RST_PIN);
		break;
	case 4:
		rfid.begin(IRQ_PIN, SCK_PIN, MOSI_PIN, 22, SDA_PIN, RST_PIN);
		break;
	case 5:
		rfid.begin(IRQ_PIN, SCK_PIN, MOSI_PIN, 34, SDA_PIN, RST_PIN);
		break;
	case 6:
		rfid.begin(IRQ_PIN, SCK_PIN, MOSI_PIN, 32, SDA_PIN, RST_PIN);
		break;
	case 7:
		rfid.begin(IRQ_PIN, SCK_PIN, MOSI_PIN, 36, SDA_PIN, RST_PIN);
		break;
	}
	//delay(1);  //delay 1ms
	rfid.init(); //initialize the RFID

	uchar status;
	uchar Tag_ID[MAX_LEN];
	// Search card, return card types
	status = rfid.request(PICC_REQIDL, Tag_ID);
	if (status != MI_OK)
	{
		for (int i; i < 5; i++)
		{
			IDStorage[i] = 0; //Set IDStorage to 0 when no card is detected
		}
		return IDStorage;
	}
	//Prevent conflict, return the 4 bytes Serial number of the card
	status = rfid.anticoll(Tag_ID);
	if (status == MI_OK)
	{
		memcpy(IDStorage, Tag_ID, 5);
		Serial.print("Reader ");
		Serial.print(ReaderSelect);
		Serial.print(": ");
		rfid.showCardID(IDStorage);//show the card 
		Serial.println();
		//delay(1);  //delay of 100ms
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
	portReader::GateType gate = portReader::NO_GATE; //no gate

	if (tagID[0] == EEPROM.read(0) && tagID[1] == EEPROM.read(1) && tagID[2] == EEPROM.read(2) && tagID[3] == EEPROM.read(3)) {
		gate = portReader::AND_GATE; 
		Serial.println("AND GATE");
	}
	if (tagID[0] == EEPROM.read(4) && tagID[1] == EEPROM.read(5) && (tagID[2] == EEPROM.read(6) && tagID[3] == EEPROM.read(7))) {
		gate = portReader::AND_GATE; 
		Serial.println("AND GATE");
	}
	if (tagID[0] == EEPROM.read(8) && tagID[1] == EEPROM.read(9) && tagID[2] == EEPROM.read(10) && tagID[3] == EEPROM.read(11)) {
		gate = portReader::OR_GATE; 
		Serial.println("OR GATE");
	}
	if (tagID[0] == EEPROM.read(12) && tagID[1] == EEPROM.read(13) && tagID[2] == EEPROM.read(14) && tagID[3] == EEPROM.read(15)) {
		gate = portReader::NAND_GATE; 
		Serial.println("NAND GATE");
	}
	if (tagID[0] == EEPROM.read(16) && tagID[1] == EEPROM.read(17) && tagID[2] == EEPROM.read(18) && tagID[3] == EEPROM.read(19)) {
		gate = portReader::NOR_GATE; 
		Serial.println("NOR GATE");
	}
	if (tagID[0] == EEPROM.read(20) && tagID[1] == EEPROM.read(21) && tagID[2] == EEPROM.read(22) && tagID[3] == EEPROM.read(23)) {
		gate = portReader::XOR_GATE; 
		Serial.println("XOR GATE");
	}
	if (tagID[0] == EEPROM.read(24) && tagID[1] == EEPROM.read(25) && tagID[2] == EEPROM.read(26) && tagID[3] == EEPROM.read(27)) {
		gate = portReader::INVERTER; 
		Serial.println("NOT GATE");
	}
	if (tagID[0] == EEPROM.read(28) && tagID[1] == EEPROM.read(29) && tagID[2] == EEPROM.read(30) && tagID[3] == EEPROM.read(31)) {
		gate = portReader::WIRE; 
		Serial.println("WIRE");
	}
	return gate;
}


/*************************************************************
 * Function：ProcessSerial
 * Description：Read the incoming serial message and process it
 * Input parameter：none
 * Return：none
 *************************************************************/
void ProcessSerial()
{
	String Q;
	String ReadString;

	while (Serial.available()>0)
	{
		char c = Serial.read();
		if (isControl(c))
		{
			break;
		}
		ReadString += c;
	}
	//-------Checking Serial Read---------
	if (ReadString == "CAL")
	{
		CalibrateTags();
		Serial.println("Calibrating Done!");
	}
	else if (ReadString == "RESTART")
	{
		RandomizeStartValues();
	}
	else if (ReadString == "HELP")
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
	}
	wires[0].Value  = (WireValues)StartValues[0];
	wires[1].Value  = (WireValues)StartValues[1];
	wires[3].Value  = (WireValues)StartValues[2];
	wires[4].Value  = (WireValues)StartValues[3];
	wires[11].Value = (WireValues)StartValues[4];
	wires[6].Value  = (WireValues)StartValues[5];
}

void UpdateReader(int Reader){
	readers[Reader].Gate = portReader::GateType(CompareNFC(ReadRFID(Reader)));
}

void CreateLayout()
{
	//READER 0
	readers[0] = portReader(portReader::NO_GATE);

	wires[0] = LedStrip(L, 11, 10);
	wires[1] = LedStrip(H, 9, 8);
	wires[2] = LedStrip(Z, 19, 18);

	readers[0].Inputs[0] = &wires[0];
	readers[0].Inputs[1] = &wires[1];
	readers[0].Output    = &wires[2];

	//READER 1
	readers[1] = portReader(portReader::NO_GATE);

	wires[3] = LedStrip(H, 7, 6);
	wires[4] = LedStrip(L, 5, 4);
	wires[5] = LedStrip(Z, 17, 16);
	readers[1].Inputs[0] = &wires[3];
	readers[1].Inputs[1] = &wires[4];
	readers[1].Output    = &wires[5];

	//READER 2
	readers[2] = portReader(portReader::NO_GATE);

	wires[6] = LedStrip(H, 1, 0);
	wires[7] = LedStrip(NC, -1, -1);
	wires[8] = LedStrip(Z, 13, 12);
	readers[2].Inputs[0] = &wires[6];
	readers[2].Inputs[1] = &wires[7];
	readers[2].Output    = &wires[8];

	//READER 3
	readers[3] = portReader(portReader::NO_GATE);

	wires[9] = LedStrip(NC, -1, -1);
	wires[10] = LedStrip(Z, 23, 22);
	readers[3].Inputs[0] = readers[0].Output;
	readers[3].Inputs[1] = &wires[9];
	readers[3].Output    = &wires[10];

	//READER 4
	readers[4] = portReader(portReader::NO_GATE);

	wires[11] = LedStrip(H, 3, 2);
	wires[12] = LedStrip(Z, 21, 20);
	readers[4].Inputs[0] = readers[1].Output;
	readers[4].Inputs[1] = &wires[11];
	readers[4].Output    = &wires[12];

	//READER 5
	readers[5] = portReader(portReader::NO_GATE);

	wires[13] = LedStrip(Z, 27, 26);
	readers[5].Inputs[0] = readers[3].Output;
	readers[5].Inputs[1] = readers[1].Output;
	readers[5].Output    = &wires[13];

	//READER 6
	readers[6] = portReader(portReader::NO_GATE);

	wires[14] = LedStrip(Z, 25, 24);
	readers[6].Inputs[0] = readers[4].Output;
	readers[6].Inputs[1] = readers[2].Output;
	readers[6].Output    = &wires[14];

	//READER 7
	readers[7] = portReader(portReader::NO_GATE);

	wires[15] = LedStrip(Z, 29, 28);
	readers[7].Inputs[0] = readers[5].Output;
	readers[7].Inputs[1] = readers[6].Output;
	readers[7].Output    = &wires[15];

	for (int i = 0; i < AMOUNT_OF_READERS; i++) {
		readers[i].CalculateOutput();
	}
}

void setup()
{
	Serial.begin(115200);
	randomSeed(analogRead(1));

  pinMode(PinA, OUTPUT);
  pinMode(PinB, OUTPUT);
  pinMode(PinC, OUTPUT);
  pinMode(PinD, OUTPUT);
  pinMode(ClearDisplay, OUTPUT);

  sr.setAllLow();

  CreateLayout();

  //Boot show
  for (int i = 0; i < 32; i++) {
	  sr.set(i, HIGH);
	  delay(10);
  }

  for (int i = 0; i < 32; i++) {
	  sr.set(i, LOW);
	  delay(10);
  }

  RandomizeStartValues();

  Serial.println("Setup Done!");
}

void loop()
{
  ProcessSerial();

  //lees alle readers en zet de outputs op de goede waardes
  for (int i = 0; i < AMOUNT_OF_READERS; i++){
	  UpdateReader(i);
    readers[i].CalculateOutput();
  }

  //update de waardes van alle ledstrips
  for (int i = 0; i < AMOUNT_OF_WIRES; i++) {
	  LedStrip strip = wires[i];
	  if (strip.RedPin == -1 || strip.GreenPin == -1) {
		  continue;
	  }
	  strip.CalcualtePinValues();
	  sr.set(strip.GreenPin, strip.GreenValue);
	  sr.set(strip.RedPin, strip.RedValue);
  }
}
