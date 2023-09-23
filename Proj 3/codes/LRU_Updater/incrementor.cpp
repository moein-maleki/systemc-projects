#include "incrementor.h"

void incrementor::adding() {
	if (do_inc == SC_LOGIC_1)
		dataOut = (sc_lv<4>)((sc_uint<4>) dataIn + (sc_uint<4>) 1);
	else
		dataOut = dataOut;
}