#include <systemc.h>
#include "dregister.h"
#include "incrementor.h"
#include "counter.h"
#include "mux1bit_2to1.h"

SC_MODULE(datapath){

	// Port Declaration
	sc_in <sc_logic> clk, rst;
	sc_in <sc_lv<4>> newPageBus, dataReadBus;
	sc_in <sc_logic> selPageAddr, selZero, selInc, selAddrCounter, isTagLT;
	sc_in <sc_logic> loadTag, loadPageAddr;
	sc_in <sc_logic> rstCounter, cen;
	
	sc_out <sc_logic> co;
	sc_out <sc_lv<4>> dataWriteBus, pageTagRegOut, addrBus;

	// Signal Declaration
	sc_signal <sc_lv<4>> zero, addrCount, pageAddrRegOut, incrOut;

	// Instantiation 
	mux1bit_2to1* addrMux;
	mux1bit_2to1* tagMux;
	dregister* tagRegister;
	dregister* pageAddrRegister;
	incrementor* tagIncrementor;
	counter* addrCounter;

	SC_CTOR(datapath){
		zero = "0000";

		tagRegister = new dregister("tag_register");
			tagRegister->clk(clk); //
			tagRegister->rst(rst); //
			tagRegister->loadData(loadTag); //
			tagRegister->dataIn(dataReadBus); //
			tagRegister->dataOut(pageTagRegOut); //

		pageAddrRegister = new dregister("page_register");
			pageAddrRegister->clk(clk); //
			pageAddrRegister->rst(rst); //
			pageAddrRegister->loadData(loadPageAddr); //
			pageAddrRegister->dataIn(newPageBus); //
			pageAddrRegister->dataOut(pageAddrRegOut); //

		tagIncrementor = new incrementor("tag_incrementor");
			tagIncrementor->dataIn(dataReadBus); //
			tagIncrementor->do_inc(isTagLT); //
			tagIncrementor->dataOut(incrOut); //

		addrCounter = new counter("addr_counter");
			addrCounter->clk(clk); //
			addrCounter->rst(rstCounter); //
			addrCounter->cen(cen); //
			addrCounter->co(co); //
			addrCounter->count(addrCount); //

		addrMux = new mux1bit_2to1("mux_addr");
			addrMux->dataA(addrCount); //
			addrMux->dataB(pageAddrRegOut); //
			addrMux->selA(selAddrCounter); //
			addrMux->selB(selPageAddr); //
			addrMux->dataOut(addrBus); //

		tagMux = new mux1bit_2to1("mux_tag");
			tagMux->dataA(zero); //
			tagMux->dataB(incrOut); //
			tagMux->selA(selZero); //
			tagMux->selB(selInc); //
			tagMux->dataOut(dataWriteBus); //
	}
};




