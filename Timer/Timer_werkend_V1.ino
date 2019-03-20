

#include <Adafruit_NeoPixel.h>
#include <TM1637.h>

#define CLK 3//Pins for TM1637       
#define DIO 4
#define PIN 5
#define NUMPIXELS 16

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
TM1637 tm1637(CLK, DIO);

unsigned long Watch, _micro, time = micros();
unsigned int Clock = 0, R_clock;
boolean Reset = false, Stop = false, Paused = false;
volatile boolean timeFlag = false;

int counter = 0;
int k = 0;
int R = 0;
int G = 255;
int B = 0;

int Min = 10; //minutes of the timer
int Sec = 0; //secondes of the timer + 1 sec

const int buttonPin = 2;
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;

const int buttonPin2 = 8;
int buttonState2;             // the current reading from the input pin
int lastButtonState2 = LOW;
unsigned long lastDebounceTime2 = 0;  // the last time the output pin was toggled
unsigned long debounceDelay2 = 50;

void setup()
{
  Serial.begin(115200);
  SetTimer(0, Min, Sec); // 10 seconds
  StartTimer();

  pinMode(buttonPin, INPUT);
  pinMode(buttonPin2, INPUT);

  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);

  pixels.begin();
  pixels.setBrightness(64);

  tm1637.display(0, 1);
  tm1637.display(1, 0);
  tm1637.display(2, 0);
  tm1637.display(3, 0);

  for (int i = 0; i < 16; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
  }
  pixels.show();
}

void loop()
{
  ResetTimer();
  CountDownTimer(); // run the timer
  PauseTimer();


  // this prevents the time from being constantly shown.
  if (TimeHasChanged() )
  {
    counter++;

    if (counter == 12)
    {
      pixels.setPixelColor(k % 16, pixels.Color(R, G, B));
      k++;
      pixels.show();

      if (k == 16)
      {
        R = 255;
        G = 125;
        B = 0;
      }

      if (k == 32)
      {
        R = 255;
        G = 0;
        B = 0;
      }
      //counter = 0;

    }

    if (counter == 25)
    {
      pixels.setPixelColor(k % 16, pixels.Color(R, G, B));
      k++;
      pixels.show();

      if (k == 16)
      {
        R = 255;
        G = 125;
        B = 0;
      }

      if (k == 32)
      {
        R = 255;
        G = 0;
        B = 0;
      }

      counter = 0;
    }

    Serial.print(ShowHours());
    Serial.print(":");
    Serial.print(ShowMinutes());
    Serial.print(":");
    Serial.print(ShowSeconds());
    Serial.print(":");
    Serial.print(ShowMilliSeconds());
    Serial.print(":");
    Serial.println(ShowMicroSeconds());

    tm1637.display(0, Clock / 600);
    tm1637.display(1, (Clock % 600) / 60);
    tm1637.display(2, (Clock % 60) / 10);
    tm1637.display(3, Clock % 10);
    // This DOES NOT format the time to 0:0x when seconds is less than 10.
    // if you need to format the time to standard format, use the sprintf() function.
  }
}

boolean CountDownTimer()
{
  static unsigned long duration = 1000000; // 1 second
  timeFlag = false;

  if (!Stop && !Paused) // if not Stopped or Paused, run timer
  {
    // check the time difference and see if 1 second has elapsed
    if ((_micro = micros()) - time > duration )
    {
      Clock--;
      timeFlag = true;

      if (Clock == 0) // check to see if the clock is 0
        StopTimer(); // If so, stop the timer


      // check to see if micros() has rolled over, if not,
      // then increment "time" by duration
      _micro < time ? time = _micro : time += duration;
    }
  }
  return !Stop; // return the state of the timer
}

void Alarm()
{
  for (int r = 0; r < 5; r++)
  {
    for (int j = 0; j < 16; j++) {
      pixels.setPixelColor(j, pixels.Color(255, 0, 0));
    }
    pixels.show();
    delay(300);

    for (int s = 0; s < 16; s++) {
      pixels.setPixelColor(s, pixels.Color(0, 0, 0));
    }
    pixels.show();
    delay(300);
  }

}

void ResetTimer()
{
  int reading2 = digitalRead(buttonPin2);

  if (reading2 != lastButtonState2) {
    lastDebounceTime2 = millis();
  }

  if ((millis() - lastDebounceTime2) > debounceDelay2) {
    if (reading2 != buttonState2) {
      buttonState2 = reading2;
      if (buttonState2 == HIGH) {
        SetTimer( R_clock);
        Stop = false;
        Serial.print("HALLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
        StartTimer();
        k = 0;
        counter = 0;
        R = 0;
        G = 255;
        B = 0;

        for (int f = 0; f < 16; f++) {
          pixels.setPixelColor(f, pixels.Color(255, 255, 255));
        }
        pixels.show();

        tm1637.display(0, 1);
        tm1637.display(1, 0);
        tm1637.display(2, 0);
        tm1637.display(3, 0);
      }
    }
  }
  lastButtonState2 = reading2;
}

void StartTimer()
{
  Watch = micros(); // get the initial microseconds at the start of the timer
  time = micros(); // hwd added so timer will reset if stopped and then started
  Stop = false;
  Paused = false;
}

void StopTimer()
{
  Stop = true;
  Alarm();
}

void StopTimerAt(unsigned int hours, unsigned int minutes, unsigned int seconds)
{
  if (TimeCheck(hours, minutes, seconds) )
    Stop = true;
}

void PauseTimer()
{
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        Paused = true;
        //ResetTimer();
      }
    }
  }

  lastButtonState = reading;

}


void ResumeTimer() // You can resume the timer if you ever stop it.
{
  Paused = false;
}

void SetTimer(unsigned int hours, unsigned int minutes, unsigned int seconds)
{
  // This handles invalid time overflow ie 1(H), 0(M), 120(S) -> 1, 2, 0
  unsigned int _S = (seconds / 60), _M = (minutes / 60);
  if (_S) minutes += _S;
  if (_M) hours += _M;

  Clock = (hours * 3600) + (minutes * 60) + (seconds % 60);
  R_clock = Clock;
  Stop = false;
}

void SetTimer(unsigned int seconds)
{
  // StartTimer(seconds / 3600, (seconds / 3600) / 60, seconds % 60);
  Clock = seconds;
  R_clock = Clock;
  Stop = false;
}

int ShowHours()
{
  return Clock / 3600;
}

int ShowMinutes()
{
  return (Clock / 60) % 60;
}

int ShowSeconds()
{
  return Clock % 60;
}

unsigned long ShowMilliSeconds()
{
  return (_micro - Watch) / 1000.0;
}

unsigned long ShowMicroSeconds()
{
  return _micro - Watch;
}

boolean TimeHasChanged()
{
  return timeFlag;
}

// output true if timer equals requested time
boolean TimeCheck(unsigned int hours, unsigned int minutes, unsigned int seconds)
{
  return (hours == ShowHours() && minutes == ShowMinutes() && seconds == ShowSeconds());
}

