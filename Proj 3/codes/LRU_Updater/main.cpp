#include "LRU_Updater_testbench.h"

int sc_main(int argc, char **argv){
	LRU_Updater_testbench TB("LRU_Updater_testbench");

	sc_trace_file* vcdfile;
	vcdfile = sc_create_vcd_trace_file("LRU_Updater_testbench");
		sc_trace(vcdfile, TB.UUT->clk, "clk");
		sc_trace(vcdfile, TB.UUT->rst, "rst");
		sc_trace(vcdfile, TB.UUT->completed, "completed");
		sc_trace(vcdfile, TB.UUT->newPageBus, "newPageBus");
		sc_trace(vcdfile, TB.UUT->dataReadBus, "dataReadBus");
		sc_trace(vcdfile, TB.UUT->addrBus, "addrBus");
		sc_trace(vcdfile, TB.UUT->dataWriteBus, "dataWriteBus");
		sc_trace(vcdfile, TB.UUT->read, "read");
		sc_trace(vcdfile, TB.UUT->write, "write");
		sc_trace(vcdfile, TB.UUT->free, "free");
		sc_trace(vcdfile, TB.UUT->dp->addrCount, "addrCount");
		sc_trace(vcdfile, TB.UUT->pageTagRegOut, "pageTagRegOut");
		sc_trace(vcdfile, TB.UUT->selAddrCounter, "selAddrCounter");
		sc_trace(vcdfile, TB.UUT->selPageAddr, "selPageAddr");
		sc_trace(vcdfile, TB.UUT->selZero, "selZero");
		sc_trace(vcdfile, TB.UUT->selInc, "selInc");
		sc_trace(vcdfile, TB.UUT->dp->pageAddrRegOut, "pageAddrRegOut");
		sc_trace(vcdfile, TB.UUT->loadTag, "loadTag");
		sc_trace(vcdfile, TB.UUT->loadPageAddr, "loadPageAddr");
		sc_trace(vcdfile, TB.UUT->co, "co");
		sc_trace(vcdfile, TB.UUT->rstCounter, "rstCounter");
		sc_trace(vcdfile, TB.UUT->cen, "cen");
		sc_trace(vcdfile, TB.UUT->isTagLT, "isTagLT");
		sc_trace(vcdfile, TB.UUT->ctrl->p_state, "p_state");
		sc_trace(vcdfile, TB.UUT->ctrl->n_state, "n_state");
		sc_trace(vcdfile, TB.UUT->ctrl->compEQ, "compEQ");
		sc_trace(vcdfile, TB.UUT->ctrl->compLT, "compLT");
		sc_trace(vcdfile, TB.UUT->ctrl->tagComparator->dataA, "comp_mem_TAGS");
		sc_trace(vcdfile, TB.UUT->ctrl->tagComparator->dataB, "comp_page_TAGS");


	sc_start(50000, SC_NS);
	return 0;
}

