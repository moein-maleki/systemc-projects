#include "../bin/gate.hpp"

gate::gate(string d, string n, vector<wire*> w)
: name(n), output(w[0]), level(NOT_SET), obs_counted(false) {
    delay = extract_delay_from_string(d); 
    cout << SIM_ACK << "new gate: name=" << name << " delay=" << delay
        << " output=" << output->name << " inputs=";
    for(int i = 1; i < w.size(); i++) {
        cout << w[i]->name << " "; 
        inputs.push_back(w[i]);
    }
    unapplied_output = LOGIC_X;
    output->set_value(LOGIC_X);
    cout << endl;
}

event* gate::add_event(int present_time) {
    logic3 new_event = unapplied_output_value();
    return output->add_event((change)new_event, present_time + delay);
}

void gate::eval_output() {
    output->set_value(unapplied_output_value());
};

bool gate::set_level() {
    int max_level_on_input = 0;
    for(wire* w: inputs) {
        if(w->level == NOT_SET) {
            return false;
        } else if(level != NOT_SET) {
            return false;
        }
        else if(w->level > max_level_on_input)
            max_level_on_input = w->level;
    }
    level = max_level_on_input + 1;
    output->level = level;
    return true;
}

bool gate::is_attached_to(wire *w) {
    for(wire* t: inputs)
        if(t == w)
            return true;
    return false;
}

bool gate::is_driving(wire* t) {
    return output == t;
}

string gate::ctrl_verilog_line() {
    stringstream ss;
    int count = 0;
    ss << valid_gate_names[(int)gate_type] << " "
        << name << " ("
        << output->name << ", ";
    for(wire* w: inputs) {
        count++;
        if(count == inputs.size()) {
            ss << w->name << ");\t//ctrl = " << this->output->get_ctrl1();
        } else {
            ss << w->name << ", ";
        }
    }
    return ss.str();
}

string gate::level_verilog_line() {
    stringstream ss;
    int count = 0;
    ss << valid_gate_names[(int)gate_type] << " "
        << name << " ("
        << output->name << ", ";
    for(wire* w: inputs) {
        count++;
        if(count == inputs.size()) {
            ss << w->name << ");\t//level = " << level;
        } else {
            ss << w->name << ", ";
        }
    }
    return ss.str();
}

bool determine_all_levels(vector<gate*> gl) {
    for(gate* g: gl)
        if(g->set_level() == false)
            return false;
    return true;
}

void gate::set_controllability() {
    for(wire* w: inputs) {
        if(w->get_ctrl1() == NOT_SET) {
            return;
        } else if(this->output->get_ctrl1() == NOT_SET) {
            cout << name << " ctrl done." << endl;
            this->set_ctrl();
            this->remove_shells();
            break;
        }
    }
}

string gate::inputs_ord() {
    string s;
    for(wire* w: inputs)
        s += w->name + " ";
    return s;
}

bool gate::set_observability() {
    if(output->lock_obs() == false || obs_counted == true)
        return false;
    for(wire* w: inputs) {
        if(w->get_ctrl1() == NOT_SET) {
            return false;
        }
    }
    this->set_obs();
    cout << name <<  " obs done. " << endl;
    obs_counted = true;
    return true;
}

void gate::remove_shells() {
    for(wire* w: inputs) {
        w->clear_shells();
    }
    output->clear_shells();
}

void gate::print_status(int present_time) {
    cout << "# " << name << " status: "; 
    for(wire* w: inputs) {
        cout << w->name << "." << w->get_value() << " ";
    }
    cout << "old:" << output->name << "." << output->get_value() << " at " << present_time << endl;
}

Nand::Nand(string delay, string gate_name, vector<wire*> wires):
    gate(delay, gate_name, wires) {gate_type = NAND; }

And::And(string delay, string gate_name, vector<wire*> wires):
    gate(delay, gate_name, wires) {gate_type = AND; }

Nor::Nor(string delay, string gate_name, vector<wire*> wires):
    gate(delay, gate_name, wires) {gate_type = NOR; }

Or::Or(string delay, string gate_name, vector<wire*> wires):
    gate(delay, gate_name, wires) {gate_type = OR; }

Xor::Xor(string delay, string gate_name, vector<wire*> wires):
    gate(delay, gate_name, wires) {gate_type = XOR; }

Not::Not(string delay, string gate_name, vector<wire*> wires):
    gate(delay, gate_name, wires) {gate_type = NOT; }

