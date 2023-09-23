#include "LRU_Updater_testbench.h"

int sc_main(int argc, char **argv){
	LRU_Updater_testbench TB("LRU_Updater_testbench");

	sc_trace_file* vcdfile;
	vcdfile = sc_create_vcd_trace_file("LRU_Updater_testbench");
	sc_trace(vcdfile, TB.clk, "clk");
	sc_trace(vcdfile, TB.rst, "rst");
	sc_trace(vcdfile, TB.completed, "completed");
	sc_trace(vcdfile, TB.newPageBus, "newPageBus");
	sc_trace(vcdfile, TB.dataReadBus, "dataReadBus");
	sc_trace(vcdfile, TB.addrBus, "addrBus");
	sc_trace(vcdfile, TB.dataWriteBus, "dataWriteBus");
	sc_trace(vcdfile, TB.read, "read");
	sc_trace(vcdfile, TB.write, "write");
	sc_trace(vcdfile, TB.free, "free");
	sc_trace(vcdfile, TB.UUT->pageAddr, "pageAddrReg");
	sc_trace(vcdfile, TB.UUT->pageTag, "pageTagReg");
	sc_trace(vcdfile, TB.UUT->LT_flag, "LT_flag");
	sc_trace(vcdfile, TB.UUT->EQ_flag, "EQ_flag");

	sc_start(15000, SC_NS);
	return 0;
}

