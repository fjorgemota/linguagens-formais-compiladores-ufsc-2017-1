#include "UnionNode.h"

list<NodeAction> UnionNode::ascend() {
    Node *last = this->getRight();
    while (last != NULL) {
        last = last->getRight();
    }
    list<NodeAction> neighbors;
    neighbors.push_back(NodeAction(last->getParent, up));
    return neighbors;
}

list<NodeAction> UnionNode::descend() {
    list<NodeAction> neighbors;
    neighbors.push_back(NodeAction(this->getLeft, down));
    neighbors.push_back(NodeAction(this->getRight, down));
    return neighbors;
}
