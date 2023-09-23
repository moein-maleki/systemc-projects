#include <systemc.h>

SC_MODULE(LRU_Updater)
{
	sc_in<sc_logic> clk, rst, completed;
	sc_in<sc_lv<4>> newPageBus, dataReadBus;

	sc_out<sc_logic> free, write, read;
	sc_out<sc_lv<4>> dataWriteBus, addrBus;

	sc_signal<sc_lv<4>> pageAddr, pageTag;
	bool LT_flag, EQ_flag;

	SC_CTOR(LRU_Updater)
	{
		SC_THREAD(operation)
			sensitive << clk << completed << rst;
	}
	void operation();
};