#include "Arduino.h"
#include "portReader.h"

portReader::portReader() : portReader(NO_GATE) {}

portReader::portReader(GateType gate) {
	Gate = gate;
}

void portReader::CalculateOutput() {
	if ((Inputs[0]->Value == Z || Inputs[1]->Value == Z) && Gate != WIRE && Gate != INVERTER) {
		Output->Value = Z;
	}
	else if ((Inputs[0]->Value == NC || Inputs[1]->Value == NC) && Gate != WIRE && Gate != INVERTER) {
		Output->Value = Z;
	}
	else {
		switch (Gate)
		{
		case portReader::AND_GATE:
			if (Inputs[0]->Value && Inputs[1]->Value) {
				Output->Value = H;
			}
			else {
				Output->Value = L;
			}
			break;
		case portReader::OR_GATE:
			if (Inputs[0]->Value || Inputs[1]->Value) {
				Output->Value = H;
			}
			else {
				Output->Value = L;
			}
			break;
		case portReader::NAND_GATE:
			if (Inputs[0]->Value && Inputs[1]->Value) {
				Output->Value = L;
			}
			else {
				Output->Value = H;
			}
			break;
		case portReader::NOR_GATE:
			if (Inputs[0]->Value || Inputs[1]->Value) {
				Output->Value = L;
			}
			else {
				Output->Value = H;
			}
			break;
		case portReader::XOR_GATE:
			if (!Inputs[0]->Value != !Inputs[1]->Value) {
				Output->Value = H;
			}
			else {
				Output->Value = L;
			}
			break;
		case portReader::WIRE:
			if (Inputs[0]->Value == NC || Inputs[1]->Value != NC) {
				Output->Value = Z;
			}
			else {
				Output->Value = Inputs[0]->Value;
			}
			break;
		case portReader::INVERTER:
			if (Inputs[0]->Value == NC || Inputs[1]->Value != NC) {
				Output->Value = Z;
			}
			else {
        switch(Inputs[0]->Value){
          case H:
          Output->Value = L;
          break;
          case L:
          Output->Value = H;
          break;
          case Z:
          Output->Value = Z;
          break;
        }
			}
			break;
    case portReader::NO_GATE:
      Output->Value = Z;
      break;
		default:
			break;
		}
	}
}
