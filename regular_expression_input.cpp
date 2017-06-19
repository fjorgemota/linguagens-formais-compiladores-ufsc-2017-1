#include "regular_expression_input.h"
#include <iostream>

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

    connect(this, SIGNAL(cursorPositionChanged()),
            this, SLOT(matchParentheses()));
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
    int count_parenthesis = 0;

    for (char &c : regex_input) {
        if (c == '(') {
            count_parenthesis++;
        } if (c == ')') {
            count_parenthesis--;
        }

        if (count_parenthesis < 0) {
            return false;
        }
    }

    return count_parenthesis == 0;
}

void RegularExpressionInput::matchParentheses() {
    QList<QTextEdit::ExtraSelection> selections;
    setExtraSelections(selections);
    BlockData *data = static_cast<BlockData *>(textCursor().block().userData());

    if (data) {
        QMap<int, int> infos = data->parenthesis;

        int curPos = textCursor().position() - textCursor().block().position();
        if (infos.count(curPos)) {
            createParenthesisSelection(infos[curPos]);
            createParenthesisSelection(curPos);
        }
    }
}

void RegularExpressionInput::createParenthesisSelection(int pos) {
    QList<QTextEdit::ExtraSelection> selections = extraSelections();

    QTextEdit::ExtraSelection selection;
    QTextCharFormat format = selection.format;
    format.setBackground(Qt::green);
    selection.format = format;

    QTextCursor cursor = textCursor();
    cursor.setPosition(pos);
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    selection.cursor = cursor;

    selections.append(selection);

    setExtraSelections(selections);
}
