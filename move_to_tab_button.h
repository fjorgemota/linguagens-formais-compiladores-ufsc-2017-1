#ifndef MOVE_TO_TAB_BUTTON_H
#define MOVE_TO_TAB_BUTTON_H

#include <QPushButton>
#include "finite_automata.h"
#include "finite_automata_tab.h"
#include "mainwindow.h"

/*!
 * Represents a button that, when clicked, will create
 * a new tab with the specified FiniteAutomata preloaded
 */
class MoveToTabButton : public QPushButton
{
    Q_OBJECT

public:
    /*!
     * Constructs a new button, that, when clicked, will create a new
     * tab with the specified FiniteAutomata preloaded
     *
     * @param parent The parent widget, if any
     * @param window The Window, necessary to create the tab
     * @param f The FiniteAutomata that this button represents
     */
    MoveToTabButton(QWidget *parent, MainWindow *window, FiniteAutomata f);

public slots:
    /*!
     * Method that is called when the button is clicked. It simply creates
     * a new tab, asking the name of it for the user, with the FiniteAutomata
     * that this object represents
     */
    void moveToTab();
private:
    FiniteAutomata f; //!< The FiniteAutomata that this object represents
    MainWindow *window; //!< The window, needed to create the tab
};

#endif // MOVE_TO_TAB_BUTTON_H
