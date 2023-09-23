#include "clockGenerator.h"

void clockGenerator::clocking() {
    double halfPeriod = (0.5/REGISTER_CLOCK_FREQUENCY_MHZ);
	while(1) {
		out = SC_LOGIC_1;
		wait(halfPeriod, SC_US);
		out = SC_LOGIC_0;
		wait(halfPeriod, SC_US);
	}
}