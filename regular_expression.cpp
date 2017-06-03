#include <string>
#include <iostream>
#include <exception>
#include <iterator>

using namespace std;

class RegularExpression {
  public:
        int getLessPriority(string re) {
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
        string normalize(string re) {
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

  private:
        int getOrder(char c) {
            if (c == '|') {
                return 0;
            } else if (c == '*' || c == '+' || c == '?') {
                return 2;
            } else if (c == '.') {
                return 1;
            }
            return 3;
        }

        bool isMultiplier(char c) {
            return c == '*' || c == '+' || c == '?';
        }

        bool isTerminal(char c) {
            return !(c == '|' || isMultiplier(c) | c == ')' || c == '(');
        }

        bool shouldConcatenate(char prev, char c) {
            bool pTerminal = isTerminal(prev);
            bool cTerminal = isTerminal(c);
            if (pTerminal && cTerminal) {
                return true;
            }
            return isMultiplier(prev) && cTerminal ||
                    isMultiplier(prev) && c == '(';
        }


        string regex;
};

#ifndef REGULAR_EXPRESSION_TEST
int main(int argc, char const *argv[]) {
    RegularExpression regex;
    string re = "(a|bc)*(ab)*ab";
    cout << regex.normalize(re) << endl;
    int pos = regex.getLessPriority(re);
    string c = regex.normalize(re).substr(pos);
    cout << c << endl;
    return 0;
}
#endif
