#ifndef ALGORITHM_HELPER_H
#define ALGORITHM_HELPER_H

#define DEBUG false
#include<list>
#include"../structure/node_state.h"

using namespace std;

int calculate_cost(Node_State node) {
    Node_State* parent = node.getParent();
    if (parent == nullptr) {
        return node.getG(); // If root node, return its g value
    }
    return node.getG() + parent->getG();
}

#endif