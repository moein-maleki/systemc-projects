#include "mux1bit_2to1.h"

void mux1bit_2to1::muxing() {
	if (selA == SC_LOGIC_1) {
		dataOut = dataA;
	}
	else if (selB == SC_LOGIC_1) {
		dataOut = dataB;
	}
	else {
		dataOut = dataOut;
	}
}