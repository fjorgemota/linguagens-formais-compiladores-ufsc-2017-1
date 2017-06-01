#include "dot_node.h"
#include <iostream>

void DotNode::ascend() {
    this->getRight();
}

void DotNode::descend() {
    this->getLeft();
}

#ifndef DOT_NODE_TEST
int main() {
    DotNode *node = DotNode('.');

    cout << node->getValue() << "\n";
}
#endif
