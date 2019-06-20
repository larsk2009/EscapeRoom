#include "Arduino.h"
#include "portReader.h"

portReader::portReader() : portReader(NO_GATE) {}

portReader::portReader(GateType gate) {
	Gate = gate;
}

void portReader::CalculateOutput() {
	if ((Inputs[0]->Type == Z || Inputs[1]->Type == Z) && Gate != WIRE && Gate != INVERTER) {
		Output->Type = Z;
	}
	else if ((Inputs[0]->Type == NC || Inputs[1]->Type == NC) && Gate != WIRE && Gate != INVERTER) {
		Output->Type = Z;
	}
	else {
		switch (Gate)
		{
		case portReader::AND_GATE:
			if (Inputs[0]->Type && Inputs[1]->Type) {
				Output->Type = H;
			}
			else {
				Output->Type = L;
			}
			break;
		case portReader::OR_GATE:
			if (Inputs[0]->Type || Inputs[1]->Type) {
				Output->Type = H;
			}
			else {
				Output->Type = L;
			}
			break;
		case portReader::NAND_GATE:
			if (Inputs[0]->Type && Inputs[1]->Type) {
				Output->Type = L;
			}
			else {
				Output->Type = H;
			}
			break;
		case portReader::NOR_GATE:
			if (Inputs[0]->Type || Inputs[1]->Type) {
				Output->Type = L;
			}
			else {
				Output->Type = H;
			}
			break;
		case portReader::XOR_GATE:
			if (!Inputs[0]->Type != !Inputs[1]->Type) {
				Output->Type = H;
			}
			else {
				Output->Type = L;
			}
			break;
		case portReader::WIRE:
			if (Inputs[0]->Type == NC || Inputs[1]->Type != NC) {
				Output->Type = Z;
			}
			else {
				Output->Type = Inputs[0]->Type;
			}
			break;
		case portReader::INVERTER:
			if (Inputs[0]->Type == NC || Inputs[1]->Type != NC) {
				Output->Type = Z;
			}
			else {
        switch(Inputs[0]->Type){
          case H:
          Output->Type = L;
          break;
          case L:
          Output->Type = H;
          break;
          case Z:
          Output->Type = Z;
          break;
        }
			}
			break;
    case portReader::NO_GATE:
      Output->Type = Z;
      break;
		default:
			break;
		}
	}
}
