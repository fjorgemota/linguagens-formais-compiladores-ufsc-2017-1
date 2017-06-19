#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include "finite_automata.h"
#include "automata_tab.h"

namespace Ui {
class MainWindow;
}

class OperationTab;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * Constructs the MainWindow, setting up the object
     * and a few other things
     *
     * @param parent The parent widget, if any
     */
    explicit MainWindow(QWidget *parent = 0);

    /*!
     * Destructs the object and free memory, if any
     */
    ~MainWindow();

public slots:
    /*!
     * Method that is called when the user requests
     * the creation of a new finite automata.
     * Please note that this method asks the user for
     * a name, suggesting a name not used yet, and
     * simply returns if the user cancels the creation
     * of the finite automata.
     */
    void createNewAutomata();

    /*!
     * Method that is called when the user requests
     * the loading of a existing Finite Automata
     * Please note that this method asks the user for
     * a name, suggesting a name not used yet, and
     * simply returns if the user cancels the creation
     * of the tab for that finite automata.
     */
    void createNewAutomata(FiniteAutomata f);

    /*!
     * Method that is called when the user requests
     * the creation of a new regular expression.
     * Please note that this method asks the user for
     * a name, suggesting a name not used yet, and
     * simply returns if the user cancels the creation
     * of the regular expression.
     */
    void createNewRegularExpression();

    /*!
     * Set the name of a tab, asking for the user to enter
     * it
     */
    void setTabName();

    /*!
     * Duplicate the current tab, copying the content to another
     * tab
     */
    void duplicateTab();

    /*!
     * Do the union between two automatas, asking for the other
     * automata first
     */
    void doUnion();

    /*!
     * Do the intersection between two automatas, asking for the other
     * automata first
     */
    void doIntersection();

    /*!
     * Check if the selected automata is contained in another automata,
     * asking for that other automata first
     */
    void checkContainment();

    /*!
     * Check if the selected automata is equivalent with another automata,
     * asking for that other automata first
     */
    void checkEquivalence();

    /*!
     * Do the difference between two automatas, asking for the other
     * automata first
     */
    void doDifference();

    /*!
     * Do the complement of the selected automata
     */
    void doComplement();

    /*!
     * Do the conversion to Automata, but only if it is
     * a Regular Expression
     */
    void doConversionToAutomata();

    /*!
     * Check if a string is accepted by a Finite Automata or
     * Regular Expresssion
     */
    void doAccept();

    /*!
     * Do the minimization of the selected automata
     */
    void doMinimization();

    /*!
     * Do the determinization of the selected automata
     */
    void doDeterminization();

    /*!
     * Fix the menus, enabling and disabling options as needed
     *
     * @param currentIndex The actual tab enabled
     */
    void fixMenus(int currentIndex);

    /*!
     * Close the table indicated in the index
     *
     * @param tab The index of the tab to close
     */
    void closeTab(int tab);
private:
    /*!
     * Find a free name for a tab
     *
     * @param prefix The prefix of the tab
     * @return A name that was not used
     */
    QString findFreeName(QString prefix);

    /*!
     * Check if a tab for an operation is already opened. If it is,
     * set that tab as current. If it is not, create a new OperationTab,
     * set all the attributes and set that as current.
     *
     * \param opName The name of the tab
     * \param title The title of the tab
     * \return  An OperationTab object
     */
    OperationTab *getOperationTab(QString opName, QString title);

    /*!
     * Check if a tab already exists
     *
     * @param tabName The name to check for existence
     * @return true if the tab already exists, false otherwise
     */
    bool tabAlreadyExists(QString tabName);

    /*!
     * Show the data about a Regular Expression or Finite Automata
     * before computing the algorithm
     *
     * @param op   The OperationTab where the operation is running
     * \param tab  The Tab that should be printed
     * \param name The name of the Tab that should be printed
     */
    void showAutomata(OperationTab *op, AutomataTab *tab, QString name);

    /*!
     * Return a list of the items actually opened
     * in the window
     *
     * @return A list of the names of items actually opened
     */
    QStringList getItemsNames();

    Ui::MainWindow *ui; //!< A pointer to the Qt Main Window
    QTabWidget *tabWidget; //!< A pointer to the tab widget present in the main window
};

#endif // MAINWINDOW_H
