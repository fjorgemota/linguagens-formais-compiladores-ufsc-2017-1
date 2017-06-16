#include "regular_expression.h"

RegularExpression::RegularExpression(string re) {
    regex = re;
}

int RegularExpression::getLessPriority(string re) {
    int pos = 0;
    int less_priority = 3;
    int count_parentesis = 0;
    int subexpr = -1;
    for (char& c : re) {
        if (c == '(') {
            count_parentesis++;
        }

        if (getOrder(c) < less_priority && count_parentesis == 0) {
            less_priority = getOrder(c);
            subexpr = pos;
        }

        if (c == ')') {
            count_parentesis--;
        }
        pos++;
    }
    return subexpr;
}

string RegularExpression::normalize() {
    string::iterator it = regex.begin();
    string result;
    if (it == regex.end()) {
        return result;
    }
    char prev = *it;
    result.append(1, prev);
    string multipliers;
    for (++it; it != regex.end(); ++it) {
        char c = *it;
        if (isMultiplier(c)) {
            multipliers.append(1, c);
            continue;
        } else if(!multipliers.empty()) {
            prev = fixMultipliers(multipliers);
            result.append(1, prev);
            multipliers = "";
        }
        if (shouldConcatenate(prev, c)) {
            result.append(1, '.');
        }
        result.append(1, c);
        prev = c;
    }
    if(!multipliers.empty()) {
        prev = fixMultipliers(multipliers);
        result.append(1, prev);
        multipliers = "";
    }
    return result;
}

char RegularExpression::fixMultipliers(string multipliers) {
    string::iterator mit = multipliers.begin();
    char multiplier = *mit;
    for (++mit; mit != multipliers.end(); ++mit) {
        if (multiplier != *mit) {
            multiplier = '*';
            break;
        }
    }
    return multiplier;
}


int RegularExpression::getOrder(char c) {
    if (c == '|') {
        return 0;
    } else if (c == '*' || c == '+' || c == '?') {
        return 2;
    } else if (c == '.') {
        return 1;
    }
    return 3;
}

bool RegularExpression::isMultiplier(char c) {
    return c == '*' || c == '+' || c == '?';
}

bool RegularExpression::isTerminal(char c) {
    return !(c == '|' || isMultiplier(c) | (c == ')') || c == '(');
}

bool RegularExpression::shouldConcatenate(char prev, char c) {
    bool pTerminal = isTerminal(prev);
    bool cTerminal = isTerminal(c);
    if (pTerminal && cTerminal) {
        return true;
    }
    return (isMultiplier(prev) && cTerminal) ||
            (((isMultiplier(prev) || pTerminal) && c == '(') ||
            (prev == ')' && c == '(') ||
            (prev == ')' && cTerminal));
}

Node* RegularExpression::getNode(char c, Node *root) {
    switch(c) {
        case '.': return new DotNode(c, root);
        case '|': return new UnionNode(c, root);
        case '?': return new QuestionMarkNode(c, root);
        case '*': return new StarNode(c, root);
        case '+': return new PlusNode(c, root);
        default: return new LeafNode(c, root);
    }
}

Node* RegularExpression::getTree() {
    Node *root = new LambdaNode('L', 0);
    Node *tree = getTree(normalize(), root);
    if (tree) {
        root->setLeft(tree);
        return tree;
    } else {
        return root;
    }
}

Node* RegularExpression::getTree(string re, Node *parent) {
    int less_prior_position = getLessPriority(re);

    int size = re.size();
    while (less_prior_position == -1 && !(re.empty()) && re[0] == '(' && re[size-1] == ')') {
        re = re.substr(1, size-2);
        size = re.size();
        less_prior_position = getLessPriority(re);
    }

    if (re.empty()) {
        return NULL;
    } else if (!(re.empty()) && less_prior_position == -1) {
        return getNode(re[0], parent);
    }

    char op = re.at(less_prior_position);
    Node* root = getNode(op, parent);

    string subre_left = re.substr(0, less_prior_position);
    string subre_right = re.substr(less_prior_position+1);
    Node* left_child = getTree(subre_left, root);
    Node* right_child = getTree(subre_right, root);
    root->setLeft(left_child);
    root->setRight(right_child);

    return root;
}

