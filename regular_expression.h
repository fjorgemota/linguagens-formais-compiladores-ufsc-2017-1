#include "all.h"
#include "node.h"
#include "finite_automata.h"

using namespace std;

class RegularExpression {
  public:
    RegularExpression(string re);
    int getLessPriority(string re);
    string normalize();
    Node* getTree();
    FiniteAutomata getAutomata(Node* tree);

  private:
    Node* getNode(char c, Node *root);
    Node* getTree(string re, Node *root);
    map<Node*, set<Node*>> getCompositionPerLeaf(Node* tree);
    set<Node*> getFirstComposition(Node* tree);
    set<char> getAlphabet(map<Node*, set<Node*>> leaves_comp);
    int getOrder(char c);
    bool isMultiplier(char c);
    bool isTerminal(char c);
    bool shouldConcatenate(char prev, char c);
    bool hasLambda(set<Node*> composition);
    char fixMultipliers(string multipliers);

    string regex;
};
