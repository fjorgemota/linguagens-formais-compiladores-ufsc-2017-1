#include "question_mark_node.h"

list<NodeAction> QuestionMarkNode::ascend() {
    list<NodeAction> neighbors;
    neighbors.push_back(NodeAction(this->getParent(), up));
    return neighbors;
}

list<NodeAction> QuestionMarkNode::descend() {
    list<NodeAction> neighbors;
    neighbors.push_back(NodeAction(this->getLeft(), down));
    return neighbors;
}

NodeType QuestionMarkNode::getType() {
    return NodeType::QUESTION;
}
