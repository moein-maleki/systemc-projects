#include "controller.h"

void controller::combOutputs()
{
	// Inactive output values
	selAddrCounter = SC_LOGIC_0; //
	selPageAddr = SC_LOGIC_0; //
	selZero = SC_LOGIC_0; //
	selInc = SC_LOGIC_0; //
	loadTag = SC_LOGIC_0; //
	loadPageAddr = SC_LOGIC_0; //
	cen = SC_LOGIC_0; //
	rstCounter = SC_LOGIC_0; //
	read = SC_LOGIC_0; //
	write = SC_LOGIC_0; //
	free = SC_LOGIC_0; //
	isTagLT = SC_LOGIC_0; //


	switch ((sc_uint<3>) p_state){
		case IDLE:
			free = SC_LOGIC_1;
			break;
		case LOAD_PAGE_ADDR:
			loadPageAddr = SC_LOGIC_1;
			rstCounter = SC_LOGIC_1;
			break;
		case LOAD_PAGE_TAG:
			selPageAddr = SC_LOGIC_1;
			read = SC_LOGIC_1;
			loadTag = SC_LOGIC_1;
			break;
		case UPDATE_TAG:
			cen = SC_LOGIC_1;
			selAddrCounter = SC_LOGIC_1;
			read = SC_LOGIC_1;
			write = ((compEQ == SC_LOGIC_1) || (compLT == SC_LOGIC_1)) ? (SC_LOGIC_1) : (SC_LOGIC_0);
			selZero = compEQ;
			selInc = compLT;
			isTagLT = compLT;
			break;
		default:
			selAddrCounter = SC_LOGIC_0;
			selPageAddr = SC_LOGIC_0;
			selZero = SC_LOGIC_0;
			selInc = SC_LOGIC_0;
			loadTag = SC_LOGIC_0;
			loadPageAddr = SC_LOGIC_0;
			cen = SC_LOGIC_0;
			rstCounter = SC_LOGIC_0;
			read = SC_LOGIC_0;
			write = SC_LOGIC_0;
			free = SC_LOGIC_0;
			isTagLT = SC_LOGIC_0;
			break;
	}
}


void controller::combNextState(){
	n_state = (sc_lv<3>) IDLE;
	switch ((sc_uint<3>) p_state){
		case IDLE:
			n_state = (sc_lv<3>) ((completed == SC_LOGIC_1) ? (STARTING) : (IDLE));
			break;
		case STARTING:
			n_state = (sc_lv<3>) ((completed == SC_LOGIC_0) ? (LOAD_PAGE_ADDR) : (STARTING));
			break;
		case LOAD_PAGE_ADDR:
			n_state = (sc_lv<3>) LOAD_PAGE_TAG;
			break;
		case LOAD_PAGE_TAG:
			n_state = (sc_lv<3>) UPDATE_TAG;
			break;
		case UPDATE_TAG:
			n_state = (sc_lv<3>) ((co == SC_LOGIC_1) ? (IDLE) : (UPDATE_TAG));
			break;
		default:
			n_state = (sc_lv<3>) IDLE;
			break;
	}
}

void controller::setPresentState(){
	while (1){
		if (rst == SC_LOGIC_1)
			p_state = (sc_lv<3>) IDLE;
		else if (clk->event() && (clk == SC_LOGIC_1))
			p_state = n_state;
		wait();
	}
}