void And::set_obs() {
    inputs[0]->add_obs(inputs[1]->get_ctrl1() * output->read_obs());
    inputs[1]->add_obs(inputs[0]->get_ctrl1() * output->read_obs());
}

void Nand::set_obs() {
    inputs[0]->add_obs(inputs[1]->get_ctrl1() * output->read_obs());
    inputs[1]->add_obs(inputs[0]->get_ctrl1() * output->read_obs());
}

void Or::set_obs() {
    inputs[0]->add_obs(inputs[1]->get_ctrl0() * output->read_obs());
    inputs[1]->add_obs(inputs[0]->get_ctrl0() * output->read_obs());
}

void Nor::set_obs() {
    inputs[0]->add_obs(inputs[1]->get_ctrl0() * output->read_obs());
    inputs[1]->add_obs(inputs[0]->get_ctrl0() * output->read_obs());
}

void Xor::set_obs() {
    inputs[0]->add_obs(output->read_obs() * max({inputs[1]->get_ctrl1(), inputs[1]->get_ctrl0()}));
    inputs[1]->add_obs(output->read_obs() * max({inputs[0]->get_ctrl1(), inputs[0]->get_ctrl0()}));
}

void Not::set_obs() {
    inputs[0]->add_obs(output->read_obs());
}

logic3 Nand::unapplied_output_value() {
    for(wire* w: inputs) {
        if(w->get_value() == LOGIC_X) {
            unapplied_output = LOGIC_X;
            return unapplied_output;
        }
    }
    for(wire* w: inputs) {
        if(w->get_value() == LOGIC_0) {
            unapplied_output = LOGIC_1;
            return unapplied_output;
        }
    }
    unapplied_output = LOGIC_0;
    return unapplied_output;
}

logic3 And::unapplied_output_value() {
    for(wire* w: inputs) {
        if(w->get_value() == LOGIC_X) {
            unapplied_output = LOGIC_X;
            return unapplied_output;
        }
    }
    for(wire* w: inputs) {
        if(w->get_value() == LOGIC_0) {
            unapplied_output = LOGIC_0;
            return unapplied_output;
        }
    }
    unapplied_output = LOGIC_1;
    return unapplied_output;
}

logic3 Nor::unapplied_output_value() {
    for(wire* w: inputs) {
        if(w->get_value() == LOGIC_X) {
            unapplied_output = LOGIC_X;
            return unapplied_output;
        }
    }
    for(wire* w: inputs) {
        if(w->get_value() == LOGIC_1) {
            unapplied_output = LOGIC_0;
            return unapplied_output;
        }
    }
    unapplied_output = LOGIC_1;
    return unapplied_output;
}

logic3 Or::unapplied_output_value() {
    for(wire* w: inputs) {
        if(w->get_value() == LOGIC_X) {
            unapplied_output = LOGIC_X;
            return unapplied_output;
        }
    }
    for(wire* w: inputs) {
        if(w->get_value() == LOGIC_1) {
            unapplied_output = LOGIC_1;
            return unapplied_output;
        }
    }
    unapplied_output = LOGIC_0;
    return unapplied_output;
}


logic3 Xor::unapplied_output_value() {
    int count = 0;
    for(wire* w: inputs) {
        if(w->get_value() == LOGIC_X) {
            unapplied_output = LOGIC_X;
            return unapplied_output;
        } else if(w->get_value() == LOGIC_1) {
            count++;
        }
    }
    if(count % 2 == 0) {
        unapplied_output = LOGIC_0;
    } else {
        unapplied_output = LOGIC_1;
    }
    return unapplied_output;
}

logic3 Not::unapplied_output_value() {
    if(inputs.size() != 1) {
        cout << SIM_ACK << "not gate with more than 1 input?" << endl;
    }
    unapplied_output =
        (inputs[0]->get_value() == LOGIC_X) ? (LOGIC_X) :
        (inputs[0]->get_value() == LOGIC_0) ? (LOGIC_1) : LOGIC_0;
    return unapplied_output;
}

void Nand::set_ctrl() {
    float mult = 1.0;
    for(wire* w: inputs) {
        if(!w->already_assumed_1()) {
            mult *= w->assume_c1();
        } else {
            mult *= 1.0;
        }
    }
    output->set_ctrl1(1 - mult);
}

void And::set_ctrl() {
    float mult = 1.0;
    for(wire* w: inputs) {
        if(!w->already_assumed_1()) {
            mult *= w->assume_c1();
        } else {
            mult *= 1.0;
        }
    }
    output->set_ctrl1(mult);
}

