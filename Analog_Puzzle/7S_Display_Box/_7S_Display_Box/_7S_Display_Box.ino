#include "ER_NET.h"

#define BCD_A 32
#define BCD_B 28
#define BCD_C 26
#define BCD_D 30
#define BCD_CLEAR 34

bool NeedNewNumber = true;
int DisplayNumber = 0;

ErNet erNet;

void setup() {
  Serial.begin(115200); //  setup serial
  erNet.Setup();
  SetupDisplay();
}

void loop() {
  erNet.Loop();
  erNet.SetResetCallback(&OnReset);

  if (erNet.GetDisplayNumber(&DisplayNumber) && NeedNewNumber) {
    NeedNewNumber = false;
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
