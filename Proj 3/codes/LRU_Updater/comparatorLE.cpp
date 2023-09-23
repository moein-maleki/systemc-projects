#include "comparatorLE.h"

void comparatorLE::compingLT() {
	while (1) {
		sc_uint<4> tempA, tempB;
		tempA = (sc_uint<4>) dataA;
		tempB = (sc_uint<4>) dataB;
		if (tempA < tempB) {
			LT = SC_LOGIC_1;
		}
		else {
			LT = SC_LOGIC_0;
		}
		wait();
	}
}

void comparatorLE::compingEQ() {
	while (1) {
		sc_uint<4> tempA, tempB;
		tempA = (sc_uint<4>) dataA;
		tempB = (sc_uint<4>) dataB;
		if (tempA == tempB) {
			EQ = SC_LOGIC_1;
		}
		else {
			EQ = SC_LOGIC_0;
		}
		wait();
	}
}