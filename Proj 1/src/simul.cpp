#include "../bin/simul.hpp"

simul::simul(string vf_name, string tf_name) {
    this->vf_name = vf_name;
    this->tf_name = tf_name;
}

simul::~simul() {
    for(wire* w: all_wires_list)
        free(w);
    for(gate* g: gate_list)
        free(g);
}

vector<string> simul::read_vf() {
    fstream vf(vf_name);
    vector<string> con;
    string line;
    while(getline(vf, line)) {
        if(line.size() != 1 && line[0] != '/')
            con.push_back(line);
    }
    cout << SIM_ACK << "./" << vf_name << " file opened" << endl;
    for(string l: con)
        cout << SIM_ACK << l << endl;
    vf.close();
    cout << SIM_ACK << "./" << vf_name << " file closed" << endl;
    return con;
}

vf_lt simul::get_lt(string line) {
    stringstream ss(line);
    string fw;
    ss >> fw;
    if(!fw.compare(INPUTS_LINE_STR))
        return INPUTS_LINE;
    else if(!fw.compare(OUTPUTS_LINE_STR))
        return OUTPUTS_LINE;
    else if(!fw.compare(WIRES_LINE_STR))
        return WIRES_LINE;
    else if(!fw.compare(NAND_STR) ||
        !fw.compare(AND_STR) ||
        !fw.compare(NOR_STR) ||
        !fw.compare(OR_STR) ||
        !fw.compare(XOR_STR) ||
        !fw.compare(NOT_STR))
        return GATE_LINE;
    else if(!fw.compare(MODULE_LINE_STR))
        return MODULE_LINE;
    else
        return OTHER;
}

wire* simul::find_wire(string wn) {
    for(wire* w: all_wires_list)
        if(!w->name.compare(wn)) {
            return w;
        }
    return NULL;
}

void simul::create_wires(string line, vf_lt lt) {
    vector<wire*> wires;
    string word, wire_name;
    cout << SIM_ACK << "new wires: ";
    int i;
    for(i = 0; i < line.size(); i++) {
        if(line[i] == ' ')
            break;
    }
    for(i++; i<line.size(); i++) {
        char c = line[i];
        if(c == ' ') {
            continue;
        } else if(c == ',' || c == ';') {
            wire* w = find_wire(wire_name);
            if(w == NULL) {
                cout << wire_name << " ";
                wires.push_back(new wire(wire_name));
            }
            wire_name = string();
        } else
            wire_name = wire_name + c; 
    }

    all_wires_list.insert(all_wires_list.end(), wires.begin(), wires.end());
    if(lt == INPUTS_LINE) {
        input_list.insert(input_list.end(), wires.begin(), wires.end());
        cout << "added as circuit inputs";
    }
    else if(lt == OUTPUTS_LINE) {
        output_list.insert(output_list.end(), wires.begin(), wires.end());
        cout << "added as circuit outputs";
    } else {
        normal_wire_list.insert(normal_wire_list.end(), wires.begin(), wires.end());
        cout << "added as normal wires";
    }
    cout << endl;
}

gate* simul::alloc_gate_pointer(string gid, string gname, vector<wire*> &wires) {
    for(int i = 0; i < gate_names.size(); i++) {
        string s = gate_names[i];
        if(!gid.compare(s)) {
            gt gate_type = (gt)i;
            gate* new_gate;
            switch (gate_type)
            {
            case NAND:
                new_gate = new Nand(gid, gname, wires);
                return new_gate;
            case AND:
                new_gate = new And(gid, gname, wires);
                return new_gate;
            case NOR:
                new_gate = new Nor(gid, gname, wires);
                return new_gate;
            case OR:
                new_gate = new Or(gid, gname, wires);
                return new_gate;
            case XOR:
                new_gate = new Xor(gid, gname, wires);
                return new_gate;
            case NOT:
                new_gate = new Not(gid, gname, wires);
                return new_gate;
            default:
                cout << SIM_ACK << "what the heck guys?!" << endl;
                return new_gate;
            }
        }
    }
    return NULL;
}

void simul::create_gate(string line) {
    stringstream ss(line);
    vector<wire*> wires;
    string wire_name, gid, gname;
    ss >> gid >> gname;
    int start_index = gid.size() + gname.size() + 2;
    for(int i = start_index; i < line.size(); i++) {
        char c = line[i];
        if(c == '(' || c == ' ' ||
            c == ';' || c == '\n')
            continue;
        if(c == ',' || c == ')') {
            wire* w = find_wire(wire_name);
            if(w != NULL)
                wires.push_back(w);
            else
                cout << SIM_ACK << "couldnt find wire: -" << wire_name << "-" << endl;
            wire_name = string();
        } else
            wire_name = wire_name + c;
    }
    gate_list.push_back(alloc_gate_pointer(gid, gname, wires));
}

