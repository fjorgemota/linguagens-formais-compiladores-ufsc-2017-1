#ifndef OPERATION_TAB_H
#define OPERATION_TAB_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include "finite_automata.h"
#include "finite_automata_table.h"
#include "move_to_tab_button.h"
#include "mainwindow.h"


/*!
 * A widget that represents an operation
 */
class OperationTab : public QWidget
{
    Q_OBJECT

public:
    /*!
     * Constructs a tab which represents an operation
     *
     * @param widget The parent widget, if any
     */
    explicit OperationTab(QWidget *widget = 0);

    /*!
     * Add a new step to this tab
     *
     * @param window The window of the program which started this operation
     * @param f The intermediary Finite Automata
     * @param title The text describing the step actually made
     */
    void addStep(MainWindow *window, FiniteAutomata f, QString title);

    /*!
     * Add a new step to this tab
     *
     * @param title The text describing the step actually made
     */
    void addStep(QString title);
private:
    QGridLayout *gridLayout; //!< The grid layout of this ttab
};

#endif // OPERATION_TAB_H
