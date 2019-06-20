#define LedPin 13
#define SolenoidPin 12

#include "ER_NET.h"

#define BCD_A 30
#define BCD_B 26
#define BCD_C 24
#define BCD_D 28
#define BCD_CLEAR 32

int HandDetection = 700;
int CoilDetection = 80; //deze waarde hangt heel erg sterk af van de voedingsspanning en moet dus nog even naar gekeken worden
int HandsPin = A3;
int CoilPin = A0;

bool NeedNewNumber = true;
bool hatch_open = false;
int DisplayNumber = 0;

ErNet erNet;

void setup() {
  Serial.begin(115200); //  setup serial
  pinMode(LedPin, OUTPUT); //led
  pinMode(SolenoidPin, OUTPUT); //solenoid

  erNet.Setup("FecPuzzle", 0x10, 0x50, 0x22, 0x30, 0xF2, 0xED);
  SetupDisplay();
}

void loop() {
  HandPuzzle();
  
  if (hatch_open == true)
  {
    CoilPuzzle();
  }

  erNet.Loop();
  erNet.SetResetCallback(&OnReset);

  if(erNet.GetDisplayNumber(&DisplayNumber) && NeedNewNumber) {
    NeedNewNumber = false;
  }
}

void HandPuzzle() {
  int HandsValue = analogRead(HandsPin);  // read the input pin
  //Serial.println(ReadValue); // debug value
  if (HandsValue < HandDetection) {
    //Hand detected
    digitalWrite(LedPin, HIGH);
    digitalWrite(SolenoidPin, HIGH);
    hatch_open = true;
    delay(100);
  }
  else
  {
    digitalWrite(LedPin, LOW);
    digitalWrite(SolenoidPin, LOW);
  }
}

void CoilPuzzle() {
  int CoilValue = analogRead(CoilPin);
  if (CoilValue > CoilDetection) { //Coil detected
    //Serial.println(CoilValue);
    //zet het 7 segment display aan
    Serial.println(DisplayNumber);
    ShowNumber(DisplayNumber);
    digitalWrite(LedPin, HIGH);
  }
}

//Do reset things
void OnReset() {
  Serial.println("RESET RECEIVED");
  digitalWrite(BCD_CLEAR, HIGH);
  NeedNewNumber = true;
  hatch_open = false;
  digitalWrite(SolenoidPin, HIGH);
  delay(10000);
  digitalWrite(SolenoidPin, LOW);
}

void SetupDisplay() {
  pinMode(BCD_A, OUTPUT);
  pinMode(BCD_B, OUTPUT);
  pinMode(BCD_C, OUTPUT);
  pinMode(BCD_D, OUTPUT);
  pinMode(BCD_CLEAR, OUTPUT);

  digitalWrite(BCD_A, LOW);
  digitalWrite(BCD_B, LOW);
  digitalWrite(BCD_C, LOW);
  digitalWrite(BCD_D, LOW);

  digitalWrite(BCD_CLEAR, HIGH);
}

void ShowNumber(int number) {
  if(number <= 0 || number > 9) {
    return;
  } else {
    digitalWrite(BCD_A, (number & ( 1 << 0 )) >> 0);
    digitalWrite(BCD_B, (number & ( 1 << 1 )) >> 1);
    digitalWrite(BCD_C, (number & ( 1 << 2 )) >> 2);
    digitalWrite(BCD_D, (number & ( 1 << 3 )) >> 3);

    digitalWrite(BCD_CLEAR, LOW);
  }
}
