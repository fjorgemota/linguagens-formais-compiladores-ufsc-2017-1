#include <string>
#include <iostream>
#include <exception>
#include <iterator>

using namespace std;

class RegularExpression {
  public:
        int getLessPriority(string re) {
            int pos = 0;
            int less_priority = 5;
            int count_parentesis = 0;
            int subexpr;
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

  private:
        int getOrder(char c) {
            if (c == '|') {
                return 0;
            } else if (c == '*' || c == '+' || c == '?') {
                return 2;
            }
            return 1;
        }

        string regex;
};

#ifndef REGULAR_EXPRESSION_TEST
int main(int argc, char const *argv[]) {
    RegularExpression regex;
    string re = "(a|bc)*(ab)*|ab";
    int str = regex.getLessPriority(re);
    string subsr1 = re.substr(0, str);
    string subsr2 = re.substr(str+1);

    cout << subsr1 << "\n";
    cout << subsr2 << "\n";
    return 0;
}
#endif
