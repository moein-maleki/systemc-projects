#include "datapath.h"
#include "controller.h"

SC_MODULE(LRU_Updater){
	sc_in <sc_logic> clk, rst, completed;
	sc_in <sc_lv<4>> newPageBus, dataReadBus;
	sc_out <sc_lv<4>> addrBus, dataWriteBus;
	sc_out <sc_logic> read, write;
	sc_out <sc_logic> free;

	sc_signal <sc_lv<4>> addrCount, pageTagRegOut;
	sc_signal <sc_logic> selAddrCounter, selPageAddr, selZero, selInc;
	sc_signal <sc_logic> loadTag, loadPageAddr;
	sc_signal <sc_logic> co, rstCounter, cen, isTagLT;

	datapath* dp;
	controller* ctrl;

	SC_CTOR(LRU_Updater){
		dp = new datapath("datapath");
			dp->co(co); //--
			dp->clk(clk); //--
			dp->rst(rst); //--
			dp->cen(cen); //--
			dp->selInc(selInc); //--
			dp->loadTag(loadTag); //--
			dp->isTagLT(isTagLT); //--
			dp->selZero(selZero); //--
			dp->addrBus(addrBus); //--
			dp->newPageBus(newPageBus); //--
			dp->rstCounter(rstCounter); //--
			dp->dataReadBus(dataReadBus); //--
			dp->selPageAddr(selPageAddr); //--
			dp->loadPageAddr(loadPageAddr); //--
			dp->dataWriteBus(dataWriteBus); //--
			dp->pageTagRegOut(pageTagRegOut); //--
			dp->selAddrCounter(selAddrCounter); //--

		ctrl = new controller("controller");

			ctrl->co(co); //--
			ctrl->clk(clk); //--
			ctrl->cen(cen); //--
			ctrl->rst(rst); //--
			ctrl->read(read); //--
			ctrl->free(free); //--
			ctrl->write(write); //--
			ctrl->selInc(selInc); //--
			ctrl->isTagLT(isTagLT); //--
			ctrl->selZero(selZero); //--
			ctrl->loadTag(loadTag); //--
			ctrl->completed(completed); //--
			ctrl->rstCounter(rstCounter); //--
			ctrl->selPageAddr(selPageAddr); //--
			ctrl->dataReadBus(dataReadBus); //--
			ctrl->loadPageAddr(loadPageAddr); //--
			ctrl->pageTagRegOut(pageTagRegOut); //--
			ctrl->selAddrCounter(selAddrCounter); //--
	}
};

