#include "all.h"
#include "question_mark_node.h"
#include "star_node.h"
#include "dot_node.h"
#include "plus_node.h"
#include "union_node.h"
#include "leaf_node.h"
#include "lambda_node.h"

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