void simul::set_module_name(string line) {
    stringstream ss(line);
    ss >> module_name >> module_name;
    cout << SIM_ACK << "module name=" << module_name << endl;
}

void simul::dissect_line(string line, vf_lt lt) {
    if(lt == INPUTS_LINE || lt == OUTPUTS_LINE || 
        lt == WIRES_LINE)
        create_wires(line, lt);
    else if(lt == GATE_LINE)
        create_gate(line);
    else if(lt == MODULE_LINE)
        set_module_name(line);
}

void simul::create_component_list() {
    cout << SIM_ACK << TASK_ONE << endl;
    vector<string> vf_lines = read_vf();
    for(string line: vf_lines) {
        vf_lt line_type = get_lt(line);
        dissect_line(line, line_type);
    }
}

vector<gate*> simul::find_gates_attached(vector<wire*> wires) {
    vector<gate*> gl;
    for(gate* g: gate_list)
        for(wire* w: wires)
            if(g->is_attached_to(w))
                gl.push_back(g);
    return gl;
}

void simul::determine_levels() {
    cout << SIM_ACK << TASK_TWO << endl;
    for(wire* w: input_list)
        w->level = 0;
    determine_all_levels(find_gates_attached(input_list));
    int done_count = 0;
    while(1) {
        for(gate* g: gate_list)
            if(g->set_level() == true)
                done_count++;
        if(done_count == gate_list.size())
            break;
    }
}

string simul::create_module_line() {
    stringstream ss;
    int count = 0;
    ss << MODULE_LINE_STR << " " << module_name
        << " (";
    vector<wire*> io_wires = input_list;
    io_wires.insert(io_wires.end(), output_list.begin(), output_list.end());
    for(wire* w: io_wires) {
        count++;
        ss << w->name;
        if(count == io_wires.size()) {
            ss << ");" << endl;
        } else {
            ss << ",";
        }
    }
    return ss.str();
}

string simul::create_inputs_line() {
    stringstream ss;
    ss << INPUTS_LINE_STR << " ";
    int count = 0;
    for(wire* w: input_list) {
        count++;
        ss << w->name;
        if(count == input_list.size()) {
            ss << ";" << endl;
        } else {
            ss << ",";
        }
    }
    return ss.str();
}

string simul::create_outputs_line() {
    stringstream ss;
    ss << OUTPUTS_LINE_STR << " ";
    int count = 0;
    for(wire* w: output_list) {
        count++;
        ss << w->name;
        if(count == output_list.size()) {
            ss << ";" << endl;
        } else {
            ss << ",";
        }
    }
    return ss.str();
}

string simul::create_normal_wires_line() {
    stringstream ss;
    ss << WIRES_LINE_STR << " ";
    int count = 0;
    for(wire* w: normal_wire_list) {
        count++;
        ss << w->name;
        if(count == normal_wire_list.size()) {
            ss << ";" << endl;
        } else {
            ss << ",";
        }
    }
    return ss.str();
}

string simul::create_gates_lines_levels() {
    stringstream ss;
    sort(gate_list.begin(), gate_list.end(), comp_level);
    for(gate* g: gate_list) {
        ss << g->level_verilog_line() << endl;
    }
    return ss.str();
}

string simul::create_endmodule_line() {
    return ENDMODULE_LINE_STR;
}

void simul::create_levels_verilog_file() {
    cout << SIM_ACK << TASK_THREE << endl;
    stringstream new_vf_name, ss;
    new_vf_name << "./level_" << module_name + ".v";
    ofstream new_vf(new_vf_name.str());
    cout << SIM_ACK << new_vf_name.str() << " file opened" << endl;
    ss << create_module_line()
        << create_inputs_line()
        << create_outputs_line()
        << create_normal_wires_line()
        << create_gates_lines_levels()
        << create_endmodule_line();
    new_vf << ss.str();
    cout << SIM_ACK << "wrote following lines to " << new_vf_name.str() << endl << ss.str() << endl;
    new_vf.close();
    cout << SIM_ACK << new_vf_name.str() << " file closed" << endl;
}

bool comp_level(gate* g1, gate* g2) { 
    return (g1->l() < g2->l());
}

