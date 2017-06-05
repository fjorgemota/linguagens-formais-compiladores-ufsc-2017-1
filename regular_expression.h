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
    map<int, string> getSubExpressions(string s);
    Node* getNode(char c);
    Node* getTree(string re);
    int getOrder(char c);
    bool isMultiplier(char c);
    bool isTerminal(char c);
    bool shouldConcatenate(char prev, char c);

    string regex;
};
