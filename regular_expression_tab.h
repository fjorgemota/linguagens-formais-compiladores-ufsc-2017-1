#ifndef REGULAR_EXPRESSION_TAB_H
#define REGULAR_EXPRESSION_TAB_H

#include <QtWidgets>
#include "regular_expression_input.h"

/*!
 * Represents a regular expression tab
 */
class RegularExpressionTab : public QWidget
{
    Q_OBJECT

public:
    /*!
     * Creates a layout tab with a text editor
     *
     * @param parent A parent QWidget, if any
     */
    RegularExpressionTab(QWidget *parent = 0);

    /*!
     * Writes the specified RegularExpression object to the plain text
     *
     * @param re A RegularExpression object
     */
    void fromRegularExpression(RegularExpression &re);

private:
    RegularExpressionInput *editor; //!< The text editor for regular expressions
};

#endif // REGULAR_EXPRESSION_TAB_H
