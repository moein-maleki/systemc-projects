#include "counter.h"

void counter::counting() {
	if (rst == SC_LOGIC_1){
		count = "0000";
	}
	else if (clk->event() && clk == SC_LOGIC_1)	{
		if (cen == '1')
			count = (sc_lv<4>)(count->read().to_uint() + (sc_uint<4>) 1);
		else
			count = count;
	}
}

void counter::carrying() {
	if (count.read().to_uint() == 15)
		co = SC_LOGIC_1;
	else
		co = SC_LOGIC_0;
}
