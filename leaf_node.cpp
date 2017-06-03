#include "leaf_node.h"

list<NodeAction> LeafNode::ascend() {
    list<NodeAction> neighbors;
    neighbors.push_back(NodeAction(this->getParent(), up));
    return neighbors;
}

list<NodeAction> LeafNode::descend() {
    list<NodeAction> neighbors;
    return neighbors;
}

NodeType LeafNode::getType() {
    return NodeType::LEAF;
}
