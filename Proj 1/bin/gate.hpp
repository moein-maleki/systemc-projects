#ifndef __GATE_HPP__
#define __GATE_HPP__

#include "./libs.hpp"
#include "./wire.hpp"

#define NAND_STR "nand"
#define AND_STR "and"
#define OR_STR "or"
#define NOR_STR "nor"
#define XOR_STR "xor"
#define NOT_STR "not"
#define TEMP_FILE_NAME "tmp.txt"

enum gt{NAND, AND, NOR, OR, XOR, NOT};

static vector<std::string> gate_names =
    {NAND_STR, AND_STR, NOR_STR,
    OR_STR, XOR_STR, NOT_STR};

using namespace std;

class gate {
    public:
        gate(string gid, string gname, vector<wire*> wires);
        string n() {return name; }
        int l() {return level; }
        bool set_level();
        bool is_attached_to(wire *w);
        string level_verilog_line();
        string ctrl_verilog_line();
        virtual void eval_output() = 0;
        void set_controllability();
        virtual void set_ctrl() = 0;
        float ctrl() {return output->c1(); }
        void remove_shells();
    private:
        friend class Nand;
        friend class And;
        friend class Nor;
        friend class Or;
        friend class Xor;
        friend class Not;

        int level;
        string name;
        gt gate_type;
        wire* output;
        vector<wire*> inputs;
};

class Nand: public gate {
    public:
        Nand(string gid, string gname, vector<wire*> wires);
        virtual void eval_output();
        virtual void set_ctrl();
};

class And: public gate {
    public:
        And(string gid, string gname, vector<wire*> wires);
        virtual void eval_output();
        virtual void set_ctrl();
};

class Nor: public gate {
    public:
        Nor(string gid, string gname, vector<wire*> wires);
        virtual void eval_output();
        virtual void set_ctrl();
};

class Or: public gate {
    public:
        Or(string gid, string gname, vector<wire*> wires);
        virtual void eval_output();
        virtual void set_ctrl();
};

class Xor: public gate {
    public:
        Xor(string gid, string gname, vector<wire*> wires);
        virtual void eval_output();
        virtual void set_ctrl();
    private:
        vector<int> odd_combinations();
        vector<wire*> unselected_wires(vector<wire*> sw);
        void write_combinations_to_file(vector<int> arr, int k);
        void combination_util(vector<int> arr, vector<int> data,
            int start, int end, int index, int r);
        vector<vector<int>> read_temp_file();

};

class Not: public gate {
    public:
        Not(string gid, string gname, vector<wire*> wires);
        virtual void eval_output();
        virtual void set_ctrl();
};


bool determine_all_levels(vector<gate*> gl);

#endif