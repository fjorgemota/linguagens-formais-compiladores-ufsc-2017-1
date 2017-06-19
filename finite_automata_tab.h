#ifndef FINITE_AUTOMATA_TAB_H
#define FINITE_AUTOMATA_TAB_H

#include <QWidget>
#include "finite_automata_table.h"
#include "automata_tab.h"

/*!
 * Implement a Qt Widget that manages a table representing
 * a FiniteAutomata, so the user can edit and do various
 * operations with it.
 */
class FiniteAutomataTab : public AutomataTab {
    Q_OBJECT

public:
    /*!
     * Implements a Qt tab that manages in a smart way
     * FiniteAutomata objects, allowing to edit it and
     * do operations with it
     *
     * @param parent A parent Qt Widget, if any
     */
    explicit FiniteAutomataTab(QWidget *parent = 0);

    /*!
     * Fill this widget with data from another automata
     *
     * @param f The Finite Automata to use
     */
    void fromAutomata(FiniteAutomata &f);

    /*!
     * Fill this widget with data from another tab
     *
     * @param f The tab to use in the importation
     */
    void fromTab(FiniteAutomataTab &f);

    /*!
     * Convert the data in this table to a Automata
     */
    FiniteAutomata toAutomata() override;

    /*!
     * Check if the automata present in this tab is
     * valid or not
     *
     * \return true if it is valid, false otherwise
     */
    bool isValid() override;
private:
    FiniteAutomataTable *table;
};

#endif // FINITE_AUTOMATA_TAB_H
