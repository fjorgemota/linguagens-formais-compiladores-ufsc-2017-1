#include "regular_expression.h"

RegularExpression::RegularExpression(string re) {
    regex = re;
}

int RegularExpression::getLessPriority(string re) {
    int pos = 0;
    int less_priority = 3;
    int count_parentesis = 0;
    int subexpr;
    string norm = normalize(re);
    for (char& c : norm) {
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

string RegularExpression::normalize(string re) {
    string::iterator it = re.begin();

    char prev = *it;
    string result;
    result.append(1, prev);
    for (++it; it != re.end(); ++it) {
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

#ifndef REGULAR_EXPRESSION_TEST
int main(int argc, char const *argv[]) {
    RegularExpression regex;
    string re = "(a|bc)*(ab)*ab";
    cout << regex.normalize(re) << endl;
    int pos = regex.getLessPriority(re);
    char c = regex.normalize(re).at(pos);
    cout << c << endl;
    return 0;
}
#endif