bool comp_ctrl(gate* g1, gate* g2) { 
    return (g1->ctrl() > g2->ctrl());
}

bool simul::is_test_input_valid(vector<string> tf) {
    for(string line: tf) {
        if(line.size() != input_list.size())
            return false;
        for(char c: line)
            if(c != '1' && c != '0')
                return false;
    }
    return true;
}

vector<string> simul::read_test_file() {
    fstream tf(tf_name);
    string line;
    vector<string> tfc;
    while(getline(tf, line)) {
        tfc.push_back(line);
        cout << SIM_ACK << "read tf line: " << line << "" << endl;
    }
    if(is_test_input_valid(tfc)) {
        cout << SIM_ACK << "test input valid" << endl;
    } else {
        cout << SIM_ACK << "test input invalid" << endl;
        tfc = vector<string>();
    }
    tf.close();
    return tfc;
}

void simul::reset_circuit() {
    for(wire* w: all_wires_list)
        w->value = LOGIC_X;
}

logic3 char_to_logic3(char c) {
    if(c == '1') {
        return LOGIC_1;
    } else if(c == '0') {
        return LOGIC_0;
    } else {
        cout << SIM_ACK << "what in gods name" << endl;
    }
    return LOGIC_X;
}

string simul::eval_output_for_test(string test) {
    string tr;
    for(int i = 0; i < input_list.size(); i++) {
        input_list[i]->value = char_to_logic3(test[i]);
    }
    for(gate* g: gate_list) {
        g->eval_output();
    }
    for(wire* w: output_list) {
        if(w->value == LOGIC_0) {
            tr = tr + '0';
        } else if(w->value == LOGIC_1) {
            tr = tr + '1';
        } else if(w->value == LOGIC_X) {
            tr = tr + 'X';
        }
    }
    cout << SIM_ACK << "simulation result:" << endl << "i= ";
    for(int i = 0; i < input_list.size(); i++) {
        cout << input_list[i]->name << "="
            << input_list[i]->value << " ";
    }
    cout << endl << "o= ";
    for(int i = 0; i < output_list.size(); i++) {
        cout << output_list[i]->name << "="
            << output_list[i]->value << " ";
    }
    cout << endl;
    return tr; 
}

void simul::write_simulate_result(string result) {
    cout << SIM_ACK << "./" << TEST_RESULT_FILE_NAME << " file opned" << endl;
    cout << SIM_ACK << "wrote the following lines to ./"
        << TEST_RESULT_FILE_NAME << endl << result;
    ofstream trf(TEST_RESULT_FILE_NAME);
    trf << result;
    trf.close();
    cout << SIM_ACK << "./" << TEST_RESULT_FILE_NAME << " file closed" << endl;
}

void simul::simulate_test_file() {
    cout << SIM_ACK << TASK_FOUR << endl;
    vector<string> tl = read_test_file();
    stringstream ss;
    
    for(string test: tl) {
        reset_circuit();
        ss << eval_output_for_test(test) << endl;
    }
    write_simulate_result(ss.str());
}

string simul::create_gates_lines_ctrl() {
    stringstream ss;
    sort(gate_list.begin(), gate_list.end(), comp_ctrl);
    for(gate* g: gate_list) {
        ss << g->ctrl_verilog_line() << endl;
    }
    return ss.str();
}

void simul::create_controllability_verilog_file() {
    cout << SIM_ACK << TASK_FIVE << endl;
    for(wire* w: input_list) {
        w->set_ctrl1(0.5);
        cout << SIM_ACK << "wire: name=" << w->name
            << " ctrl=" << w->c1() << endl;
    }
    for(gate* g: gate_list) {
        g->set_controllability();
    }

    stringstream new_vf_name, ss;
    new_vf_name << "./ctrl_" << module_name + ".v";
    ofstream new_vf(new_vf_name.str());
    cout << SIM_ACK << new_vf_name.str() << " file opened" << endl;
    ss << create_module_line()
        << create_inputs_line()
        << create_outputs_line()
        << create_normal_wires_line()
        << create_gates_lines_ctrl()
        << create_endmodule_line();
    new_vf << ss.str();
    cout << SIM_ACK << "wrote following lines to " << new_vf_name.str() << endl << ss.str() << endl;
    new_vf.close();
    cout << SIM_ACK << new_vf_name.str() << " file closed" << endl;
}

void simul::run() {
    create_component_list();
    determine_levels();
    create_levels_verilog_file();
    simulate_test_file();
    create_controllability_verilog_file();
}