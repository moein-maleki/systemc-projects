#include <systemc.h>

SC_MODULE(memoryRW){
	sc_in<sc_logic> clk, read, write, dump;
	sc_in<sc_lv<4>> dataWriteBus, addrBus;
	
	sc_out<sc_lv<4>> dataReadBus;

	int addrSpace;
	sc_lv <4> *mem;

	SC_CTOR(memoryRW){
		addrSpace = int(pow(2.0, 4));
		mem = new sc_lv<4>[addrSpace];

		SC_THREAD(meminit);
		SC_THREAD(memread);
		sensitive << addrBus << read;
		SC_THREAD(memwrite);
		sensitive << addrBus << clk << dataWriteBus << write;
		SC_THREAD(memdump);
		sensitive << dump;
	};

	void meminit();
	void memread();
	void memwrite();
	void memdump();
};