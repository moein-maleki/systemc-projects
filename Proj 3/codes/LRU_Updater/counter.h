#include <systemc.h>

SC_MODULE(counter){
	sc_in <sc_logic> clk, rst, cen;
	sc_out <sc_logic> co;
	sc_out <sc_lv<4>> count;

	SC_CTOR(counter) {
		SC_METHOD(counting);
		sensitive << clk << rst;
		SC_METHOD(carrying);
		sensitive << count;
	};

	void counting();
	void carrying();
};