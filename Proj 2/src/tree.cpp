#include "../bin/tree.hpp"

gate_node::gate_node(gate* v) {
    parent = NULL;
    value = v;
    children = vector<gate_node*>();
}

gate_node::gate_node(gate* v, vector<gate_node*> c, gate_node* p) {
    parent = p;
    value = v;
    children = c;
}

gate* gate_tree::parent_of(gate *c) {
    wire* o = c->output;
    for(gate* g: gate_list) {
        if(g->is_attached_to(o)) {
            return g;
        }
    }
    return NULL;
}

vector<gate*> gate_tree::gates_feeding(gate* t) {
    vector<gate*> g_feeding;
    for(gate* g: gate_list) {
        for(wire* w: t->inputs) {
            if(w == g->output) {
                g_feeding.push_back(g);
            }
        }
    }
    return g_feeding;
}

vector<gate_node*> gate_tree::children_of(gate_node* parent_node) {
    vector<gate*> children_gates = gates_feeding(parent_node->value);
    vector<gate_node*> children_nodes;

    for(gate* child_gate: children_gates) {
        gate_node* child_node = new gate_node(child_gate);
            child_node->children = children_of(child_node);
            child_node->parent = parent_node;
        all_nodes.push_back(child_node);
        children_nodes.push_back(child_node);
    }
    
    return children_nodes;
}

gate_tree::~gate_tree() {
    for(gate_node* n: all_nodes)
        free(n);
}

gate* gate_tree::find_driving_gate(wire* t) {
    for(gate* g: gate_list) {
        if(g->is_driving(t))
            return g;
    }
    return NULL;
}

bool comp_int(int i1, int i2) {
    return i1 > i2;
}


gate_tree::gate_tree(vector<gate*> gl, vector<wire*> ol, vector<wire*> nl):
gate_list(gl), output_list(ol), normal_wire_list(nl) {
    head_node = new gate_node(NULL);
    all_nodes.push_back(head_node);

    for(wire* o: output_list) {
        gate* final_gate = find_driving_gate(o);
        gate_node* final_node = new gate_node(final_gate);
        final_node->parent = head_node;
        head_node->children.push_back(final_node);
        final_node->children = children_of(final_node);
        all_nodes.push_back(final_node);
    }
    cout << SIM_ACK << "created a " << all_nodes.size()
        << "-node gate_tree. " << endl; 
}

int gate_tree::delays_of(gate_node* node) {
    if(node->children.size() == 0) {
        return node->value->d();
    } else {
        vector<int> delays;
        for(gate_node* c: node->children) {
            delays.push_back(delays_of(c));
        }
        int index = index_of_maximum_in(delays);
        return node->value->d() + node->children[index]->value->d();
    }
}

int index_of_maximum_in(vector<int> heights) {
    int max = -1;
    int index = -1;
    for(int i = 0; i < heights.size(); i++) {
        if(heights[i] > max) {
            index = i;
            max = heights[i];
        }
    }
    return index;
}

vector<gate*> gate_tree::longest_path_down(gate_node* parent) {
    vector<int> lower_delays;
    if(parent->children.size() == 0) {
        return vector<gate*>({parent->value});
    }
    for(gate_node* child: parent->children) {
        lower_delays.push_back(delays_of(child));
    }
    int max_index = index_of_maximum_in(lower_delays);
    gate_node* tallest_child = parent->children[max_index];
    vector<gate*> longest_path = (parent->value != NULL) ?
            vector<gate*>({parent->value}) :
            vector<gate*>({});
    vector<gate*> exclusive = longest_path_down(tallest_child);
    longest_path.insert(longest_path.end(), exclusive.begin(), exclusive.end());
    return longest_path;
}

vector<gate*> gate_tree::longest_path() {
    return longest_path_down(head_node);
}

gate_node* gate_tree::find_node(gate* t) {
    for(gate_node* g: all_nodes)
        if(g->value == t)
            return g;
    return NULL;
}
