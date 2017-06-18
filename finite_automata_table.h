#ifndef FINITE_AUTOMATA_TABLE_H
#define FINITE_AUTOMATA_TABLE_H
#include "finite_automata.h"
#include <QTableWidget>

/*!
 * Implements a widget for the FiniteAutomata objects.
 */
class FiniteAutomataTable : public QTableWidget
{
    Q_OBJECT

public:
    /*!
     * Implements a Qt widget that manages in a smart way
     * FiniteAutomata objects, allowing to fill the table
     * with data from an already constructed
     * FiniteAutomata and allowing to export the data to
     * a FiniteAutomata object
     *
     * @param parent A parent Qt Widget, if any
     */
    explicit FiniteAutomataTable(QWidget *parent = 0);

    /*!
     * Destructs the object, freeing any memory used
     */
    ~FiniteAutomataTable();

    /*!
     * Fill this widget with data from another automata
     *
     * \param f The Finite Automata to use
     */
    void fromAutomata(FiniteAutomata &f);

    /*!
     * Exports the data of this widget to a
     * FiniteAutomata object
     *
     * @return The exported FiniteAutomata object
     */
    FiniteAutomata toAutomata();

    /*!
     * Check if the data in this widget is valid or not
     *
     * @return true if the data is valid, false otherwise
     */
    bool isValid();
private:
    /*!
     * Fill the background of the item with yellow color
     * and set the status tooltip of the widget
     *
     * @param item The item to alter
     * @param message The message, if any. If empty, the
     * background color is set to white.
     */
    void warn(QTableWidgetItem *item, string message = "");

    /*!
     * Return the states registered in the table.
     *
     * @return A set of strings representing the states
     * detected
     */
    set<string> getStates();

    /*!
     * Return the alphabet registered in the table.
     *
     * @return A set of symbols representing the alphabet
     * detected
     */
    set<char> getAlphabet();

    /*!
     * Validate transactions to check if the states present
     * there are valid (e.g do not point to an invalid
     * state)
     */
    void validateTransitions();

    /*!
     * Check if a transaction is valid
     *
     * @param item The cell representing the transaction
     * @param states The states actually present in the table
     */
    void validateTransition(QTableWidgetItem *item, set<string> states);

    /*!
     * Check if the alphabet present in this table is valid, like
     * if there is no empty cell, if the cells have only one char
     * and if there were no duplicated cells
     */
    void validateAlphabet();


    /*!
     * Check if the states present in this table is valid, llike
     * if there is no empty cell and if the cells there were
     * no duplicated cells
     */
    void validateStates();

    /*!
     * "Fix" state name, removing "*" and "->" from the start of
     * the string, if present.
     *
     * @param state The string to fix
     * @return The fixed state name
     */
    string fixStateName(string state);

    /*!
     * Remove any rows or columns that are empty, allowing the
     * table to stay thin.
     */
    void fitTable();

    const static QString WARNING_COLOR; //!< The color in the background of the cells with warnings

public slots:
    /*!
     * Detect changes in the table and respond to them validating
     * the data, adding columns and rows and removing columns and
     * rows as needed.
     *
     * @param item The actual cell changed
     */
    void insertRowOrColumn(QTableWidgetItem *item);
};

#endif // FINITE_AUTOMATA_TABLE_H
