#pragma once

#include "LedStrip.h"

class portReader {
public:
	enum GateType { AND_GATE, OR_GATE, NAND_GATE, NOR_GATE, XOR_GATE, WIRE, INVERTER, NO_GATE };
	portReader();
	portReader(GateType gate);

	void CalculateOutput();

	LedStrip* Inputs[2];
	LedStrip* Output;

	enum GateType Gate;

};