#ifndef AUTOMATA_TAB_H
#define AUTOMATA_TAB_H

#include "finite_automata.h"
#include <QString>
#include <QObject>

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
