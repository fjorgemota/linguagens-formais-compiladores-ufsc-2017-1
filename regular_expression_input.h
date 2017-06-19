#ifndef REGULAR_EXPRESSION_INPUT_H
#define REGULAR_EXPRESSION_INPUT_H

#include "regular_expression_highlighter.h"
#include "regular_expression.h"
#include <QTextEdit>

/*!
 * Implements a text editor for the regular expressions
 */
class RegularExpressionInput : public QTextEdit
{
    Q_OBJECT

public:
    /*!
     * Defines the window with a plain text editor, with the font type
     * accpeted and the highlight rules
     *
     * @param parent A parent QTextEdit, if necessary
     */
    explicit RegularExpressionInput(QWidget *parent = 0);

    /*!
     * Get the regular expression in the plain text and translate
     * to an object of the RegularExpression class
     */
    RegularExpression toRegularExpression();

    /*!
     * Writes the specified RegularExpression object to the plain text
     *
     * @param regex A RegularExpression object
     */
    void fromRegularExpression(RegularExpression regex);

    /*!
     * Checks the regex in the input text, returns true if each parentheses
     * opened is properly closed
     */
    bool isValid();

private slots:
    /*!
     * Checks the plain text and find parentheses properly matched
     */
    void matchParentheses();

private:
    /*!
     * Highlights matching parentheses
     *
     * @param pos Position of the cursor to highlight
     */
    void createParenthesisSelection(int pos);
    RegularExpressionHighlighter *highlighter; //!< The rules of highlight
};

#endif // REGULAR_EXPRESSION_INPUT_H
