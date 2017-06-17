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
    bool operator<(const NodeAction &act) const;

  private:
    Direction direction;
    Node *node;
};

class Node {
  public:
    Node(char v, Node *root);
    Node* getLeft();
    Node* getRight();
    Node* getParent();
    char getValue();
    void setRight(Node* n);
    void setLeft(Node* n);
    virtual list<NodeAction> descend() = 0;
    virtual list<NodeAction> ascend() = 0;
    virtual NodeType getType() = 0;

  private:
    Node *left;
    Node *right;
    Node *root;
    char value;
};


class DotNode : public Node {
  public:
    using Node::Node;

    list<NodeAction> ascend();
    list<NodeAction> descend();
    NodeType getType();
};

class LambdaNode : public Node {
  public:
      using Node::Node;

      list<NodeAction> ascend();
      list<NodeAction> descend();
      NodeType getType();
};

class LeafNode : public Node {
  public:
    using Node::Node;

    list<NodeAction> ascend();
    list<NodeAction> descend();
    NodeType getType();
};

class PlusNode : public Node {
  public:
    using Node::Node;

    list<NodeAction> ascend();
    list<NodeAction> descend();
    NodeType getType();
};

class QuestionMarkNode : public Node {
  public:
    using Node::Node;

    list<NodeAction> ascend();
    list<NodeAction> descend();
    NodeType getType();
};

class StarNode : public Node {
  public:
    using Node::Node;

    list<NodeAction> ascend();
    list<NodeAction> descend();
    NodeType getType();
};

class UnionNode : public Node {
  public:
    using Node::Node;

    list<NodeAction> ascend();
    list<NodeAction> descend();
    NodeType getType();
};
#endif
