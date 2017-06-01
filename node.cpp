#include "node.h"

Node::Node(string v) {
    value = v;
    left = NULL;
    right = NULL;
}

Node* Node::getLeft() {
    return this->left;
}

Node* Node::getRight() {
    return this->right;
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

void Node::connectLeft(Node *a, Node *b) {
    a->setLeft(b);
}

void Node::connectRight(Node *a, Node *b) {
    a->setRight(b);
}
