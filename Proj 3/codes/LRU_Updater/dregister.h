#include <systemc.h>

SC_MODULE(dregister){
	sc_in <sc_logic> clk, rst, loadData;
	sc_in <sc_lv<4>> dataIn;
	sc_out <sc_lv<4>> dataOut;

	SC_CTOR(dregister) {
		SC_METHOD(registering);
		sensitive << clk << rst;
	};

	void registering();
};