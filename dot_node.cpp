#include "dot_node.h"

list<NodeAction> DotNode::ascend() {
    list<NodeAction> neighbors;
    neighbors.push_back(NodeAction(this->getRight(), down));
    return neighbors;
}

list<NodeAction> DotNode::descend() {
    list<NodeAction> neighbors;
    neighbors.push_back(NodeAction(this->getLeft(), down));
    return neighbors;
}

NodeType DotNode::getType() {
    return NodeType::DOT;
}
