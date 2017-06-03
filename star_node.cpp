#include "star_node.h"

list<NodeAction> StarNode::ascend() {
    list<NodeAction> neighbors;
    neighbors.push_back(NodeAction(this->getLeft(), down));
    neighbors.push_back(NodeAction(this->getParent(), up));
    return neighbors;
}

list<NodeAction> StarNode::descend() {
    list<NodeAction> neighbors;
    neighbors.push_back(NodeAction(this->getLeft(), down));
    neighbors.push_back(NodeAction(this->getParent(), up));
    return neighbors;
}
