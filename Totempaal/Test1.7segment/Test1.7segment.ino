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
  pinMode(Segment_B, OUTPUT);
  pinMode(Segment_E, OUTPUT);
  pinMode(Segment_DP, OUTPUT);
  pinMode(Segment_A, OUTPUT);
  pinMode(Segment_F, OUTPUT);
  pinMode(Segment_G, OUTPUT);
  pinMode(Segment_D, OUTPUT);
  pinMode(Segment_C, OUTPUT);
  Serial.begin(9600);
}



void loop() {
  for (CaseState = 0; CaseState < 11; CaseState++)
  {
    KeyBoard();
    switch (CaseState) {
      case 0:
        digitalWrite(Segment_B, HIGH);
        digitalWrite(Segment_E, HIGH);
        digitalWrite(Segment_DP, LOW);
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_F, HIGH);
        digitalWrite(Segment_G, LOW);
        digitalWrite(Segment_D, HIGH);
        digitalWrite(Segment_7, HIGH);
        break;
      case 1:
        digitalWrite(Segment_B, HIGH);
        digitalWrite(Segment_E, LOW);
        digitalWrite(Segment_DP, LOW);
        digitalWrite(Segment_A, LOW);
        digitalWrite(Segment_F, LOW);
        digitalWrite(Segment_G, LOW);
        digitalWrite(Segment_D, LOW);
        digitalWrite(Segment_7, HIGH);
        break;
      case 2:
        digitalWrite(Segment_B, HIGH);
        digitalWrite(Segment_E, HIGH);
        digitalWrite(Segment_DP, LOW);
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_F, LOW);
        digitalWrite(Segment_G, HIGH);
        digitalWrite(Segment_D, HIGH);
        digitalWrite(Segment_7, LOW);
        break;
      case 3:
        digitalWrite(Segment_B, HIGH);
        digitalWrite(Segment_E, LOW);
        digitalWrite(Segment_DP, LOW);
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_F, LOW);
        digitalWrite(Segment_G, HIGH);
        digitalWrite(Segment_D, HIGH);
        digitalWrite(Segment_7, HIGH);
        break;
      case 4:
        digitalWrite(Segment_B, HIGH);
        digitalWrite(Segment_E, LOW);
        digitalWrite(Segment_DP, LOW);
        digitalWrite(Segment_A, LOW);
        digitalWrite(Segment_F, HIGH);
        digitalWrite(Segment_G, HIGH);
        digitalWrite(Segment_D, LOW);
        digitalWrite(Segment_7, HIGH);
        break;
      case 5:
        digitalWrite(Segment_B, LOW);
        digitalWrite(Segment_E, LOW);
        digitalWrite(Segment_DP, LOW);
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_F, HIGH);
        digitalWrite(Segment_G, HIGH);
        digitalWrite(Segment_D, HIGH);
        digitalWrite(Segment_7, HIGH);
        break;
      case 6:
        digitalWrite(Segment_B, LOW);
        digitalWrite(Segment_E, HIGH);
        digitalWrite(Segment_DP, LOW);
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_F, HIGH);
        digitalWrite(Segment_G, HIGH);
        digitalWrite(Segment_D, HIGH);
        digitalWrite(Segment_7, HIGH);
        break;
      case 7:
        digitalWrite(Segment_B, HIGH);
        digitalWrite(Segment_E, LOW);
        digitalWrite(Segment_DP, LOW);
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_F, LOW);
        digitalWrite(Segment_G, LOW);
        digitalWrite(Segment_D, LOW);
        digitalWrite(Segment_7, HIGH);
        break;
      case 8:
        digitalWrite(Segment_B, HIGH);
        digitalWrite(Segment_E, HIGH);
        digitalWrite(Segment_DP, LOW);
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_F, HIGH);
        digitalWrite(Segment_G, HIGH);
        digitalWrite(Segment_D, HIGH);
        digitalWrite(Segment_7, HIGH);
        break;
      case 9:
        digitalWrite(Segment_B, HIGH);
        digitalWrite(Segment_E, LOW);
        digitalWrite(Segment_DP, LOW);
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_F, HIGH);
        digitalWrite(Segment_G, HIGH);
        digitalWrite(Segment_D, HIGH);
        digitalWrite(Segment_7, HIGH);
        break;
       case 10:
        digitalWrite(Segment_B, LOW);
        digitalWrite(Segment_E, LOW);
        digitalWrite(Segment_DP, HIGH);
        digitalWrite(Segment_A, LOW);
        digitalWrite(Segment_F, LOW);
        digitalWrite(Segment_G, LOW);
        digitalWrite(Segment_D, LOW);
        digitalWrite(Segment_7, LOW);
        break;
    }
    delay(500);
  }
}