void Nor::set_ctrl() {
    float mult = 1.0;
    for(wire* w: inputs) {
        if(!w->already_assumed_0()) {
            mult *= w->assume_c0();
        } else {
            mult *= 1.0;
        }
    }
    output->set_ctrl1(mult);
}

void Or::set_ctrl() {
    float mult = 1.0;
    for(wire* w: inputs) {
        if(!w->already_assumed_0()) {
            mult *= w->assume_c0();
        } else {
            mult *= 1.0;
        }
    }
    output->set_ctrl1(1 - mult);
}

void Xor::set_ctrl() {
    vector<int> oc = odd_combinations();
    float sum = 0.0, tmp = 1.0;
    vector<vector<wire*>> r;
    vector<int> arr(inputs.size(), 0);
    for(int i = 0; i < inputs.size(); i++) {
        arr[i] = i;
    }
    remove(TEMP_FILE_NAME);
    for(int k: oc) {
        write_combinations_to_file(arr, k);
    }
    vector<vector<int>> vecvec = read_temp_file();
    for(vector<int> vi: vecvec) {
        this->remove_shells();
        vector<wire*> sw;
        for(int i: vi) {
            sw.push_back(inputs[i]);
        }
        vector<wire*> uw = unselected_wires(sw);
        for(wire* w: sw) {
            if(!w->already_assumed_1()) {
                tmp *= w->assume_c1();
            } else {
                tmp *= 1.0;
            }
        }
        for(wire* w: uw) {
            if(!w->already_assumed_0()) {
                tmp *= w->assume_c0();
            } else {
                tmp *= 1.0;
            }
        }
        r.push_back(sw);
        sum += tmp;
        tmp = 1.0;
    }
    output->set_ctrl1(sum);
    cout << SIM_ACK << "xor wire=" << output->name << " ctrl="<< sum << endl; 
}

vector<wire*> Xor::unselected_wires(vector<wire*> sw) {
    vector<wire*> uw;
    for(wire* w: inputs) {
        if(!w->is_in(sw))
            uw.push_back(w);
    }
    return uw;
}

vector<int> Xor::odd_combinations() {
    int i = 1, t = inputs.size();
    cout << SIM_ACK << "gate=" << this->name
        << " input_size=" << t
        << " combo=";  
    vector<int> i_vec;
    for(i; i <= t; i+=2) {
        i_vec.push_back(i);
        cout << i << " ";
    }
    cout << endl;
    return i_vec;
}

void Not::set_ctrl() {
    output->set_ctrl1(1 - inputs[0]->get_ctrl1());
}

vector<vector<int>> Xor::read_temp_file() {
    fstream file(TEMP_FILE_NAME);
    cout << SIM_ACK << "./" << TEMP_FILE_NAME << " opened" << endl;
    string line, i;
    stringstream ss;
    vector<vector<int>> r;
    while(getline(file, line)) {
        //cout << SIM_ACK << "line=" << line << endl;
        ss << line << endl;
        vector<int> t;
        while(ss >> i) {
            t.push_back(atoi(i.c_str())); 
        }
        r.push_back(t);
        ss = stringstream();
    }
    cout << SIM_ACK << "./" << TEMP_FILE_NAME << " closed" << endl;
    file.close();
    return r;
}

void Xor::write_combinations_to_file(vector<int> arr, int k)
{
    vector<int> tmp(k, 0);
    combination_util(arr, tmp, 0, arr.size() - 1, 0, k);
}
 
void Xor::combination_util(vector<int> arr, vector<int> data,
    int start, int end, int index, int r)
{
    if (index == r) {
        fstream file(TEMP_FILE_NAME, ios::out | ios::app);
        cout << SIM_ACK << "combination: ";
        for (int j = 0; j < r; j++) {
            file << data[j] << " ";
            cout << data[j] << " ";
        }
        file << endl;
        file.close();
        cout << endl;
        return;
    }
 
    for (int i = start;
        i <= end && end-i+1 >= r-index;
        i++) {
        data[index] = arr[i];
        combination_util(arr, data, i+1, end, index+1, r);
    }
}

int extract_delay_from_string(string delay_str) {
    string delay_str_pure = "";
    int delay = 0;
    for(char c: delay_str) {
        if(c == '#') continue;
        delay_str_pure += c;
    }
    int delay_str_size = delay_str_pure.size(); 
    for(int i = 0; i < delay_str_size; i++) {
        int d = delay_str_pure[i] - '0';
        delay += pow(10, delay_str_size - 1 - i) * d;
    }
    return delay;
}