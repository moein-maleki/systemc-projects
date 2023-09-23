#include "imageProcessing_TB.h"
#include "macros.h"

int sc_main(int argc, char **argv){
	sc_report_handler::set_actions(SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_,SC_DO_NOTHING);

	imageProcessing_TB TB("imageProcessing_TB");

	sc_trace_file* vcdfile;
	vcdfile = sc_create_vcd_trace_file("imageProcessing_TB");
		sc_trace(vcdfile, TB.clk, "clk");
	vcdfile->set_time_unit(1, SC_NS);
	sc_start(10 * IMAGE_SIZE * CLOCK_ACTIVE_DUR_NS * 2, SC_NS);
	cout << "in sc_main after it all went down." << endl;
	return 0;
}

