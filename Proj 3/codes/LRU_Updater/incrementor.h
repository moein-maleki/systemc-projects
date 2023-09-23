#include <systemc.h>

SC_MODULE(incrementor){
	sc_in <sc_logic> do_inc;
	sc_in <sc_lv<4>> dataIn;
	sc_out <sc_lv<4>> dataOut;


	SC_CTOR(incrementor) {
		SC_METHOD(adding);
		sensitive << dataIn << do_inc;
	};

	void adding();
};