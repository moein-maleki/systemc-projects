#ifndef TESTBENCH_HH
#define TESTBENCH_HH

#include <systemc.h>
#include "imageProcessing.h"

SC_MODULE(imageProcessing_TB){
	sc_signal <sc_logic> clk;
	
	imageProcessing* UUT;

	SC_CTOR(imageProcessing_TB){
		UUT = new imageProcessing("imageProcessing_UUT");
			UUT->clk(clk);

		SC_THREAD(clocking);
	}
	void clocking();
};

#endif