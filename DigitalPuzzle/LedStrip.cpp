#include "Arduino.h"
#include "LedStrip.h"

LedStrip::LedStrip() : LedStrip(NC, -1, -1)
{
}

LedStrip::LedStrip(WireValues type, int greenPin, int redPin)
{
	Value = type;
	GreenPin = greenPin;
	RedPin = redPin;
}

void LedStrip::CalcualtePinValues() {
	if (GreenPin == -1 || RedPin == -1) {
		return;
	}
	if (Value == WireValues::H) {
		GreenValue = HIGH;
		RedValue = LOW;
	}
	else if (Value == WireValues::L) {
		GreenValue = LOW;
		RedValue = HIGH;
	}
	else {
		GreenValue = LOW;
		RedValue = LOW;
	}
}