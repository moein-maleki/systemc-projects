#include "dregister.h"

void dregister::registering() {
	if (rst == SC_LOGIC_1)
		dataOut = "0000";
	else if (clk->event() && clk == SC_LOGIC_1) {
		if (loadData == SC_LOGIC_1)
			dataOut = dataIn;
		else
			dataOut = dataOut;
	}
}


