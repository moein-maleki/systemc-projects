#ifndef __SIMUL_HPP__
#define __SIMUL_HPP__

#include "./libs.hpp"
#include "./gate.hpp"
#include "./wire.hpp"

#define VERILOG_FILE_NAME 1
#define TEST_FILE_NAME 2

#define INPUTS_LINE_STR "input"
#define OUTPUTS_LINE_STR "output"
#define WIRES_LINE_STR "wire"
#define MODULE_LINE_STR "module"
#define ENDMODULE_LINE_STR "endmodule"
#define TEST_RESULT_FILE_NAME "output.txt"

enum vf_lt{INPUTS_LINE, OUTPUTS_LINE, WIRES_LINE,
    GATE_LINE, MODULE_LINE, OTHER};

using namespace std;

class simul {
    public:
        simul(string vf_name, string tf_name);
        ~simul();
        void run();
    private:
        void create_component_list();
        vector<string> read_vf();
        vf_lt get_lt(string line);
        void dissect_line(string line, vf_lt lt);
        void create_wires(string line, vf_lt lt);
        void create_gate(string line);
        wire* find_wire(string wn);
        void determine_levels();
        gate* alloc_gate_pointer(string gid, string gname, vector<wire*> &wires);
        vector<gate*> find_gates_attached(vector<wire*> wires);
        void create_levels_verilog_file();
        string create_module_line();
        string create_inputs_line();
        string create_outputs_line();
        string create_normal_wires_line();
        string create_gates_lines_levels();
        string create_endmodule_line();
        void set_module_name(string line);
        void simulate_test_file();
        vector<string> read_test_file();
        bool is_test_input_valid(vector<string> tf);
        string eval_output_for_test(string test);
        void write_simulate_result(string result);
        void reset_circuit();
        void create_controllability_verilog_file();
        string create_gates_lines_ctrl();

        string vf_name;
        string tf_name;
        string module_name;
        vector<wire*> input_list;
        vector<wire*> output_list;
        vector<wire*> normal_wire_list;
        vector<wire*> all_wires_list;
        vector<gate*> gate_list;
};

bool comp_level(gate* g1, gate* g2);
bool comp_ctrl(gate* g1, gate* g2);
logic3 char_to_logic3(char c);


#endif