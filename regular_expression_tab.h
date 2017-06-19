#ifndef REGULAR_EXPRESSION_TAB_H
#define REGULAR_EXPRESSION_TAB_H

#include <QWidget>
#include "regular_expression_input.h"
#include "automata_tab.h"

/*!
 * Represents a regular expression tab
 */
class RegularExpressionTab : public AutomataTab
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

    /*!
     * Return the Regular Expression that this widget contains
     * @return The RegularExpression object
     */
    RegularExpression toRegularExpression();

    /*!
     * Convert the Regular Expression of this widget to a
     * FiniteAutomata
     *
     * @return The FiniteAutomata that this widget contains
     */
    FiniteAutomata toAutomata() override;

    /*!
     * Check if this regular expression is valid
     *
     * @return true if it is valid, false otherwise
     */
    bool isValid() override;

    /*!
     * Return the Regular Expression that this widget contains, as
     * string
     *
     * \return A string containing the regular expression
     */
    QString getRegularExpression();

private:
    RegularExpressionInput *editor; //!< The text editor for regular expressions
};

#endif // REGULAR_EXPRESSION_TAB_H
