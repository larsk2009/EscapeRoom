#define C_Input 2
#define B_Input 3
#define D_Input 4
#define A_Input 5
#define Clear 13
#define Digit_1 A0
#define Digit_2 A1
#define Digit_3 A2
#define Digit_4 A3
#define Digit_5 A4
#define Big_Red_Button 10
#define Magnet_Switch 8
#include <Keypad.h>

char NumberOnDisplay[5] = {'0', '0', '0', '0', '0'};
int SegmentSelect = 0;
int i = 0;
bool Enter = LOW;
char ten_thousands = '8';
char thousands = '2';
char hundredths = '2';
char tenths = '5';
char ones = '2';

char TheAwnser[5] = {ten_thousands, thousands, hundredths, tenths, ones};


boolean array_cmp(char *a, char *b, int len_a, int len_b) {
  int n;

  // if their lengths are different, return false
  if (len_a != len_b) {
    return false;
  }
  for (n = 0; n < len_a; n++) {
    if (a[n] != b[n]) {
      return false;
    }
  }
  for (n = 0; n < len_a; n++) {
    if (a[n] == b[n]) {
      return true;
    }
  }
}




const byte ROWS = 4;
const byte COLS = 3;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {25, 26, 27, 28}; // Grey, Yellow, Purple, Green
byte colPins[COLS] = {22, 23, 24}; //Green, Blue, Yellow

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup() {
  pinMode(C_Input, OUTPUT);
  pinMode(B_Input, OUTPUT);
  pinMode(D_Input, OUTPUT);
  pinMode(A_Input, OUTPUT);
  pinMode(Clear, OUTPUT);
  pinMode(Digit_1, OUTPUT);
  pinMode(Digit_2, OUTPUT);
  pinMode(Digit_3, OUTPUT);
  pinMode(Digit_4, OUTPUT);
  pinMode(Digit_5, OUTPUT);
  pinMode(Big_Red_Button, INPUT);
  pinMode(Magnet_Switch, OUTPUT);
  digitalWrite(Magnet_Switch, HIGH);
  Serial.begin(9600);
}


char KeyBoard()
{
  char customKey = customKeypad.getKey();
  return customKey;
}


void NumbMem()
{
  char key = KeyBoard();
  if (key != NO_KEY) {
    if (key == '#') {
      for (int i = 0; i < 5; i++) {
        NumberOnDisplay[i] = '0';
      }
      SegmentSelect = 0;
    }
    else if (key == '*') {
      Enter = HIGH;
      SegmentSelect = 0;
    }
    else
    {
      NumberOnDisplay[SegmentSelect] = key;
      SegmentSelect++;
      if (SegmentSelect == 5)
      {
        SegmentSelect = 0;
      }
    }
  }
}

void Compare() {
  if (array_cmp(NumberOnDisplay, TheAwnser, 5, 5) == true) {
    // do this if they are equal
    //Victory
    Serial.println ("Victory");

    for (int i = 0; i < 5; i++) {
      NumberOnDisplay[i] = '#';
    }
    SegmentSelect = 0;
    Enter = LOW;
    Release();
  }
  else {
    // do this if they are different
    Serial.println ("Failure");
    for (int i = 0; i < 5; i++) {
      NumberOnDisplay[i] = '0';
    }
    SegmentSelect = 0;
    Enter = LOW;
  }
}
void Release() {
  digitalWrite(Magnet_Switch, LOW);
  Serial.println("hello");
}

void Finished() {

}

void ChangeNumb(char Number)
{
  switch (Number) {
    case '#':
      digitalWrite(Clear, HIGH);
      break;
    case '0':
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, LOW);
      digitalWrite(B_Input, LOW);
      digitalWrite(C_Input, LOW);
      digitalWrite(D_Input, LOW);
      break;
    case '1':
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, HIGH);
      digitalWrite(B_Input, LOW);
      digitalWrite(C_Input, LOW);
      digitalWrite(D_Input, LOW);
      break;
    case '2':
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, LOW);
      digitalWrite(B_Input, HIGH);
      digitalWrite(C_Input, LOW);
      digitalWrite(D_Input, LOW);
      break;
    case '3':
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, HIGH);
      digitalWrite(B_Input, HIGH);
      digitalWrite(C_Input, LOW);
      digitalWrite(D_Input, LOW);
      break;
    case '4':
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, LOW);
      digitalWrite(B_Input, LOW);
      digitalWrite(C_Input, HIGH);
      digitalWrite(D_Input, LOW);
      break;
    case '5':
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, HIGH);
      digitalWrite(B_Input, LOW);
      digitalWrite(C_Input, HIGH);
      digitalWrite(D_Input, LOW);
      break;
    case '6':
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, LOW);
      digitalWrite(B_Input, HIGH);
      digitalWrite(C_Input, HIGH);
      digitalWrite(D_Input, LOW);
      break;
    case '7':
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, HIGH);
      digitalWrite(B_Input, HIGH);
      digitalWrite(C_Input, HIGH);
      digitalWrite(D_Input, LOW);
      break;
    case '8':
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, LOW);
      digitalWrite(B_Input, LOW);
      digitalWrite(C_Input, LOW);
      digitalWrite(D_Input, HIGH);
      break;
    case '9':
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, HIGH);
      digitalWrite(B_Input, LOW);
      digitalWrite(C_Input, LOW);
      digitalWrite(D_Input, HIGH);
      break;
    default:
      digitalWrite(Clear, LOW);
      digitalWrite(A_Input, LOW);
      digitalWrite(B_Input, LOW);
      digitalWrite(C_Input, LOW);
      digitalWrite(D_Input, LOW);
      break;

  }
}

void ControlDisplays(int Display)
{
  switch (Display) {
    case 0:
      digitalWrite(Digit_1, HIGH);
      digitalWrite(Digit_2, LOW);
      digitalWrite(Digit_3, LOW);
      digitalWrite(Digit_4, LOW);
      digitalWrite(Digit_5, LOW);
      break;
    case 1:
      digitalWrite(Digit_1, LOW);
      digitalWrite(Digit_2, HIGH);
      digitalWrite(Digit_3, LOW);
      digitalWrite(Digit_4, LOW);
      digitalWrite(Digit_5, LOW);
      break;
    case 2:
      digitalWrite(Digit_1, LOW);
      digitalWrite(Digit_2, LOW);
      digitalWrite(Digit_3, HIGH);
      digitalWrite(Digit_4, LOW);
      digitalWrite(Digit_5, LOW);
      break;
    case 3:
      digitalWrite(Digit_1, LOW);
      digitalWrite(Digit_2, LOW);
      digitalWrite(Digit_3, LOW);
      digitalWrite(Digit_4, HIGH);
      digitalWrite(Digit_5, LOW);
      break;
    case 4:
      digitalWrite(Digit_1, LOW);
      digitalWrite(Digit_2, LOW);
      digitalWrite(Digit_3, LOW);
      digitalWrite(Digit_4, LOW);
      digitalWrite(Digit_5, HIGH);
      break;

  }
}


void loop()
{
  NumbMem();
  delay(1);
  ChangeNumb(NumberOnDisplay[i]);
  ControlDisplays(i);
  i++;
  if (Enter == HIGH) {
    Compare();
  }
  if (digitalRead(Big_Red_Button) == HIGH) {
    Finished();
    Serial.println("Finished");
  }
  if (i > 4) {
    i = 0;
  }

}
