#pragma once

enum WireValues {H = 1, L = 0, Z = -1, NC = -2};

class LedStrip {
public:
	LedStrip();
	LedStrip(WireValues type, int greenPin, int redPin);

	WireValues Value;
	
	int GreenPin;
	int GreenValue;

	int RedPin;
	int RedValue;

	void CalcualtePinValues();
};