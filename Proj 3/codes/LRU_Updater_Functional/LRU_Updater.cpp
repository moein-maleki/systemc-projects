#include "LRU_Updater.h"

void LRU_Updater::operation()
{
	free = SC_LOGIC_1;
	while (true)
	{
		if (rst == '1') {
			free = SC_LOGIC_1;
			read = SC_LOGIC_0;
			write = SC_LOGIC_0;
			pageTag = "0000";
			pageAddr = "0000";
			addrBus = "0000";
			dataWriteBus = "0000";
		}
		else if (clk == '1' && clk.event()) {
			while (completed != SC_LOGIC_1) {
				wait(clk.posedge_event());
			}

			free = SC_LOGIC_0;

			while (completed == SC_LOGIC_1) {
				wait(clk.posedge_event());
			}

			wait(clk.posedge_event());
			pageAddr = newPageBus;

			wait(clk.posedge_event());
			read = SC_LOGIC_1;
			addrBus = pageAddr;
			wait(1, SC_PS);
			pageTag = dataReadBus;

			for (int i = 0; i < 16; i++) {
				wait(clk.posedge_event());
				addrBus = (sc_lv<4>) i;
				wait(1, SC_PS);
				EQ_flag = (dataReadBus.read().to_uint() == pageTag.read().to_uint());
				LT_flag = (dataReadBus.read().to_uint() < pageTag.read().to_uint());
				write = (LT_flag || EQ_flag) ? SC_LOGIC_1 : SC_LOGIC_0;
				wait(1, SC_PS);
				if (EQ_flag) {
					dataWriteBus = (sc_lv<4>) "0000";
				}
				else if (LT_flag) {
					dataWriteBus = (sc_lv<4>) ((sc_uint<4>)dataReadBus + (sc_uint<4>)1);
				}
				wait(1, SC_PS);
			}

			wait(clk.posedge_event());
			free = SC_LOGIC_1;
			read = SC_LOGIC_0;
			write = SC_LOGIC_0;
			LT_flag = 0;
			EQ_flag = 0;
		}
		wait();
	}
}
