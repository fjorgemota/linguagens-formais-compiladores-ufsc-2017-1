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
    setCurrentBlockState(0);
}
