#define Segment_B 0 //yellow
#define Segment_E 1 //brown
#define Segment_DP 2 //orange
#define Segment_A 3 // grey
#define Segment_F 4 //Purple
#define Segment_G 5 //red
#define Segment_D 6 //white
#define Segment_C 7 //blue
int CaseState;


void setup() {
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  for (CaseState = 0; CaseState < 11; CaseState++)
  {
    KeyBoard();
    switch (CaseState) {
      case 0:
        digitalWrite(0, HIGH);
        digitalWrite(1, HIGH);
        digitalWrite(2, LOW);
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
        digitalWrite(7, HIGH);
        break;
      case 1:
        digitalWrite(0, HIGH);
        digitalWrite(1, LOW);
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(7, HIGH);
        break;
      case 2:
        digitalWrite(0, HIGH);
        digitalWrite(1, HIGH);
        digitalWrite(2, LOW);
        digitalWrite(3, HIGH);
        digitalWrite(4, LOW);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
        digitalWrite(7, LOW);
        break;
      case 3:
        digitalWrite(0, HIGH);
        digitalWrite(1, LOW);
        digitalWrite(2, LOW);
        digitalWrite(3, HIGH);
        digitalWrite(4, LOW);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
        digitalWrite(7, HIGH);
        break;
      case 4:
        digitalWrite(0, HIGH);
        digitalWrite(1, LOW);
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, LOW);
        digitalWrite(7, HIGH);
        break;
      case 5:
        digitalWrite(0, LOW);
        digitalWrite(1, LOW);
        digitalWrite(2, LOW);
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
        digitalWrite(7, HIGH);
        break;
      case 6:
        digitalWrite(0, LOW);
        digitalWrite(1, HIGH);
        digitalWrite(2, LOW);
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
        digitalWrite(7, HIGH);
        break;
      case 7:
        digitalWrite(0, HIGH);
        digitalWrite(1, LOW);
        digitalWrite(2, LOW);
        digitalWrite(3, HIGH);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(7, HIGH);
        break;
      case 8:
        digitalWrite(0, HIGH);
        digitalWrite(1, HIGH);
        digitalWrite(2, LOW);
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
        digitalWrite(7, HIGH);
        break;
      case 9:
        digitalWrite(0, HIGH);
        digitalWrite(1, LOW);
        digitalWrite(2, LOW);
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
        digitalWrite(7, HIGH);
        break;
       case 10:
        digitalWrite(0, LOW);
        digitalWrite(1, LOW);
        digitalWrite(2, HIGH);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(7, LOW);
        break;
    }
    delay(500);
  }
}
