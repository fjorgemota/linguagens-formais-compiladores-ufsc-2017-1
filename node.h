#include "all.h"

using namespace std;

class NodeAction {
  public:
    NodeAction(Node* n, enum d);
    Node *getNode();
    enum getDirection();

  private:
    Node* node;
    enum direction {up, down};
};

class Node {
  public:
    Node(string v);
    Node *getLeft();
    Node *getRight();
    string getValue();
    void setRight(Node *n);
    void setLeft(Node *n);
    void connectLeft(Node *a, Node *b);
    void connectRight(Node *a, Node *b);
    virtual list<NodeAction> descend() = 0;
    virtual list<NodeAction> ascend() = 0;

  private:
    Node *left;
    Node *right;
    Node *parent;
    string value;
};