map<Node*, set<Node*>> RegularExpression::getCompositionPerLeaf(Node* tree) {
    map<Node*, set<Node*>> table;
    set<Node*> achievable;
    list<NodeAction> to_process;
    Node *leaf;
    queue<Node*> leaves;
    Node *root = tree;

    list<Node*> nodes;
    nodes.push_back(root);

    while (!nodes.empty()) {
        root = nodes.back();
        nodes.pop_back();

        if (root->getType() == LEAF) {
            leaves.push(root);
        }
        if (root->getLeft()) {
            nodes.push_back(root->getLeft());
        }
        if (root->getRight()) {
            nodes.push_back(root->getLeft());
        }
    }

    while(!leaves.empty()) {
        leaf = leaves.front();
        leaves.pop();
        to_process = leaf->ascend();

        while (!to_process.empty()) {
            NodeAction child = to_process.back();
            to_process.pop_back();
            root = child.getNode();

            if (root->getType() == LEAF || root->getType() == LAMBDA) {
                achievable.insert(root);
            } else if (child.getDirection() == up) {
                auto it = to_process.begin();
                auto ascend = root->ascend();
                to_process.insert(it, ascend.begin(), ascend.end());
            } else if (child.getDirection() == down) {
                auto it = to_process.begin();
                auto descend = root->descend();
                to_process.insert(it, descend.begin(), descend.end());
            }
        }

        table[leaf] = achievable;
    }

    return table;
}

set<Node*> RegularExpression::getFirstComposition(Node* tree) {
    Node *root = tree;
    map<Node*, set<Node*>> table = getCompositionPerLeaf(root);
    set<Node*> composition;

    list<NodeAction> to_process = root->descend();

    while(!to_process.empty()) {
        NodeAction child = to_process.back();
        to_process.pop_back();
        root = child.getNode();

        if (root->getType() == LEAF || root->getType() == LAMBDA) {
            composition.insert(root);
        } else if (child.getDirection() == up) {
            auto it = to_process.begin();
            auto ascend = root->ascend();
            to_process.insert(it, ascend.begin(), ascend.end());
        } else if (child.getDirection() == down) {
            auto it = to_process.begin();
            auto descend = root->descend();
            to_process.insert(it, descend.begin(), descend.end());
        }
    }

    return composition;
}

set<char> RegularExpression::getAlphabet(map<Node*, set<Node*>> leaves_comp) {
    set<char> alphabet;
    for (auto &l : leaves_comp) {
        alphabet.insert(l.first->getValue());
    }

    return alphabet;
}

bool RegularExpression::hasLambda(set<Node*> composition) {
    for (Node *cps : composition) {
        if (cps->getType() == LAMBDA) {
            return true;
        }
    }
    return false;
}

FiniteAutomata RegularExpression::getAutomata(Node *tree) {
    map<Node*, set<Node*>> compositions = getCompositionPerLeaf(tree);
    set<Node*> first_composition = getFirstComposition(tree);

    set<char> alphabet = getAlphabet(compositions);
    FiniteAutomata automata;

    for (char s : alphabet) {
        automata.addSymbol(s);
    }

    automata.addState("q0", FiniteAutomata::INITIAL_STATE |
            (hasLambda(first_composition) ? FiniteAutomata::FINAL_STATE : 0));

    map<set<Node*>, string> nodes;
    int i = 0;
    nodes[first_composition] = "q" + to_string(i);

    queue<set<Node*>> q;
    q.push(first_composition);

    while (!q.empty()) {
        set<Node*> comp = q.front();
        q.pop();
        map<char, set<Node*>> transition;
        for (Node *ts : comp) {
            if (ts->getType() != LAMBDA) {
                transition[ts->getValue()].insert(compositions[ts].begin(),
                        compositions[ts].end());
            }
        }

        for (auto &trs : transition) {
            if (!nodes.count(trs.second)) {
                i++;
                nodes[trs.second] = "q" + to_string(i);
                q.push(trs.second);
                automata.addState(nodes[trs.second], (hasLambda(trs.second) ?
                            FiniteAutomata::FINAL_STATE : 0));
            }
            automata.addTransition(nodes[comp], trs.first, nodes[trs.second]);
        }
    }

    return automata;
}

