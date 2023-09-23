#include "LRU_Updater_testbench.h"

void LRU_Updater_testbench::inputing(){
	completed = SC_LOGIC_0;
	newPageBus = (sc_lv<4>) "1001";
	wait(201, SC_NS);
	cout << "################### accessing page addr = 9 ###################" << endl;
	completed = SC_LOGIC_1;
	wait(201, SC_NS);
	completed = SC_LOGIC_0;

	wait(3000, SC_NS);
	newPageBus = (sc_lv<4>) "0011";
	cout << "################### accessing page addr = 3 ###################" << endl;
	completed = SC_LOGIC_1;
	wait(201, SC_NS);
	completed = SC_LOGIC_0;

	wait(3000, SC_NS);
	newPageBus = (sc_lv<4>) "0111";
	cout << "################### accessing page addr = 7 ###################" << endl;
	completed = SC_LOGIC_1;
	wait(201, SC_NS);
	completed = SC_LOGIC_0;

	/*sc_uint<4> page_addr = 0;
	srand((int)sc_time_stamp);
	while (1) {
	completed = SC_LOGIC_0;
	page_addr = (sc_uint<4>) (rand() % 16);
	newPageBus = (sc_lv<4>) page_addr;
	cout << "################### accessing page addr = " << page_addr << " ###################"<< endl;

	wait(free.posedge_event());
	wait(500, SC_NS);
	}*/
}

void LRU_Updater_testbench::clocking(){
	while (1) {
		clk = SC_LOGIC_0;
		wait(50, SC_NS);
		clk = SC_LOGIC_1;
		wait(50, SC_NS);
	}
}

void LRU_Updater_testbench::reseting(){
	rst = SC_LOGIC_0;
	wait(20, SC_NS);
	rst = SC_LOGIC_1;
	wait(101, SC_NS);
	rst = SC_LOGIC_0;
};


