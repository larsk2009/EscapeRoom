#include "Arduino.h"
#include "LedStrip.h"

LedStrip::LedStrip() : LedStrip(NC, -1, -1)
{
}

LedStrip::LedStrip(WireType type, int greenPin, int redPin)
{
	Type = type;
	GreenPin = greenPin;
	RedPin = redPin;
}

void LedStrip::CalcualtePinValues() {
	if (GreenPin == -1 || RedPin == -1) {
		return;
	}
	if (Type == WireType::H) {
		GreenValue = HIGH;
		RedValue = LOW;
	}
	else if (Type == WireType::L) {
		GreenValue = LOW;
		RedValue = HIGH;
	}
	else {
		GreenValue = LOW;
		RedValue = LOW;
	}
}