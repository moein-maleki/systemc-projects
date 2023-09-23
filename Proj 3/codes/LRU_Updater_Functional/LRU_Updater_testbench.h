#include "LRU_Updater.h"
#include "memoryRW.h"

SC_MODULE(LRU_Updater_testbench){
	sc_signal <sc_logic> clk, rst, completed;
	sc_signal <sc_lv<4>> newPageBus, addrBus, dataReadBus, dataWriteBus;
	sc_signal <sc_logic> read, write, free;

	LRU_Updater* UUT;
	memoryRW* mem;

	SC_CTOR(LRU_Updater_testbench){
		UUT = new LRU_Updater("LRU_Updater");
			UUT->clk(clk); //
			UUT->rst(rst); //
			UUT->read(read); //
			UUT->free(free); //
			UUT->write(write); //
			UUT->addrBus(addrBus); //
			UUT->completed(completed); //
			UUT->newPageBus(newPageBus); //
			UUT->dataReadBus(dataReadBus); //
			UUT->dataWriteBus(dataWriteBus); //

		mem = new memoryRW("memory_unit");
			mem->clk(clk);
			mem->dump(free); //
			mem->read(read); //
			mem->write(write); //
			mem->addrBus(addrBus); //
			mem->dataReadBus(dataReadBus); //
			mem->dataWriteBus(dataWriteBus); //

		SC_THREAD(inputing);
		sensitive << free;
		SC_THREAD(reseting);
		sensitive << free;
		SC_THREAD(clocking);
	}
	void inputing();
	void reseting();
	void clocking();
};