template<typename T>
string printTree(T *root) {
    string result;
    queue<T*> nodes;
    nodes.push(root);
    map<T*, string> names;
    int i = 0;
    result.append("digraph {\n\tgraph [ranksep=\"equally\",nodesep=\"0.5\",ordering=\"out\",fontpath=\".\",fontname=\"Ubuntu-B\"];\n\tnode[fontname=\"Ubuntu-B\"];\n\tedge[fontname=\"Ubuntu-B\"];\n");
    while (!nodes.empty()) {
        T *n = nodes.front();
        nodes.pop();
        if (n->getLeft()) {
            nodes.push(n->getLeft());
        }
        if (n->getRight()) {
            nodes.push(n->getRight());
        }
        string name = "n"+to_string(i);
        names[n] = name;
        i++;
        if (n->getType() == LAMBDA) {
            continue;
        }
        T *parent = n->getParent();
        if (parent && parent->getType() == LAMBDA) {
            string lambdaName = "n"+to_string(i);
            names[parent] = lambdaName;
            result.append("\t\"");
            result.append(lambdaName);
            result.append("\" [image=\"lines.png\",label=\"\",imagepos=\"ml\",imagescale=\"none\",shape=\"none\"];\n");
            i++;
        }
        result.append("\t\"");
        result.append(name);
        result.append("\" [label=\"");
        result.append(1, n->getValue());
        result.append("\",shape=\"circle\"];\n");
    }
    nodes.push(root);
    i = 0;
    while (!nodes.empty()) {
        T *n = nodes.front();
        nodes.pop();
        if (n->getType() == LAMBDA) {
            nodes.push(n->getLeft());
            continue;
        }
        string rank = "\t{rank=\"same\" \"";
        if (n->getLeft()) {
            nodes.push(n->getLeft());
            result.append("\t\"");
            result.append(names[n]);
            result.append("\" -> \"");
            result.append(names[n->getLeft()]);
            result.append("\";\n");
            rank.append(names[n->getLeft()]);
            rank.append("\" -> \"");
        } else if (n->getRight()) {
            string name = "null"+to_string(i);
            result.append("\t\"");
            result.append(name);
            result.append("\" [style=\"invis\",shape=\"circle\"],label=\"0\";\n");
            result.append("\t\"");
            result.append(names[n]);
            result.append("\" -> \"");
            result.append(name);
            result.append("\" [style=\"invis\"];\n");
            rank.append(name);
            rank.append("\" -> \"");
            i++;
        }
        if (n->getRight()) {
            nodes.push(n->getRight());
            result.append("\t\"");
            result.append(names[n]);
            result.append("\" -> \"");
            result.append(names[n->getRight()]);
            result.append("\";\n");

            rank.append(names[n->getRight()]);
            rank.append("\" ");
        } else if (n->getLeft()) {
            string name = "null"+to_string(i);
            result.append("\t\"");
            result.append(name);
            result.append("\" [style=\"invis\",shape=\"circle\",label=\"0\"];\n");
            result.append("\t\"");
            result.append(names[n]);
            result.append("\" -> \"");
            result.append(name);
            result.append("\" [style=\"invis\"];\n");
            rank.append(name);
            rank.append("\" ");
            i++;
        }
        rank.append(" [style=\"invis\"]}\n");
        if (n->getLeft() || n->getRight()) {
            result.append(rank);
        }
        if (n->getParent()) {
            result.append("\t\"");
            result.append(names[n]);
            result.append("\" -> \"");
            result.append(names[n->getParent()]);
            result.append("\" [style=dotted];\n");
            if (n->getParent()->getType() == LAMBDA) {
                result.append("\n\t{rank=\"same\" \"");
                result.append(names[n]);
                result.append("\" -> \"");
                result.append(names[n->getParent()]);
                result.append("\" [style=\"invis\"]}\n");
            }
        }
    }
    result.append("\n}\n");
    return result;
}
