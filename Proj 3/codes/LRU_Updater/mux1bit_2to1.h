#include <systemc.h>

SC_MODULE(mux1bit_2to1){
	sc_in <sc_logic> selA, selB;
	sc_in <sc_lv<4>> dataA, dataB;
	sc_out <sc_lv<4>> dataOut;

	SC_CTOR(mux1bit_2to1) {
		SC_METHOD(muxing);
		sensitive << dataA << dataB << selA << selB;
	};

	void muxing();
};