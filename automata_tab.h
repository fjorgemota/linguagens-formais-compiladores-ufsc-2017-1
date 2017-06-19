#ifndef AUTOMATA_TAB_H
#define AUTOMATA_TAB_H

#include "finite_automata.h"
#include <QString>
#include <QObject>

/*!
 * This class is the base for classes like FiniteAutomataTab
 * and RegularExpressionTab. The goal is to create a base for
 * common methods that are used across the operations, so we
 * can easily detect what can be operated and if it is valid
 * or not.
 */
class AutomataTab : public QWidget {
    Q_OBJECT

public:
    // Uses the QWidget's constructor
    using QWidget::QWidget;

    /*!
     * Convert the data in this tab to a Automata
     */
    virtual FiniteAutomata toAutomata() = 0;

    /*!
     * Check if the automata present in this tab is
     * valid or not
     *
     * \return true if it is valid, false otherwise
     */
    virtual bool isValid() = 0;
};

#endif // AUTOMATA_TAB_H
