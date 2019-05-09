#include "ER_NET.h"

ErNet erNet;

#define BCD_A 32
#define BCD_B 28
#define BCD_C 26
#define BCD_D 30
#define BCD_CLEAR 34

void setup(){
  Serial.begin(115200);

  erNet.Setup("Totempaal", 0x70, 0x69, 0x69, 0x2D, 0x30, 0x31);

  SetupDisplay();
}

void loop(){
  //this must be called for ethercard functions to work.
  erNet.Loop();
  erNet.SetResetCallback(&OnReset);
  unsigned long test;/*
  if(erNet.GetDisplayNumber(&test)) {
    Serial.println(test);
    ShowNumber(test);
  }*/
  if(erNet.GetSolution(&test)) {
    Serial.println((unsigned long) test);
  }
}

void OnReset() {
  Serial.println("RESET RECEIVED");
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

  digitalWrite(BCD_CLEAR, LOW);
}

void ShowNumber(int number) {
  if(number <= 0 || number > 9) {
    return;
  } else {
    digitalWrite(BCD_A, (number & ( 1 << 0 )) >> 0);
    digitalWrite(BCD_B, (number & ( 1 << 1 )) >> 1);
    digitalWrite(BCD_C, (number & ( 1 << 2 )) >> 2);
    digitalWrite(BCD_D, (number & ( 1 << 3 )) >> 3);
  }
}