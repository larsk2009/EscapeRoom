#define LedPin 13
#define SolenoidPin 12
#define SegmentC 6

<<<<<<< HEAD
int HandDetection = 600;
int CoilDetection = 85; //deze waarde hangt heel erg sterk af van de voedingsspanning en moet dus nog even naar gekeken worden
int HandsPin = A3;
int CoilPin = A0;
=======
#include "ER_NET.h"

#define BCD_A 32
#define BCD_B 28
#define BCD_C 26
#define BCD_D 30
#define BCD_CLEAR 34

int HandDetection = 700;
int CoilDetection = 80; //deze waarde hangt heel erg sterk af van de voedingsspanning en moet dus nog even naar gekeken worden
int HandsPin = A3;
int CoilPin = A0;

bool NeedNewNumber = true;
int DisplayNumber = 0;

ErNet erNet;
>>>>>>> 37e4268881e7ea91462a25f32e011482cdc4edcc

void setup() {
  Serial.begin(115200); //  setup serial
  pinMode(LedPin, OUTPUT); //led
  pinMode(SolenoidPin, OUTPUT); //solenoid
<<<<<<< HEAD
  pinMode(SegmentC, OUTPUT);
=======

  erNet.Setup();
  SetupDisplay();
>>>>>>> 37e4268881e7ea91462a25f32e011482cdc4edcc
}

void loop() {
  HandPuzzle();
  CoilPuzzle();

  erNet.Loop();
  erNet.SetResetCallback(&OnReset);

  if(erNet.GetDisplayNumber(&DisplayNumber) && NeedNewNumber) {
    NeedNewNumber = false;
  }
}

void HandPuzzle() {
  int HandsValue = analogRead(HandsPin);  // read the input pin
  //Serial.println(HandsValue); // debug value
  if (HandsValue < HandDetection) {
    //Hand detected
    digitalWrite(LedPin, HIGH);
    digitalWrite(SolenoidPin, HIGH);
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
    Serial.println(CoilValue);
<<<<<<< HEAD
    //digitalWrite(SegmentC, HIGH); //zet het 7 segment display aan
  }
  else
  {
    //digitalWrite(SegmentC, LOW);
=======
    //zet het 7 segment display aan
    ShowNumber(DisplayNumber);
>>>>>>> 37e4268881e7ea91462a25f32e011482cdc4edcc
  }
}

//Do reset things
void OnReset() {
  Serial.println("RESET RECEIVED");
  digitalWrite(BCD_CLEAR, HIGH);
  NeedNewNumber = true;
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