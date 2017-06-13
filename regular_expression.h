#include "all.h"
#include "node.h"

using namespace std;

class RegularExpression {
  public:
    RegularExpression(string re);
    int getLessPriority(string re);
    string normalize();
    Node* getTree();

  private:
    Node* getNode(char c, Node *root);
    Node* getTree(string re, Node *root);
    int getOrder(char c);
    bool isMultiplier(char c);
    bool isTerminal(char c);
    bool shouldConcatenate(char prev, char c);
    char fixMultipliers(string multipliers);

    string regex;
};
