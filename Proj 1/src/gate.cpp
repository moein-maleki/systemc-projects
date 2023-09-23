#include "../bin/gate.hpp"

gate::gate(string gid, string gname, vector<wire*> wires) {
    for(int i = 0; i < gate_names.size(); i++) {
        string s = gate_names[i];
        if(!gid.compare(s)) {
            cout << SIM_ACK << "new gate type=" << s;
            gate_type = (gt)i;
        }
    }
    level = NOT_SET;
    name = gname;
    output = wires[0];
    cout << " name=" << name << " output=" << output->name << " inputs=";
    for(int i = 1; i < wires.size(); i++) {
        cout << wires[i]->name << " "; 
        inputs.push_back(wires[i]);
    }
    cout << endl;
}

bool gate::set_level() {
    int max = 0;
    for(wire* w: inputs) {
        if(w->level == NOT_SET || level != NOT_SET) {
            //cout << SIM_ACK << "wire: name=" << w->name << " level not set." << endl;
            return false;
        }
        else if(w->level > max)
            max = w->level;
    }
    level = max + 1;
    output->level = level;
    cout << SIM_ACK << "gate: name=" << name << " level=" << level << endl;
    return true;
}

bool gate::is_attached_to(wire *w) {
    for(wire* t: inputs)
        if(t == w)
            return true;
    return false;
}

string gate::ctrl_verilog_line() {
    stringstream ss;
    int count = 0;
    ss << gate_names[(int)gate_type] << " "
        << name << " ("
        << output->name << ", ";
    for(wire* w: inputs) {
        count++;
        if(count == inputs.size()) {
            ss << w->name << ");\t//ctrl = " << this->output->c1();
        } else {
            ss << w->name << ", ";
        }
    }
    return ss.str();
}

string gate::level_verilog_line() {
    stringstream ss;
    int count = 0;
    ss << gate_names[(int)gate_type] << " "
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
        if(w->c1() == NOT_SET) {
            cout << SIM_ACK << "gate=" << this->name
                << " wire="
                << w->name << " ctrl="
                << w->c1() << endl;
            return;
        } else if(this->output->c1() == NOT_SET) {
            this->set_ctrl();
            this->remove_shells();
        } else {}
    }
}

void gate::remove_shells() {
    for(wire* w: inputs) {
        w->clear_shells();
    }
    output->clear_shells();
}

Nand::Nand(string gid, string gname, vector<wire*> wires):
    gate(gid, gname, wires) {}

And::And(string gid, string gname, vector<wire*> wires):
    gate(gid, gname, wires) {}

Nor::Nor(string gid, string gname, vector<wire*> wires):
    gate(gid, gname, wires) {}

Or::Or(string gid, string gname, vector<wire*> wires):
    gate(gid, gname, wires) {}

Xor::Xor(string gid, string gname, vector<wire*> wires):
    gate(gid, gname, wires) {}

Not::Not(string gid, string gname, vector<wire*> wires):
    gate(gid, gname, wires) {}

void Nand::eval_output() {
    for(wire* w: inputs) {
        if(w->value == LOGIC_X) {
            output->value = LOGIC_X;
            return;
        } else if(w->value == LOGIC_0) {
            output->value = LOGIC_1;
            return;
        }
    }
    output->value = LOGIC_0;
}

void And::eval_output() {

    for(wire* w: inputs) {
        if(w->value == LOGIC_X) {
            output->value = LOGIC_X;
            return;
        } else if(w->value == LOGIC_0) {
            output->value = LOGIC_0;
            return;
        } 
    }
    output->value = LOGIC_1;
}

void Nor::eval_output() {
    for(wire* w: inputs) {
        if(w->value == LOGIC_X) {
            output->value = LOGIC_X;
            return;
        } else if(w->value == LOGIC_1) {
            output->value = LOGIC_0;
            return;
        }
    }
    output->value = LOGIC_1;
}

void Or::eval_output() {
    for(wire* w: inputs) {
        if(w->value == LOGIC_X) {
            output->value = LOGIC_X;
            return;
        } else if(w->value == LOGIC_1) {
            output->value = LOGIC_1;
            return;
        }
    }
    output->value = LOGIC_0;
}

void Xor::eval_output() {
    int count = 0;
    for(wire* w: inputs) {
        if(w->value == LOGIC_X) {
            output->value = LOGIC_X;
            return;
        } else if(w->value == LOGIC_1) {
            count++;
        }
    }
    if(count % 2 == 0) {
        output->value = LOGIC_0;
    } else {
        output->value = LOGIC_1;
    }
}

void Not::eval_output() {
    if(inputs.size() != 1) {
        cout << SIM_ACK << "not gate with more than 1 input?" << endl;
    }
    output->value =
        (inputs[0]->value == LOGIC_1) ? (LOGIC_0) :
        (inputs[0]->value == LOGIC_0) ? (LOGIC_1) : LOGIC_X;
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
        //cout << "new scenario" << endl;
        vector<wire*> sw;
        for(int i: vi) {
            sw.push_back(inputs[i]);
        }
        vector<wire*> uw = unselected_wires(sw);
        for(wire* w: sw) {
            if(!w->already_assumed_1()) {
                //cout << "wire=" << w->name << " ctrl1= " << w->c1() << endl; 
                tmp *= w->assume_c1();
            } else {
                tmp *= 1.0;
            }
        }
        //cout << "after assuming 1s tmp=" << tmp << endl;
        for(wire* w: uw) {
            if(!w->already_assumed_0()) {
                //cout << "wire=" << w->name << " ctrl1= " << w->c0() << endl; 
                tmp *= w->assume_c0();
            } else {
                tmp *= 1.0;
            }
        }
        //cout << "after assuming 0s tmp=" << tmp << endl;
        r.push_back(sw);
        sum += tmp;
        //cout << "after all sum=" << sum << endl;
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
    output->set_ctrl1(1 - inputs[0]->c1());
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