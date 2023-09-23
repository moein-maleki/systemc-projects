#include "../bin/simul.hpp"

simul::simul(string vf_name, string tf_name, int r_time) {
    this->vf_name = vf_name;
    this->tf_name = tf_name;
    time_resolution = r_time;
    present_time = 0;
}

simul::~simul() {
    for(wire* w: all_wires_list)
        free(w);
    for(gate* g: gate_list)
        free(g);
    for(event* e: global_event_vector)
        free(e);
}

vector<string> simul::read_vf() {
    fstream vf(vf_name);
    vector<string> lv;
    string line;
    while(getline(vf, line)) {
        if(line.size() != 1 && line[0] != '/')
            lv.push_back(line);
    }
    for(string l: lv)
        if(l.size() > 2 && l[0] != '/')
            cout << SIM_ACK << l << endl;
    vf.close();
    return lv;
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

gate* simul::create_gate_pointer(gt gate_type, string delay, string gate_name, vector<wire*> wires) {
    gate* new_gate;
    switch (gate_type)
    {
    case NAND:
        new_gate = new Nand(delay, gate_name, wires);
        return new_gate;
    case AND:
        new_gate = new And(delay, gate_name, wires);
        return new_gate;
    case NOR:
        new_gate = new Nor(delay, gate_name, wires);
        return new_gate;
    case OR:
        new_gate = new Or(delay, gate_name, wires);
        return new_gate;
    case XOR:
        new_gate = new Xor(delay, gate_name, wires);
        return new_gate;
    case NOT:
        new_gate = new Not(delay, gate_name, wires);
        return new_gate;
    default:
        cout << SIM_ACK << "what the heck guys?!" << endl;
        return new_gate;
    }
}

gate* simul::alloc_gate_pointer(string gate_type_identifier,
    string delay, string gate_name, vector<wire*> wires) {
    for(int i = 0; i < valid_gate_names.size(); i++) {
        string s = valid_gate_names[i];
        if(!gate_type_identifier.compare(s)) {
            gt gate_type = (gt)i;
            return create_gate_pointer(gate_type, delay, gate_name, wires);
        }
    }
    return NULL;
}

void simul::create_gate(string gate_line) {
    stringstream ss(gate_line);
    vector<wire*> wires;
    string wire_name, gate_type, gate_name, gate_delay;
    ss >> gate_type >> gate_delay >> gate_name;
    int start_index = gate_type.size() + gate_delay.size()
        + gate_name.size() + 2;
    for(int i = start_index; i < gate_line.size(); i++) {
        char c = gate_line[i];
        if(c == '(' || c == ' ' ||
            c == ';' || c == '\n')
            continue;
        if(c == ',' || c == ')') {
            wire* w = find_wire(wire_name);
            if(w != NULL) {
                wires.push_back(w);
            } else {
                cout << SIM_ACK << "couldnt find wire: -" << wire_name << "-" << endl;
            }
            wire_name = string();
        } else {
            wire_name = wire_name + c;
        }
    }
    gate_list.push_back(alloc_gate_pointer(gate_type, gate_delay, gate_name, wires));
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
    cout << SIM_ACK << TASK_1 << endl;

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
    stringstream new_vf_name, ss;
    new_vf_name << "./level_" << module_name + ".v";
    ofstream new_vf(new_vf_name.str());
    ss << create_module_line()
        << create_inputs_line()
        << create_outputs_line()
        << create_normal_wires_line()
        << create_gates_lines_levels()
        << create_endmodule_line();
    new_vf << ss.str();
    new_vf.close();
}

bool comp_level_inc(gate* g1, gate* g2) { 
    return (g1->l() > g2->l());
}

bool comp_obs(wire* w1, wire* w2) { 
    return (w1->read_obs() > w2->read_obs());
}

bool comp_level(gate* g1, gate* g2) { 
    return (g1->l() < g2->l());
}

bool comp_time(event* e1, event* e2) {
    return (e1->time_e < e2->time_e);
}

bool comp_ctrl(wire* w1, wire* w2) { 
    return (w1->get_ctrl1() > w2->get_ctrl1());
}

bool simul::is_test_input_valid(vector<string> tf) {
    for(string line: tf) {
        if(line.size() != input_list.size()) {
            cout << SIM_ACK << "line.size() != input_list.size() --> " << line << endl;
            return false;
        }
        for(char c: line)
            if(c != '1' && c != '0') {
                cout << SIM_ACK << "c != '1' && c != '0' --> " << line << " // " << c << endl;
                return false;
            }
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
    vector<string> test_values;
    for(string l: tfc) {
        stringstream ss(l);
        string test_delay, test_value;
        ss >> test_delay >> test_value;
        test_values.push_back(test_value);
    }

    if(is_test_input_valid(test_values)) {
        cout << SIM_ACK << "test input valid" << endl;
    } else {
        cout << SIM_ACK << "test input invalid" << endl;
        tfc = vector<string>();
    }
    tf.close();
    return tfc;
}

void simul::reset_circuit() {
    for(wire* w: all_wires_list) {
        event* new_event = w->add_event(TO_X, 0);
        if(new_event != NULL)
            global_event_vector.push_back(new_event);
        w->set_value(LOGIC_X);
    }
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

bool int_is_unique(vector<int> l, int t) {
    for(int i: l) {
        if(i == t) {
            return false;
        }
    }
    return true;
}

string simul::outputs_events(vector<event*> event_list) {
    stringstream line("");
    int prev_event_time = 0;
    vector<int> written_times;
    for(event* e: event_list) {
        if(int_is_unique(written_times, e->time_e)) {
            written_times.push_back(e->time_e);
            line << "#" << e->time_e - prev_event_time << " ";
            for(wire* w: output_list) {
                line << c2s(w->last_event_before(e->time_e));
            }
            line << " //"; 
            for(wire* w: output_list) {
                line << w->name << " ";
            }
            line << endl;
            prev_event_time = e->time_e;
        }
        
    }
    return line.str();
}

void simul::write_simulation_results() {
    vector<event*> el;
    for(wire* w: output_list) {
        event* new_event = new event(w, TO_X, 0);
        el.push_back(new_event);
        global_event_vector.push_back(new_event);
        for(event* e: w->event_vector) {
            el.push_back(e);
        }
    }
    sort(el.begin(), el.end(), comp_time);
    string oe = outputs_events(el);
    
    cout << SIM_ACK << "wrote the following lines to ./" << TEST_RESULT_FILE_NAME << endl;
    cout << oe;

    ofstream trf(TEST_RESULT_FILE_NAME);
    trf << oe;
    trf.close();
}

void simul::initialize_event_timeline() {
    vector<string> tl = read_test_file();
    cout << SIM_ACK << "events created by the input test file: " << tf_name << endl;
    for(string test: tl) {
        stringstream ss(test);
        string test_value, test_delay_s;
        ss >> test_delay_s >> test_value;
        int test_delay = extract_delay_from_string(test_delay_s);
        present_time += test_delay;
        for(int i = 0; i < input_list.size(); i++) {
            change new_change = convert_to_change(test_value[i]);
            event* new_event = input_list[i]->add_event(new_change, present_time);
            if(new_event != NULL) {
                global_event_vector.push_back(new_event);
            }
        }
    }
}

int simul::max_event_time() {
    int max_event_time = 0;
    for(event* e: global_event_vector) {
        if(e->time_e > max_event_time) {
            max_event_time = e->time_e;
        }
    }
    cout << "max event time=" << max_event_time << endl;
    return max_event_time;
}

gate* simul::find_driving_gate(wire* t) {
    for(gate* g: gate_list) {
        if(g->is_driving(t))
            return g;
    }
    return NULL;
}

bool simul::wire_is_primary_input(wire* w) {
    for(wire* t: input_list) {
        if(t == w) {
            return true;
        }
    }
    return false;
}

void simul::apply_all_events_at_present_time() {
    cout << "applying events at " << present_time << endl;
    for(event* e: global_event_vector) {
        if(e->time_e == present_time) {
            if(wire_is_primary_input(e->wire_e)) {
                cout << "primary input sat value: " << e->wire_e->name << " v=" << e->change_e << endl; 
                e->wire_e->set_value((logic3) e->change_e);
            } else {
                gate* g = find_driving_gate(e->wire_e);
                g->eval_output();
                cout << g->n() << " drove value: " << e->wire_e->name << " v=" << e->change_e << endl; 
            }
        }
    }
}

bool gate_is_unique(gate* t, vector<gate*> gate_list) {
    for(gate* g: gate_list)
        if(g == t)
            return false;
    return true;
}

vector<gate*> simul::eventful_gates() {
    vector<gate*> eventfuls;
    for(event* e: global_event_vector) {
        if(e->time_e == present_time) {
            vector<gate*> new_gates = find_gates_attached(vector<wire*>({e->wire_e}));
            for(gate* g: new_gates) {
                if(gate_is_unique(g, eventfuls)) {
                    eventfuls.push_back(g);
                }
            }
        }
    }
    return eventfuls;
}

void simul::schedule_events_on(vector<gate*> eventfuls) {
    for(gate* g: eventfuls) {
        event* new_event = g->add_event(present_time);
        if(new_event != NULL) {
            global_event_vector.push_back(new_event);
        }
    }
}

void simul::move_simulation_forward_by_resolution() {
    for(int t = 0; t < time_resolution; t++) {
        apply_all_events_at_present_time();
        schedule_events_on(eventful_gates());
        present_time += 1;
    }
}

void simul::print_all_events() {
    cout << "========================" << endl;
    for(wire* w: all_wires_list) {
        cout << w->name << " ";
        for(event* e: w->event_vector)
            cout << "(c=" << e->change_e << ",t=" << e->time_e << ") ";
        cout << endl;
    }
    cout << "========================" << endl;
}

void simul::simulate_test_file() {
    cout << SIM_ACK << TASK_2 << endl;

    reset_circuit();
    initialize_event_timeline();
    cout << SIM_ACK << "circuit simulation started" << endl;
    present_time = 0;
    while(present_time <= max_event_time()) {
        move_simulation_forward_by_resolution();
    }

    print_all_events();

    write_simulation_results();
}

string simul::create_gates_lines_ctrl() {
    stringstream ss;
    for(gate* g: gate_list) {
        ss << g->ctrl_verilog_line() << endl;
    }
    return ss.str();
}

void simul::determine_observability() {
    cout << SIM_ACK << TASK_4 << endl;

    for(wire* w: output_list) {
        w->add_obs(1.0);
    }

    sort(gate_list.begin(), gate_list.end(), comp_level_inc);

    for(gate* g: gate_list) {
        g->set_observability();
    }

    for(wire* w: input_list)
        w->lock_obs();
    
    cout << SIM_ACK << "observability results:" << endl;
    sort(all_wires_list.begin(), all_wires_list.end(), comp_obs);
    for(wire* w: all_wires_list) {
        cout << SIM_ACK << w->name << " -> " << w->read_obs() << endl;
    }
}

void simul::determine_controllability() {
    cout << SIM_ACK << TASK_3 << endl;

    determine_levels();
    sort(gate_list.begin(), gate_list.end(), comp_level);
    for(wire* w: input_list) {
        w->set_ctrl1(0.5);
    }
    for(gate* g: gate_list) {
        g->set_controllability();
    }

    cout << SIM_ACK << "controllability results:" << endl;
    sort(all_wires_list.begin(), all_wires_list.end(), comp_ctrl);
    for(wire* w: all_wires_list) {
        cout << SIM_ACK << w->name << " -> " << w->get_ctrl1() << endl;
    }
}

void simul::find_critical_path() {
    cout << SIM_ACK << TASK_5 << endl;

    gate_tree tree(gate_list, output_list, normal_wire_list);
    vector<gate*> critical_path = tree.longest_path();
    int critical_delay = 0;
    cout << SIM_ACK << "critical path:" << endl
        << SIM_ACK << "trigger: one of the following wires: " << critical_path[critical_path.size() - 1]->inputs_ord() << endl;
    for(int i = critical_path.size() - 1; i >= 0; i--) {
        gate* g = critical_path[i];
        critical_delay += g->d();
        cout << SIM_ACK << g->n() << "(" << g->d() << ") -> " << g->output_name() << endl;
    }
    cout << SIM_ACK << "critical_delay: " << critical_delay << endl;
}

void simul::run() {
    create_component_list();
    simulate_test_file();
    determine_controllability();
    determine_observability();
    find_critical_path();
}