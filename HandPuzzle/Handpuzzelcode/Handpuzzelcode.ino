#define LedPin 13
#define SolenoidPin 12

int HandDetection = 700;
int AnalogPin = A3; // potentiometer wiper (middle terminal) connected to analog pin 3
                    // outside leads to ground and +5V
int ReadValue = 0;  // variable to store the value read

void setup() {
  Serial.begin(9600); //  setup serial
  pinMode(LedPin, OUTPUT); //led
  pinMode(SolenoidPin, OUTPUT); //solenoid
}

void loop() {
  ReadValue = analogRead(AnalogPin);  // read the input pin
  Serial.println(ReadValue);          // debug value
  if (ReadValue < HandDetection) { 
    digitalWrite(LedPin, HIGH);
    digitalWrite(SolenoidPin, HIGH);
  }
  else
  {
    digitalWrite(LedPin, LOW);
    digitalWrite(SolenoidPin, LOW);
  }
}
