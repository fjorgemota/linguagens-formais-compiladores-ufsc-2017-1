#ifndef REGULAR_EXPRESSION_HIGHLIGHTER_H
#define REGULAR_EXPRESSION_HIGHLIGHTER_H

#include <QChar>
#include <QStack>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>

/*!
 * Corresponds to a block of text, in this case, the main focus is in
 * the parentheses presented in the block of text
 */
struct BlockData : public QTextBlockUserData {
    QMap<int, int> parenthesis; /**< Maps each parenthesis' position with
                                        the position of the correspondent */
};

/*!
 * Class used to highlight operators from a regular expression
 */
class RegularExpressionHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    /*!
     * Defines the highlight rules from each operator in a
     * regular expression (union, question mark, star and plus)
     *
     * @param parent A parent QTextDocument, necessary to put the text
     */
    explicit RegularExpressionHighlighter(QTextDocument *parent);

protected:
    /*!
     * Implements the highlight rules in a plain text
     *
     * @param text A block of text to highlight the specific operators, if
     * exists someone
     */
    void highlightBlock(const QString &text) override;

private:
    /*!
     * Represents a struct of a highlight rule, composed by the pattern
     * to follow and the format
     */
    struct HighlightingRule
    {
        QRegularExpression pattern; /**< The pattern that the symbol follows */
        QTextCharFormat format; /**< The format (color and font) of the symbol*/
    };

    QVector<HighlightingRule> highlightingRules; //!< Vector with all highlight
                                                 //!< rules defined in this class

    QTextCharFormat star; //!< The text symbol referent to the star operation
    QTextCharFormat plus; //!< The text symbol referent to the plus operation
    QTextCharFormat question_mark; //!< The text symbol referent to the
                                   //!< question mark operation
    QTextCharFormat union_mark; //!< The text symbol referent to the
                                //!< union operation
};

#endif // REGULAR_EXPRESSION_HIGHLIGHTER_H
