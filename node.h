#ifndef NODE_H
#define NODE_H

#include "all.h"

using namespace std;

enum Direction {up, down};
enum NodeType {DOT, LAMBDA, LEAF, PLUS, QUESTION, STAR, UNION};

// There is a symbol called Node, just name it here..
class Node;

class NodeAction {
  public:
    NodeAction(Node* n, Direction d);
    Node *getNode();
    Direction getDirection();

  private:
    Node* node;
    Direction direction;
};

class Node {
  public:
    Node(char v);
    Node* getLeft();
    Node* getRight();
    Node* getParent();
    char getValue();
    void setRight(Node* n);
    void setLeft(Node* n);
    void setParent(Node* n);
    virtual list<NodeAction> descend() = 0;
    virtual list<NodeAction> ascend() = 0;
    virtual NodeType getType() = 0;

  private:
    Node *left;
    Node *right;
    Node *parent;
    char value;
};

#endif // NODE_H