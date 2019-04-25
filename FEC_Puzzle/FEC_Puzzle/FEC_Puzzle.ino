#define LedPin 13
#define SolenoidPin 12
#define SegmentC 6

int HandDetection = 600;
int CoilDetection = 85; //deze waarde hangt heel erg sterk af van de voedingsspanning en moet dus nog even naar gekeken worden
int HandsPin = A3;
int CoilPin = A0;

void setup() {
  Serial.begin(9600); //  setup serial
  pinMode(LedPin, OUTPUT); //led
  pinMode(SolenoidPin, OUTPUT); //solenoid
  pinMode(SegmentC, OUTPUT);
}

void loop() {
  HandPuzzle();
  CoilPuzzle();
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
    //digitalWrite(SegmentC, HIGH); //zet het 7 segment display aan
  }
  else
  {
    //digitalWrite(SegmentC, LOW);
  }
}

