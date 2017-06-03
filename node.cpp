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
