#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include "finite_automata.h"


namespace Ui {
class MainWindow;
}

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
     * Do the difference between two automatas, asking for the other
     * automata first
     */
    void doDifference();

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
     * Check if a tab already exists
     *
     * @param tabName The name to check for existence
     * @return true if the tab already exists, false otherwise
     */
    bool tabAlreadyExists(QString tabName);

    /*!
     * Return a list of the automatas actually opened
     * in the window
     *
     * @return A list of the names of automatas actually opened
     */
    QStringList getAutomatasNames();

    Ui::MainWindow *ui; //!< A pointer to the Qt Main Window
    QTabWidget *tabWidget; //!< A pointer to the tab widget present in the main window
};

#endif // MAINWINDOW_H
