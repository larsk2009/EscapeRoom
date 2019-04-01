#define C_Input 2
#define B_Input 3
#define D_Input 4
#define A_Input 5
#define Clear 13
#define Digit_1 A0
#define Digit_2 A1
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 3;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {12, 11, 10, 9}; // Grey, Yellow, Purple, Green
byte colPins[COLS] = {8, 7, 6}; //Green, Blue, Yellow

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup() {
  pinMode(C_Input, OUTPUT);
  pinMode(B_Input, OUTPUT);
  pinMode(D_Input, OUTPUT);
  pinMode(A_Input, OUTPUT);
  pinMode(Clear, OUTPUT);
  pinMode(Digit_1, OUTPUT);
  digitalWrite(Digit_1, LOW);
  pinMode(Digit_2, OUTPUT);
  digitalWrite(Digit_2, HIGH);
  Serial.begin(9600);
}


int KeyBoard()
{
  int input;
  char customKey = customKeypad.getKey();

  if (customKey) {
    Serial.println(customKey);
    input = customKey;
  }
  return input;
}

void ChangeNumb(int Number)
{
  switch (Number) {
    case 35:
      digitalWrite(Clear, HIGH);
      break;
    case 48:
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, LOW);
      digitalWrite(B_Input, LOW);
      digitalWrite(C_Input, LOW);
      digitalWrite(D_Input, LOW);
      break;
    case 49:
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, HIGH);
      digitalWrite(B_Input, LOW);
      digitalWrite(C_Input, LOW);
      digitalWrite(D_Input, LOW);
      break;
    case 50:
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, LOW);
      digitalWrite(B_Input, HIGH);
      digitalWrite(C_Input, LOW);
      digitalWrite(D_Input, LOW);
      break;
    case 51:
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, HIGH);
      digitalWrite(B_Input, HIGH);
      digitalWrite(C_Input, LOW);
      digitalWrite(D_Input, LOW);
      break;
    case 52:
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, LOW);
      digitalWrite(B_Input, LOW);
      digitalWrite(C_Input, HIGH);
      digitalWrite(D_Input, LOW);
      break;
    case 53:
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, HIGH);
      digitalWrite(B_Input, LOW);
      digitalWrite(C_Input, HIGH);
      digitalWrite(D_Input, LOW);
      break;
    case 54:
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, LOW);
      digitalWrite(B_Input, HIGH);
      digitalWrite(C_Input, HIGH);
      digitalWrite(D_Input, LOW);
      break;
    case 55:
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, HIGH);
      digitalWrite(B_Input, HIGH);
      digitalWrite(C_Input, HIGH);
      digitalWrite(D_Input, LOW);
      break;
    case 56:
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, LOW);
      digitalWrite(B_Input, LOW);
      digitalWrite(C_Input, LOW);
      digitalWrite(D_Input, HIGH);
      break;
    case 57:
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, HIGH);
      digitalWrite(B_Input, LOW);
      digitalWrite(C_Input, LOW);
      digitalWrite(D_Input, HIGH);
      break;

  }
}

void ControlDisplays(int Display) 
{
  switch (Display) {
    case 1:
      digitalWrite(Digit_1, HIGH);
      digitalWrite(Digit_2, LOW);
      break;
    case 2:
      digitalWrite(Digit_1, LOW);
      digitalWrite(Digit_2, HIGH);
      break;
  }
  }


void loop()
{
  ChangeNumb(KeyBoard());
  ControlDisplays(2);
  delay(1);
  ChangeNumb(51);
  ControlDisplays(1);
  delay(1);
}
