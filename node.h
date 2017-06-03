#include "all.h"

using namespace std;

enum Direction {up, down};

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
    Node(string v);
    Node* getLeft();
    Node* getRight();
    Node* getParent();
    string getValue();
    void setRight(Node* n);
    void setLeft(Node* n);
    void setParent(Node* n);
    virtual list<NodeAction> descend() = 0;
    virtual list<NodeAction> ascend() = 0;

  private:
    Node *left;
    Node *right;
    Node *parent;
    string value;
};
