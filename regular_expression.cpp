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
            (isMultiplier(prev) && c == '(');
}

Node* RegularExpression::getNode(char c) {
    switch(c) {
        case '.': return new DotNode(c);
        case '|': return new UnionNode(c);
        case '?': return new QuestionMarkNode(c);
        case '*': return new StarNode(c);
        case '+': return new PlusNode(c);
        default: return new LeafNode(c);
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
    return getTree(normalize());
}

Node* RegularExpression::getTree(string re) {
    int less_prior_position = getLessPriority(re);

    int size = re.size();
    while (less_prior_position == -1 && !(re.empty())) {
        re = re.substr(1, size-2);
        size = re.size();
        less_prior_position = getLessPriority(re);
    }

    if (re.empty()) {
        return NULL;
    } else if (!(re.empty()) && less_prior_position == -1) {
        return new LeafNode(re[0]);
    }

    char op = re.at(less_prior_position);
    Node* root = getNode(op);

    string subre_left = re.substr(0, less_prior_position-1);
    string subre_right = re.substr(less_prior_position+1);
    Node* left_child = getTree(subre_left);
    Node* right_child = getTree(subre_right);

    root->setLeft(left_child);
    root->setRight(right_child);

    return root;
}
