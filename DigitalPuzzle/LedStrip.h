#pragma once

enum WireType {H = 1, L = 0, Z = -1, NC = -2};

class LedStrip {
public:
	LedStrip();
	LedStrip(WireType type, int greenPin, int redPin);

	WireType Type;
	
	int GreenPin;
	int GreenValue;

	int RedPin;
	int RedValue;

	void CalcualtePinValues();
};