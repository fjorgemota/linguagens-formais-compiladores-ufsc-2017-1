#include "node.h"

NodeAction::NodeAction(Node* n, Direction d) {
    this->direction = d;
    this->node = n;
}

Node* NodeAction::getNode() {
    return this->node;
}

Direction NodeAction::getDirection() {
    return this->direction;
}

Node::Node(char v) {
    value = v;
    left = NULL;
    right = NULL;
    parent = NULL;
}

Node* Node::getLeft() {
    return this->left;
}

Node* Node::getRight() {
    return this->right;
}

Node* Node::getParent() {
    return this->parent;
}

char Node::getValue() {
    return this->value;
}

void Node::setRight(Node *n) {
    this->right = n;
}

void Node::setLeft(Node *n) {
    this->left = n;
}

void Node::setParent(Node *n) {
    this->parent = n;
}

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

list<NodeAction> LambdaNode::ascend() {
    list<NodeAction> neighbors;
    return neighbors;
}

list<NodeAction> LambdaNode::descend() {
    list<NodeAction> neighbors;
    return neighbors;
}

NodeType LambdaNode::getType() {
    return NodeType::LAMBDA;
}

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

NodeType StarNode::getType() {
    return NodeType::STAR;
}

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
