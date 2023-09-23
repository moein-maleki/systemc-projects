#include <systemc.h>

SC_MODULE(comparatorLE){
	sc_in <sc_lv<4>> dataA, dataB;
	sc_out <sc_logic> LT, EQ;

	SC_CTOR(comparatorLE) {
		SC_THREAD(compingLT);
		sensitive << dataA << dataB;
		SC_THREAD(compingEQ);
		sensitive << dataA << dataB;
	};

	void compingLT();
	void compingEQ();
};