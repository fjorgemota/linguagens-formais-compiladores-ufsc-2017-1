#include <string>

using namespace std;

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
    virtual void descend() = 0;
    virtual void ascend() = 0;

  private:
    Node *left;
    Node *right;
    string value;
};
