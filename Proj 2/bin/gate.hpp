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

static vector<std::string> valid_gate_names =
    {NAND_STR, AND_STR, NOR_STR,
    OR_STR, XOR_STR, NOT_STR};

using namespace std;

class gate_tree;
class gate {
    public:
        gate(string, string, vector<wire*>);
        event* add_event(int);
        string output_name() {return output->name; }
        logic3 output_value() {return output->get_value(); }
        string n() {return name; }
        int l() {return level; }
        int d() {return delay; }
        bool set_level();
        bool set_observability();
        void set_controllability();
        virtual void set_obs() = 0;
        string inputs_ord();
        bool is_attached_to(wire *w);
        string level_verilog_line();
        string ctrl_verilog_line();
        void eval_output();
        virtual logic3 unapplied_output_value() = 0;
        virtual void set_ctrl() = 0;
        float ctrl() {return output->get_ctrl1(); }
        void remove_shells();
        bool is_driving(wire*);
    private:
        void print_status(int present_time);

        friend class gate_tree;
        friend class Nand;
        friend class And;
        friend class Nor;
        friend class Or;
        friend class Xor;
        friend class Not;

        int level;
        int delay;
        bool obs_counted;
        string name;
        gt gate_type;
        wire* output;
        logic3 unapplied_output;
        vector<wire*> inputs;
};

class Nand: public gate {
    public:
        Nand(string, string, vector<wire*>);
        virtual logic3 unapplied_output_value();
        virtual void set_obs();
        virtual void set_ctrl();
};

class And: public gate {
    public:
        And(string, string, vector<wire*>);
        virtual logic3 unapplied_output_value();
        virtual void set_obs();
        virtual void set_ctrl();
};

class Nor: public gate {
    public:
        Nor(string, string, vector<wire*>);
        virtual logic3 unapplied_output_value();
        virtual void set_obs();
        virtual void set_ctrl();
};

class Or: public gate {
    public:
        Or(string, string, vector<wire*>);
        virtual logic3 unapplied_output_value();
        virtual void set_obs();
        virtual void set_ctrl();
};

class Xor: public gate {
    public:
        Xor(string, string, vector<wire*>);
        virtual logic3 unapplied_output_value();
        virtual void set_obs();
        virtual void set_ctrl();
    private:
        vector<int> odd_combinations();
        vector<wire*> unselected_wires(vector<wire*>);
        void write_combinations_to_file(vector<int>, int );
        void combination_util(vector<int>, vector<int>, int, int, int, int);
        vector<vector<int>> read_temp_file();

};

class Not: public gate {
    public:
        Not(string, string, vector<wire*>);
        virtual logic3 unapplied_output_value();
        virtual void set_obs();
        virtual void set_ctrl();
};

bool determine_all_levels(vector<gate*> gl);
int extract_delay_from_string(string);

#endif