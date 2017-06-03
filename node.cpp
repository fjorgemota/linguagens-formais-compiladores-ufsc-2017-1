#include "node.h"

NodeAction::NodeAction(Node* n, enum d) {
    this->direction = d;
    this->node = n;
}

Node* NodeAction::getNode() {
    return this->node;
}

enum NodeAction::getDirection() {
    return this->direction;
}

Node::Node(string v) {
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

string Node::getValue() {
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

void Node::connectLeft(Node *a, Node *b) {
    a->setLeft(b);
}

void Node::connectRight(Node *a, Node *b) {
    a->setRight(b);
}
