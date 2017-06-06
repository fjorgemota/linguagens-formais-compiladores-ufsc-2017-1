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

    char prev = *it;
    string result;
    result.append(1, prev);
    for (++it; it != regex.end(); ++it) {
        char c = *it;
        if (shouldConcatenate(prev, c)) {
            result.append(1, '.');
        }
        result.append(1, c);
        prev = *it;
    }
    return result;
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
            ((isMultiplier(prev) || pTerminal) && c == '(');
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

map<int, string> RegularExpression::getSubExpressions(string s) {
    map<int, string> result;
    int numParenthesis;
    string subExpr;
    int i = 0;
    for (const char &c: s) {
        if (c == '(') {
            if (numParenthesis == 0) {
                subExpr = string();
            }
            numParenthesis++;
            i++;
            continue;
        }
        if (numParenthesis > 0 && !(numParenthesis == 1 && c == ')')) {
            subExpr.append(1, c);
        }
        if (c == ')') {
            numParenthesis--;
            if (numParenthesis == 0) {
                result[i] = subExpr;
                result[i-subExpr.size()-1] = subExpr;
            }
        }
        i++;
    }
    return result;
}

Node* RegularExpression::getTree() {
    Node *root = new LambdaNode('L', 0);
    root->setLeft(getTree(normalize(), root));
    return root;
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
        return new LeafNode(re[0], parent);
    }

    char op = re.at(less_prior_position);
    Node* root = getNode(op, parent);

    string subre_left = re.substr(0, less_prior_position);
    string subre_right = re.substr(less_prior_position+1);
    Node* left_child = getTree(subre_left, root);
    Node* right_child = getTree(subre_right, root);
    // if (left_child && !left_child->getRight()) {
    //     left_child->setParent(root);
    // }
    // if (left_child && !right_child) {
    //     Node *right = left_child->getRight();
    //     while (right) {
    //         if (!right->getRight()) {
    //             right->setParent(root);
    //         }
    //         right = right->getRight();
    //     }
    // }
    root->setLeft(left_child);
    root->setRight(right_child);

    return root;
}


string printTree(Node *root) {
    string result;
    queue<Node*> nodes;
    nodes.push(root);
    map<Node*, string> names;
    int i = 0;
    result.append("digraph {\n\tgraph [ranksep=\"equally\",nodesep=\"0.5\",ordering=\"out\",root=\"n1\"];\n");
    while (!nodes.empty()) {
        Node *n = nodes.front();
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
        Node *parent = n->getParent();
        if (parent && parent->getType() == LAMBDA) {
            result.append("\t\"");
            result.append(names[parent]);
            result.append("\" [image=\"lines.png\",label=\"\",imagepos=\"ml\",imagescale=\"none\",shape=\"none\"];\n");
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
        Node *n = nodes.front();
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
                result.append("\" -> \"n0\" [style=\"invis\"]}");
            }
        }
    }
    result.append("\n}\n");
    return result;
}
int main() {
    RegularExpression re("(ab|ac)*a?|(ba?c)*");
    cout << printTree(re.getTree()) << endl;
    return 0;
}
