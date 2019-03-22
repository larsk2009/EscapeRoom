int analogPin = A3; // potentiometer wiper (middle terminal) connected to analog pin 3
                    // outside leads to ground and +5V
int val = 0;  // variable to store the value read

void setup() {
  Serial.begin(9600); //  setup serial
  pinMode(13, OUTPUT); //led
  pinMode(12, OUTPUT); //solenoid
}

void loop() {
  val = analogRead(analogPin);  // read the input pin
  Serial.println(val);          // debug value
  if (val < 700) {
    digitalWrite(13, HIGH);
    digitalWrite(12, HIGH);
  }
  else
  {
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
  }
}
