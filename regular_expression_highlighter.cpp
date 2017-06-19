#include "regular_expression_highlighter.h"

RegularExpressionHighlighter::RegularExpressionHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    union_mark.setForeground(Qt::darkGreen);
    union_mark.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression("\\|");
    rule.format = union_mark;
    highlightingRules.append(rule);

    question_mark.setForeground(Qt::darkMagenta);
    question_mark.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression("\\?");
    rule.format = question_mark;
    highlightingRules.append(rule);

    plus.setForeground(Qt::darkBlue);
    plus.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression("\\+");
    rule.format = plus;
    highlightingRules.append(rule);

    star.setForeground(Qt::darkRed);
    star.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression("\\*");
    rule.format = star;
    highlightingRules.append(rule);
}

void RegularExpressionHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    BlockData *data = new BlockData;

    int count_parentesis = 0;
    int p = 0;
    QStack<int> positions;
    for (QChar c : text) {
        if (c == '(') {
            count_parentesis++;
        }
        if (count_parentesis <= 0) {
            p++;
            continue;
        }

        if (c == '(') {
            positions.push(p);
        } else if (c == ')') {
            int start = positions.top();
            positions.pop();
            data->parenthesis[start] = p;
            data->parenthesis[p] = start;
            count_parentesis--;
        }
        p++;
    }

    setCurrentBlockUserData(data);
    setCurrentBlockState(0);
}
