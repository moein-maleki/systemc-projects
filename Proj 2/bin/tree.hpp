#ifndef __TREE_HPP__
#define __TREE_HPP__

#include "./libs.hpp"
#include "./gate.hpp"
#include "./wire.hpp"

class gate_node {
    public:
        gate_node(gate* v);
        gate_node(gate* v, vector<gate_node*> c, gate_node* p);

        void add_child(gate_node* c) {children.push_back(c); }
        gate* value;
        gate_node* parent;
        vector<gate_node*> children;
    private:
        
};

class gate_tree {
    public:
        gate_tree(vector<gate*>, vector<wire*>, vector<wire*>);
        ~gate_tree();
        gate_node* find_node(gate* g);
        vector<gate*> longest_path();
        gate_node* head() {return head_node; }
    private:
        gate* find_driving_gate(wire* t);
        vector<gate*> longest_path_down(gate_node* node);
        int delays_of(gate_node* node);


        vector<gate*> gates_feeding(gate* g);
        gate* parent_of(gate *g);
        vector<gate_node*> children_of(gate_node* p);

        vector<gate_node*> all_nodes;
        gate_node* head_node;

        vector<gate*> gate_list;
        vector<wire*> output_list;
        vector<wire*> normal_wire_list;

};

bool comp_int(int i1, int i2);
int index_of_maximum_in(vector<int> heights);


#endif