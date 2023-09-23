#include "memoryRW.h"

void memoryRW::meminit() {
	//ifstream in("memin.txt");
	//int i;
	//sc_uint<4> temp;

	mem[0] = "1000";
	mem[1] = "0010";
	mem[2] = "1101";
	mem[3] = "0100";
	mem[4] = "1111";
	mem[5] = "0101";
	mem[6] = "1010";
	mem[7] = "1100";
	mem[8] = "0000";
	mem[9] = "0110";
	mem[10] = "1110";
	mem[11] = "0011";
	mem[12] = "1001";
	mem[13] = "0111";
	mem[14] = "1011";
	mem[15] = "0001";



	//for (i = 0; i < addrSpace; i++) {
	//	in >> temp;
	//	mem[i] = (sc_lv<4>) temp;
	//	cout << "Init at: " << i << " writes: " << mem[i] << '\n';
	//}
}

void memoryRW::memwrite() {
	sc_uint<4> ad;
	while (1) {
		if (clk.event() && clk == SC_LOGIC_1)
			if (write == SC_LOGIC_1) {
				ad = addrBus;	
				mem[ad] = dataWriteBus;
		}
		wait();
	}
}

void memoryRW::memread() {
	sc_uint<4> ad;
	while (1) {
		if (read == SC_LOGIC_1) {
			ad = addrBus;
			dataReadBus = mem[ad];
		}
		wait();
	}
}

void memoryRW::memdump() {
	int dump_id = 0;
	while (1) {
		if (dump.event() && dump == SC_LOGIC_1) {
			sc_lv<4> data;
			cout << "################### memory: dumpID=" << dump_id << " ###################" << endl;
			for (int i = 0; i < addrSpace; i++) {
				data = mem[i];
				cout << i << ":\t" << data << "\n";
			}
			dump_id++;
		}
		wait();
	}
}