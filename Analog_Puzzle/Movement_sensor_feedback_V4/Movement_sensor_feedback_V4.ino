///////////////////////////////////////////////////////////////////////////////////////////////////MOVEMENT/////////////////////////////////////////////////////////////////////

#define light A1

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
bool WakeUp = 0;
#include "musical_notes.h"

int speakerPin = 13; // speaker connected to digital pin 9

int startWakeSound = 0;
int lightTrigger = 0;
int touchTrigger = 0;;
int noiseTrigger = 0;
int moveTrigger = 0;

#define touchPin 10

int ledPin1 = 7; //led
int ledPin2 = 6; //led
int ledPin5 = 3; //led

int mVolt_old = 0;


#include "Wire.h" // This library allows you to communicate with I2C devices.
const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int32_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data
char tmp_str[7]; // temporary variable used in convert function
char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

unsigned long moveTime = millis();
unsigned long SleepTime = millis();
unsigned long LightTime = millis();

void setup() {

  Serial.begin(9600);

  pinMode(speakerPin, OUTPUT);

  pinMode(touchPin, INPUT);

  pinMode(ledPin1, OUTPUT); //Green
  pinMode(ledPin2, OUTPUT); //Red
  pinMode(ledPin5, OUTPUT); //Blue --> awake

  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

}

void loop() {
  if (Awake() == 1) {
    lightSensor();
    noiseSensor();
    touchSensor();
    movementSensor();
  }

}


bool Awake()
{
  //bool WakeUp = 0;
  //Serial.print( "-------------------------MOVE------------------------- ");

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7 * 2, true); // request a total of 7*2=14 registers

  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  gyro_x = Wire.read() << 8 | Wire.read();
  gyro_y = Wire.read() << 8 | Wire.read();
  gyro_z = Wire.read() << 8 | Wire.read();
  temperature = Wire.read() << 8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  accelerometer_x = Wire.read() << 8 | Wire.read();
  accelerometer_y = Wire.read() << 8 | Wire.read();
  accelerometer_z = Wire.read() << 8 | Wire.read();


  //FEEDBACK
  static unsigned long WakeDuration = 20000;  // time that led is on

  if (startWakeSound == 1)
  {
    WakeSound();
    startWakeSound = 2;
  }

  if (accelerometer_x > 1000 || accelerometer_x < -1000)
  {
    SleepTime = millis();
    digitalWrite(ledPin5, HIGH);

    if (startWakeSound == 0)
    {
      startWakeSound = 1;
    }

    WakeUp = 1;
  }

  if (accelerometer_y > 1000 || accelerometer_y < -1000)
  {
    SleepTime = millis();
    digitalWrite(ledPin5, HIGH);

    if (startWakeSound == 0)
    {
      startWakeSound = 1;
    }

    WakeUp = 1;
  }

  if (accelerometer_z > 1000 || accelerometer_z < -1000)
  {
    SleepTime = millis();
    digitalWrite(ledPin5, HIGH);

    if (startWakeSound == 0)
    {
      startWakeSound = 1;
    }

    WakeUp = 1;
  }

  if (millis() - SleepTime > WakeDuration)
  {
    digitalWrite(ledPin5, LOW);
    WakeUp = 0;
    startWakeSound = 0;
  }
  return WakeUp;
}

void lightSensor()
{
  //Serial.print( "-------------------------LIGHT------------------------- ");

  int Lvalue = analogRead(light);
  int mVolt = map(Lvalue, 0, 1023, 0, 5000);
  float volt = (double)mVolt / 1000;

  Serial.print(mVolt);
  Serial.print( "mV ");


  static unsigned long lightDuration = 10000;  // time that led is on

  // FEEDBACK
  if (mVolt - mVolt_old > 750 or mVolt - mVolt_old < -750) {
    Serial.println("PINGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG");
    digitalWrite(ledPin2, HIGH);
    LightTime = millis();
    WrongSound();
    mVolt_old = mVolt;

  }
  else
  {
    digitalWrite(ledPin2, LOW);
  }
}





void noiseSensor()
{
  //Serial.print( "-------------------------NOISE------------------------- ");

  unsigned long startMillis = millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(0);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  double volts = (peakToPeak * 5.0) / 1024;  // convert to volts

  // Serial.println(volts);

  // FEEDBACK
  if (volts > 4.5)
  {
    digitalWrite(ledPin2, HIGH);

    if (noiseTrigger == 0)
    {
      noiseTrigger = 1;
    }

    if (noiseTrigger == 1)
    {
      WrongSound();
      noiseTrigger = 2;
    }
  }
  if (volts < 4.5 && noiseTrigger == 2)
  {
    digitalWrite(ledPin2, LOW);
    noiseTrigger = 0;
  }

}



