#include "imageProcessing_TB.h"

void imageProcessing_TB::clocking(){
	cout << "clocking initialized" << endl;
	while (1) {
		clk = SC_LOGIC_0;
		wait(CLOCK_ACTIVE_DUR_NS, SC_NS);
		clk = SC_LOGIC_1;
		wait(CLOCK_ACTIVE_DUR_NS, SC_NS);
	}
}