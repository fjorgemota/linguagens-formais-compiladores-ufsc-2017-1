#include "plus_node.h"

list<NodeAction> PlusNode::ascend() {
    list<NodeAction> neighbors;
    neighbors.push_back(NodeAction(this->getParent(), up));
    neighbors.push_back(NodeAction(this->getLeft(), down));
    return neighbors;
}

list<NodeAction> PlusNode::descend() {
    list<NodeAction> neighbors;
    neighbors.push_back(NodeAction(this->getLeft(), down));
    return neighbors;
}

NodeType PlusNode::getType() {
    return NodeType::PLUS;
}