void touchSensor()
{
  //Serial.println( "-------------------------TOUCH------------------------- ");

  int touchValue = digitalRead(touchPin);
  if (touchValue == HIGH) {
    Serial.println("TOUCHED");
    digitalWrite(ledPin2, HIGH);

    if (touchTrigger == 0)
    {
      touchTrigger = 1;
    }

    if (touchTrigger == 1)
    {
      WrongSound();
      touchTrigger = 2;
    }

  }
  if (touchValue == LOW && touchTrigger == 2) {
    digitalWrite(ledPin2, LOW);
    touchTrigger = 0;
  }
}



void movementSensor()
{
  //Serial.print( "-------------------------MOVE------------------------- ");

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7 * 2, true); // request a total of 7*2=14 registers

  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  gyro_x = Wire.read() << 8 | Wire.read();
  gyro_y = Wire.read() << 8 | Wire.read();
  gyro_z = Wire.read() << 8 | Wire.read();
  temperature = Wire.read() << 8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  accelerometer_x = Wire.read() << 8 | Wire.read();
  accelerometer_y = Wire.read() << 8 | Wire.read();
  accelerometer_z = Wire.read() << 8 | Wire.read();

  // print out data
  //  Serial.print("aX = "); Serial.print(convert_int16_to_str(accelerometer_x));
  //  Serial.print(" | aY = "); Serial.print(convert_int16_to_str(accelerometer_y));
  //  Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(accelerometer_z));
  //  // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
  //  Serial.print(" | tmp = "); Serial.print(temperature / 340.00 + 36.53);
  //  Serial.print(" | gX = "); Serial.print(convert_int16_to_str(gyro_x));
  //  Serial.print(" | gY = "); Serial.print(convert_int16_to_str(gyro_y));
  //  Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(gyro_z));
  //  Serial.println();

  //FEEDBACK
  static unsigned long moveDuration = 2000;  // time that led is on

  if (moveTrigger == 0) {
    if (accelerometer_x > 30000 || accelerometer_x < -30000)
    {
      moveTime = millis();
      digitalWrite(ledPin1, HIGH);
      moveTrigger = 1;

    }

    if (accelerometer_y > 30000 || accelerometer_y < -30000)
    {
      moveTime = millis();
      digitalWrite(ledPin1, HIGH);
      moveTrigger = 1;

    }

    if (accelerometer_z > 30000 || accelerometer_z < -30000)
    {
      moveTime = millis();
      digitalWrite(ledPin1, HIGH);
      moveTrigger = 1;
    }
  }

  if (moveTrigger == 1)
  {
    GoodSound();
    moveTrigger = 2;

  }

  if (millis() - moveTime > moveDuration)
  {
    digitalWrite(ledPin1, LOW);
    moveTrigger = 0;
  }

}

//SOUND FEEDBACK
void beep (int speakerPin, float noteFrequency, long noteDuration)
{
  int x;
  // Convert the frequency to microseconds
  float microsecondsPerWave = 1000000 / noteFrequency;
  // Calculate how many HIGH/LOW cycles there are per millisecond
  float millisecondsPerCycle = 1000 / (microsecondsPerWave * 2);
  // Multiply noteDuration * number or cycles per millisecond
  float loopTime = noteDuration * millisecondsPerCycle;
  // Play the note for the calculated loopTime.
  for (x = 0; x < loopTime; x++)
  {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(microsecondsPerWave);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(microsecondsPerWave);
  }
}


void WakeSound() {
  for (int i = 100; i < 5000; i = i * 1.45) {
    beep(speakerPin, i, 60);
  }
  delay(10);
  for (int i = 100; i < 6000; i = i * 1.5) {
    beep(speakerPin, i, 20);
  }
}


void WrongSound() {
  for (int i = 6000; i > 100; i = i * 0.75) {
    beep(speakerPin, i, 20);
  }
  delay(10);
  for (int i = 5000; i < 10; i = i * 0.90) {
    beep(speakerPin, i, 60);
  }
}


void GoodSound() {
  beep(speakerPin, note_A7, 100); //A
  beep(speakerPin, note_G7, 100); //G
  beep(speakerPin, note_E7, 100); //E
  beep(speakerPin, note_C7, 100); //C
  beep(speakerPin, note_D7, 100); //D
  beep(speakerPin, note_B7, 100); //B
}
