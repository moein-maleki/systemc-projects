#include <systemc.h>
#include "comparatorLE.h"

#define IDLE 0
#define STARTING 1
#define LOAD_PAGE_ADDR 2
#define LOAD_PAGE_TAG 3
#define UPDATE_TAG 4

SC_MODULE(controller){

	sc_in  <sc_logic> clk, rst;
	sc_in  <sc_logic> completed, co;
	sc_in  <sc_lv<4>> dataReadBus, pageTagRegOut;

	sc_out <sc_logic> selAddrCounter, selPageAddr, selZero, selInc;
	sc_out <sc_logic> loadTag, loadPageAddr;
	sc_out <sc_logic> cen, rstCounter;
	sc_out <sc_logic> read, write, free, isTagLT;


	sc_signal <sc_logic> compLT, compEQ;
	sc_signal <sc_lv<3>> p_state, n_state;

	comparatorLE* tagComparator;

	SC_CTOR(controller)
	{
		tagComparator = new comparatorLE("tag_comparator");
			tagComparator->dataA(dataReadBus); //
			tagComparator->dataB(pageTagRegOut); //
			tagComparator->EQ(compEQ); //
			tagComparator->LT(compLT); //

		SC_METHOD(combOutputs);
		sensitive << compLT << compEQ << p_state;
		SC_METHOD(combNextState);
		sensitive << p_state << completed << co;
		SC_THREAD(setPresentState);
		sensitive << clk << rst;
	};

	void combOutputs();
	void combNextState();
	void setPresentState();
};

