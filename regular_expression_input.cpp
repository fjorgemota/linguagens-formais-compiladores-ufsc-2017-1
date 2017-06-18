#include "regular_expression_input.h"

RegularExpressionInput::RegularExpressionInput(QWidget *parent)
    : QTextEdit(parent)
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    this->setFont(font);
    this->setTextInteractionFlags(Qt::TextEditorInteraction);

    highlighter = new RegularExpressionHighlighter(this->document());
}

RegularExpression RegularExpressionInput::toRegularExpression() {
    QString regex = this->toPlainText();
    return RegularExpression(regex.toUtf8().constData());
}

void RegularExpressionInput::fromRegularExpression(RegularExpression regex) {
    string re = regex.getRegularExpression();
    this->setText(QString::fromStdString(re));
}

bool RegularExpressionInput::isValid() {
    RegularExpression input = this->toRegularExpression();
    string regex_input = input.getRegularExpression();

    int count_parentesis = 0;
    for (char &c : regex_input) {
        if (c == '(') {
            count_parentesis++;
        } else if (c == ')') {
            count_parentesis--;
        }
    }

    return count_parentesis == 0;
}
