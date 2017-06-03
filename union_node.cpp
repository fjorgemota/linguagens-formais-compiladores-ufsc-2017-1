#include "union_node.h"

list<NodeAction> UnionNode::ascend() {
    Node *last = this;
    while (last->getRight() != NULL) {
        last = last->getRight();
    }
    list<NodeAction> neighbors;
    neighbors.push_back(NodeAction(last->getParent(), up));
    return neighbors;
}

list<NodeAction> UnionNode::descend() {
    list<NodeAction> neighbors;
    neighbors.push_back(NodeAction(this->getLeft(), down));
    neighbors.push_back(NodeAction(this->getRight(), down));
    return neighbors;
}

NodeType UnionNode::getType() {
    return NodeType::UNION;
}
