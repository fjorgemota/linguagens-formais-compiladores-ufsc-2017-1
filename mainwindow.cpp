#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "finite_automata_tab.h"
#include "regular_expression.h"
#include <QInputDialog>
#include "finite_automata.h"
#include "operation_tab.h"
#include "regular_expression_tab.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    RegularExpression re("(ab|ac)*a?|(ba?c)*");

    FiniteAutomata f = re.getAutomata();
    tabWidget = this->findChild<QTabWidget*>("tabWidget");
//    FiniteAutomataTable *w = new FiniteAutomataTable();
//    w->fromAutomata(f);
//    cout << f.toASCIITable() << endl;
//    cout << w->toAutomata().toASCIITable() << endl;
//    cout << w->toAutomata().isEquivalent(f) << endl;
    FiniteAutomataTab *tab = new FiniteAutomataTab();
    tab->fromAutomata(f);
    tab->setObjectName("tab_Test");
    tabWidget->addTab(tab, "Test");
    fixMenus(0);
}

void MainWindow::createNewAutomata() {
    FiniteAutomata f;
    createNewAutomata(f);
}

QStringList MainWindow::getItemsNames() {
    QStringList result;
    int tabCount = tabWidget->count();
    for (int tab=0; tab<tabCount; tab++) {
        QWidget *widget = tabWidget->widget(tab);
        if (AutomataTab *tabItem = dynamic_cast<AutomataTab*>(widget)) {
            if (tabItem->isValid()) {
                result.append(tabWidget->tabText(tab));
            }
        }
    }
    return result;
}

void MainWindow::doUnion() {
    int currentIndex = tabWidget->currentIndex();
    if (currentIndex < 0) {
        return;
    }
    QWidget *tab = tabWidget->widget(currentIndex);
    AutomataTab *fTab = dynamic_cast<AutomataTab*>(tab);
    if (!fTab) {
        return;
    }
    if (!fTab->isValid()) {
        QMessageBox::critical(this, "Invalid automata or regular expression", "The item actually selected is invalid, please, fix the problems before doing this operation", QMessageBox::Ok);
        return;
    }
    bool ok;
    QString name = tabWidget->tabText(currentIndex);
    QString f2Name = QInputDialog::getItem(
                this,
                "Select the automata or regular expression",
                "Select the automata or regular expression to realize the union with '"+name+"':",
                getItemsNames(),
                0,
                false,
                &ok);
    if (!ok) {
        return;
    }
    OperationTab *op = getOperationTab("op_"+name+"_union_"+f2Name, "Union between '"+name+"' and '"+f2Name+"'");
    if (!op) {
        return;
    }
    FiniteAutomata f = fTab->toAutomata();
    AutomataTab *f2Tab = tabWidget->findChild<AutomataTab*>("tab_"+f2Name);
    if (!f2Tab) {
        return;
    }
    FiniteAutomata f2 = f2Tab->toAutomata();
    showAutomata(op, fTab, name);
    showAutomata(op, f2Tab, f2Name);
    op->addStep(this, f.doUnion(f2), "This is the result of the union between '"+name+"' and '"+f2Name+"':");
}


void MainWindow::closeTab(int tab) {
    if (tab == 0) {
        return;
    }
    QWidget *widget = tabWidget->widget(tab);
    // Destroy the associated page
    delete widget;
    // Remove the tab
    tabWidget->removeTab(tab);
}

void MainWindow::doIntersection() {
    int currentIndex = tabWidget->currentIndex();
    if (currentIndex < 0) {
        return;
    }
    QWidget *tab = tabWidget->widget(currentIndex);
    AutomataTab *fTab = dynamic_cast<AutomataTab*>(tab);
    if (!fTab) {
        return;
    }
    if (!fTab->isValid()) {
        QMessageBox::critical(this, "Invalid automata or regular expression", "The automata or regular expression actually selected is invalid, please, fix the problems before doing this operation", QMessageBox::Ok);
        return;
    }
    bool ok;
    QString name = tabWidget->tabText(currentIndex);
    QString f2Name = QInputDialog::getItem(
                this,
                "Select the automata or regular expression",
                "Select the automata or regular expression to realize the intersection with '"+name+"':",
                getItemsNames(),
                0,
                false,
                &ok);
    if (!ok) {
        return;
    }
    OperationTab *op = getOperationTab("op_"+name+"_intersection_"+f2Name, "Intersection between '"+name+"' and '"+f2Name+"'");
    if (!op) {
        return;
    }
    FiniteAutomata f = fTab->toAutomata();
    AutomataTab *f2Tab = tabWidget->findChild<AutomataTab*>("tab_"+f2Name);
    if (!f2Tab) {
        return;
    }
    FiniteAutomata f2 = f2Tab->toAutomata();
    showAutomata(op, fTab, name);
    showAutomata(op, f2Tab, f2Name);
    op->addStep(this, f.doComplement(), "This is the complement of the automata '"+name+"':");
    op->addStep(this, f2.doComplement(), "This is the complement of the automata '"+f2Name+"':");
    op->addStep(this, f.doComplement().doUnion(f2.doComplement()), "Union of the complement of '"+name+"' with the complement of '"+f2Name+"':");
    op->addStep(this, f.doIntersection(f2), "Complement of the union between the complement of '"+name+"' and the complement of '"+f2Name+"'  (in other words, the result of the intersection):");
}

void MainWindow::doDifference() {
    int currentIndex = tabWidget->currentIndex();
    if (currentIndex < 0) {
        return;
    }
    QWidget *tab = tabWidget->widget(currentIndex);
    AutomataTab *fTab = dynamic_cast<AutomataTab*>(tab);
    if (!fTab) {
        return;
    }
    if (!fTab->isValid()) {
        QMessageBox::critical(this, "Invalid automata or regular expression", "The automata or regular expression actually selected is invalid, please, fix the problems before doing this operation", QMessageBox::Ok);
        return;
    }
    bool ok;
    QString name = tabWidget->tabText(currentIndex);
    QString f2Name = QInputDialog::getItem(
                this,
                "Select the automata or regular expression",
                "Select the automata or regular expression to compute the difference with '"+name+"':",
                getItemsNames(),
                0,
                false,
                &ok);
    if (!ok) {
        return;
    }
    OperationTab *op = getOperationTab("op_"+name+"_difference_"+f2Name, "Difference between '"+name+"' and '"+f2Name+"'");
    if (!op) {
        return;
    }
    FiniteAutomata f = fTab->toAutomata();
    AutomataTab *f2Tab = tabWidget->findChild<AutomataTab*>("tab_"+f2Name);
    if (!f2Tab) {
        return;
    }
    FiniteAutomata f2 = f2Tab->toAutomata();

    showAutomata(op, fTab, name);
    showAutomata(op, f2Tab, f2Name);
    op->addStep(this, f2.doComplement(), "This is the complement of the automata '"+f2Name+"':");
    op->addStep("Then, we do the intersection between '"+name+"' and the complement of '"+f2Name+"'..");
    op->addStep(this, f.doComplement(), "This is the complement of the automata '"+name+"':");
    op->addStep(this, f2.doComplement().doComplement(), "This is the complement of the complement of the automata '"+f2Name+"':");
    op->addStep(this, f.doComplement().doUnion(f2.doComplement().doComplement()), "Union of the complement of '"+name+"' with the complement of the complement of '"+f2Name+"':");
    op->addStep(this, f.doDifference(f2), "Complement of the union between the complement of '"+name+"' and the complement of the complement of '"+f2Name+"' (in other words, the result of the difference):");
}

OperationTab* MainWindow::getOperationTab(QString opName, QString title) {
    OperationTab *op = tabWidget->findChild<OperationTab*>(opName);
    QString scrollName = "scroll_"+opName;
    if (op) {
        delete op;
        int total = tabWidget->count();
        for (int c=0; c<total; c++) {
            if (tabWidget->widget(c)->objectName() == scrollName) {
                tabWidget->removeTab(c);
                break;
            }
        }
    }
    QScrollArea *scroll = new QScrollArea(this);
    op = new OperationTab(this);
    scroll->setWidget(op);
    scroll->setWidgetResizable(true);
    scroll->setObjectName(scrollName);
    op->setObjectName(opName);
    int opIndex = tabWidget->addTab(scroll, title);
    tabWidget->setCurrentIndex(opIndex);
    return op;
}

void MainWindow::checkContainment() {
    int currentIndex = tabWidget->currentIndex();
    if (currentIndex < 0) {
        return;
    }
    QString name = tabWidget->tabText(currentIndex);
    QWidget *tab = tabWidget->widget(currentIndex);
    AutomataTab *fTab = dynamic_cast<AutomataTab*>(tab);
    if (!fTab) {
        return;
    }
    if (!fTab->isValid()) {
        QMessageBox::critical(this, "Invalid automata or regular expression", "The automata or regular expression actually selected is invalid, please, fix the problems before doing this operation", QMessageBox::Ok);
        return;
    }
    bool ok;
    QString f2Name = QInputDialog::getItem(
                this,
                "Select the automata or regular expression",
                "Select the automata to check if '"+name+"' inside it:",
                getItemsNames(),
                0,
                false,
                &ok);
    if (!ok) {
        return;
    }
    OperationTab *op = getOperationTab("op_"+name+"_containment_"+f2Name, "Containment of '"+name+"' inside '"+f2Name+"'");
    if (!op) {
        return;
    }
    FiniteAutomata f = fTab->toAutomata();
    AutomataTab *f2Tab = tabWidget->findChild<AutomataTab*>("tab_"+f2Name);
    if (!f2Tab) {
        return;
    }
    FiniteAutomata f2 = f2Tab->toAutomata();
    showAutomata(op, fTab, name);
    showAutomata(op, f2Tab, f2Name);
    op->addStep(this, f2.doComplement(), "This is the complement of the automata '"+f2Name+"':");
    op->addStep("Then, we do the intersection between '"+name+"' and the complement of '"+f2Name+"'..");
    op->addStep(this, f.doComplement(), "This is the complement of the automata '"+name+"':");
    op->addStep(this, f2.doComplement().doComplement(), "This is the complement of the complement of the automata '"+f2Name+"':");
    op->addStep(this, f.doComplement().doUnion(f2.doComplement().doComplement()), "Union of the complement of '"+name+"' with the complement of the complement of '"+f2Name+"':");
    op->addStep(this, f.doDifference(f2), "Complement of the union between the complement of '"+name+"' and the complement of the complement of '"+f2Name+"' (in other words, the result of the difference):");
    QString s = "Then, we check if the result of the difference between '"+name+"' and '"+f2Name+"' is empty. ";
    if (f.isContained(f2)) {
        s += "In this case, it was. So we can conclude that '"+name+"' is contained inside '"+f2Name+"'.";
    } else {
        s += "In this case, it was not. So we can conclude that '"+name+"' is NOT contained inside '"+f2Name+"'.";
    }
    op->addStep(s);
}

void MainWindow::showAutomata(OperationTab *op, AutomataTab *tab, QString name) {
    FiniteAutomata f = tab->toAutomata();
    if (RegularExpressionTab* re = dynamic_cast<RegularExpressionTab*>(tab)) {
        op->addStep("Well, '"+name+"' was a Regular Expression: '"+re->getRegularExpression()+"', so we have to create a Finite Automata to that Regular Expression. ");
        op->addStep(this, f, "This is the automata '"+name+"' corresponding to that Regular Expression:");
    } else {
        op->addStep(this, f, "This is the automata '"+name+"':");
    }
}

void MainWindow::checkEquivalence() {
    int currentIndex = tabWidget->currentIndex();
    if (currentIndex < 0) {
        return;
    }
    QWidget *tab = tabWidget->widget(currentIndex);
    AutomataTab *fTab = dynamic_cast<AutomataTab*>(tab);
    if (!fTab) {
        return;
    }
    if (!fTab->isValid()) {
        QMessageBox::critical(this, "Invalid automata or regular expression", "The automata or regular expression actually selected is invalid, please, fix the problems before doing this operation", QMessageBox::Ok);
        return;
    }
    bool ok;
    QString name = tabWidget->tabText(currentIndex);
    QString f2Name = QInputDialog::getItem(
                this,
                "Select the automata or regular expression",
                "Select the automata or regular expression to check if it is equivalent with '"+name+"':",
                getItemsNames(),
                0,
                false,
                &ok);
    if (!ok) {
        return;
    }
    OperationTab *op = getOperationTab("op_"+name+"_equivalence_"+f2Name, "Equivalence between '"+name+"' and '"+f2Name+"'");
    if (!op) {
        return;
    }
    FiniteAutomata f = fTab->toAutomata();
    AutomataTab *f2Tab = tabWidget->findChild<AutomataTab*>("tab_"+f2Name);
    if (!f2Tab) {
        return;
    }
    FiniteAutomata f2 = f2Tab->toAutomata();
    showAutomata(op, fTab, name);
    showAutomata(op, f2Tab, f2Name);
    op->addStep(this, f2.doComplement(), "This is the complement of the automata '"+f2Name+"':");
    op->addStep("Then, we do the intersection between '"+name+"' and the complement of '"+f2Name+"'..");
    op->addStep(this, f.doComplement(), "This is the complement of the automata '"+name+"':");
    op->addStep(this, f2.doComplement().doComplement(), "This is the complement of the complement of the automata '"+f2Name+"':");
    op->addStep(this, f.doComplement().doUnion(f2.doComplement().doComplement()), "Union of the complement of '"+name+"' with the complement of the complement of '"+f2Name+"':");
    op->addStep(this, f.doDifference(f2), "Complement of the union between the complement of '"+name+"' and the complement of the complement of '"+f2Name+"' (in other words, the result of the difference):");
    QString s = "Then, we check if the result of the difference between '"+name+"' and '"+f2Name+"' is empty. ";
    if (f.isContained(f2)) {
        s += "In this case, it is. So we can conclude that '"+name+"' is contained inside '"+f2Name+"'.";
    } else {
        s += "In this case, it is not. So we can conclude that '"+name+"' is NOT contained inside '"+f2Name+"'.";
    }
    op->addStep(s);
    if (f.isContained(f2)) {
        op->addStep("Then, we need to check if '"+f2Name+"' is contained inside '"+name+"' to check if these automata are equivalent:");
        op->addStep(this, f.doComplement(), "This is the complement of the automata '"+name+"':");
        op->addStep("Then, we do the intersection between '"+f2Name+"' and the complement of '"+name+"'..");
        op->addStep(this, f2.doComplement(), "This is the complement of the automata '"+f2Name+"':");
        op->addStep(this, f.doComplement().doComplement(), "This is the complement of the complement of the automata '"+name+"':");
        op->addStep(this, f2.doComplement().doUnion(f.doComplement().doComplement()), "Union of the complement of '"+f2Name+"' with the complement of the complement of '"+name+"':");
        op->addStep(this, f2.doDifference(f), "Complement of the union between the complement of '"+f2Name+"' and the complement of the complement of '"+name+"' (in other words, the result of the difference):");
        s = "Then, we check if the result of the difference between '"+f2Name+"' and '"+name+"' is empty. ";
        if (f2.isContained(f)) {
            s += "In this case, it is. So we can conclude that '"+f2Name+"' is contained inside '"+name+"'.";
        } else {
            s += "In this case, it is not. So we can conclude that '"+f2Name+"' is NOT contained inside '"+name+"'.";
        }
    }
    if (f.isEquivalent(f2)) {
        op->addStep("Well, because '"+name+"' is contained inside '"+f2Name+"' and '"+f2Name+"' is contained inside '"+name+"'. So...these two automata are equivalent! :D");
    } else {
        op->addStep("So we can conclude that '"+name+"' is NOT equivalent to '"+f2Name+"'.");
    }
}

void MainWindow::doComplement() {
    int currentIndex = tabWidget->currentIndex();
    if (currentIndex < 0) {
        return;
    }
    QWidget *tab = tabWidget->widget(currentIndex);
    AutomataTab *fTab = dynamic_cast<AutomataTab*>(tab);
    if (!fTab) {
        return;
    }
    if (!fTab->isValid()) {
        QMessageBox::critical(this, "Invalid automata or regular expression", "The automata  or regular expression actually selected is invalid, please, fix the problems before doing this operation", QMessageBox::Ok);
        return;
    }
    QString name = tabWidget->tabText(currentIndex);
    OperationTab *op = getOperationTab("op_"+name+"_complement", "Complement of '"+name+"'");
    if (!op) {
        return;
    }
    FiniteAutomata f = fTab->toAutomata();
    showAutomata(op, fTab, name);
    op->addStep(this, f.doComplement(), "This is the complement of the automata '"+name+"':");
}


void MainWindow::doConversionToAutomata() {
    int currentIndex = tabWidget->currentIndex();
    if (currentIndex < 0) {
        return;
    }
    QWidget *tab = tabWidget->widget(currentIndex);
    RegularExpressionTab *rTab = dynamic_cast<RegularExpressionTab*>(tab);
    if (!rTab) {
        return;
    }
    if (!rTab->isValid()) {
        QMessageBox::critical(this, "Invalid regular expression", "The regular expression actually selected is invalid, please, fix the problems before doing this operation", QMessageBox::Ok);
        return;
    }
    QString name = tabWidget->tabText(currentIndex);
    OperationTab *op = getOperationTab("op_"+name+"_automata", "Automata for RE '"+name+"'");
    if (!op) {
        return;
    }
    FiniteAutomata f = rTab->toAutomata();
    op->addStep("This is the Regular Expression '"+name+"': '"+rTab->getRegularExpression()+"'");
    op->addStep(this, f, "This is the automata for that Regular Expression.");
}

void MainWindow::doAccept() {
    int currentIndex = tabWidget->currentIndex();
    if (currentIndex < 0) {
        return;
    }
    QWidget *tab = tabWidget->widget(currentIndex);
    AutomataTab *aTab = dynamic_cast<AutomataTab*>(tab);
    if (!aTab) {
        return;
    }
    if (!aTab->isValid()) {
        QMessageBox::critical(this, "Invalid finite automata or regular expression", "The finite automata or regular expression actually selected is invalid, please, fix the problems before doing this operation", QMessageBox::Ok);
        return;
    }
    QString name = tabWidget->tabText(currentIndex);
    bool ok;
    QString s = QInputDialog::getText(this, "Enter the string to check", "Enter the string to check", QLineEdit::Normal, "", &ok);
    if (!ok) {
        return;
    }
    if (aTab->toAutomata().accepts(s.toStdString())) {
        QMessageBox::information(this, "Accepted", "The string '"+s+"' is accepted by '"+name+"'", QMessageBox::Ok);
    } else {
        QMessageBox::warning(this, "Not Accepted", "The string '"+s+"' is NOT accepted by '"+name+"'", QMessageBox::Ok);
    }
}

void MainWindow::doMinimization() {
    int currentIndex = tabWidget->currentIndex();
    if (currentIndex < 0) {
        return;
    }
    QWidget *tab = tabWidget->widget(currentIndex);
    FiniteAutomataTab *fTab = dynamic_cast<FiniteAutomataTab*>(tab);
    if (!fTab) {
        return;
    }
    if (!fTab->isValid()) {
        QMessageBox::critical(this, "Invalid automata", "The automata actually selected is invalid, please, fix the problems before doing this operation", QMessageBox::Ok);
        return;
    }
    QString name = tabWidget->tabText(currentIndex);
    OperationTab *op = getOperationTab("op_"+name+"_minimization", "Minimization of '"+name+"'");
    if (!op) {
        return;
    }
    FiniteAutomata f = fTab->toAutomata();
    op->addStep(this, f, "This is the automata '"+name+"':");
    op->addStep(this, f.removeDeadStates(), "This is the automata '"+name+"' without dead states:");
    op->addStep(this, f.removeDeadStates().removeUnreachableStates(), "This is the automata '"+name+"' without dead states and without unreachable states:");
    if (f.isDeterministic()) {
        op->addStep(this, f.removeDeadStates().removeUnreachableStates().removeEquivalentStates(), "This is the automata '"+name+"' without dead states, without unreachable states, and without equivalent states:");
    } else {
        op->addStep("The step of removing equivalent states was not done because the automata is not deterministic. Please, determinize it and minimize it to remove equivalent states too.");
    }
}


void MainWindow::doDeterminization() {
    int currentIndex = tabWidget->currentIndex();
    if (currentIndex < 0) {
        return;
    }
    QWidget *tab = tabWidget->widget(currentIndex);
    FiniteAutomataTab *fTab = dynamic_cast<FiniteAutomataTab*>(tab);
    if (!fTab) {
        return;
    }
    if (!fTab->isValid()) {
        QMessageBox::critical(this, "Invalid automata", "The automata actually selected is invalid, please, fix the problems or select another automata", QMessageBox::Ok);
        return;
    }
    QString name = tabWidget->tabText(currentIndex);
    FiniteAutomata f = fTab->toAutomata();
    if (f.isDeterministic()) {
        QMessageBox::information(this, "The automata is already deterministic", "The automata actually selected is already deterministic.", QMessageBox::Ok);
        return;
    }
    OperationTab *op = getOperationTab("op_"+name+"_determinization", "Determinization of '"+name+"'");
    op->addStep(this, f, "This is the automata '"+name+"':");
    op->addStep(this, f.determinize(), "This is the automata '"+name+"', determined:");
}

void MainWindow::fixMenus(int currentIndex) {
    if (currentIndex < 0) {
        return;
    }
    QWidget *tab = tabWidget->widget(currentIndex);
    QList<QAction*> operations = menuBar()->findChild<QMenu*>("menuOperations")->actions();
    QList<QAction*> tabs = menuBar()->findChild<QMenu*>("menuTabs")->actions();
    if (dynamic_cast<FiniteAutomataTab*>(tab)) {
        for (QAction* op: operations) {
            op->setEnabled(op->objectName() != "actionConvert_to_Automata");
        }
        for (QAction* op: tabs) {
            op->setEnabled(true);
        }
    } else if(dynamic_cast<RegularExpressionTab*>(tab)) {
        for (QAction* op: operations) {
            op->setEnabled(op->objectName() != "actionDeterminize" &&
                    op->objectName() != "actionMinimize");
        }
        for (QAction* op: tabs) {
            op->setEnabled(true);
        }
    } else {
        for (QAction* op: operations) {
            op->setEnabled(false);
        }
        for (QAction* op: tabs) {
            op->setEnabled(false);
        }
    }
}

void MainWindow::setTabName() {
    int currentIndex = tabWidget->currentIndex();
    if (currentIndex < 0) {
        return;
    }
    QWidget *tab = tabWidget->widget(currentIndex);
    if (!(dynamic_cast<AutomataTab* >(tab) || dynamic_cast<OperationTab*>(tab))) {
        return;
    }
    QString actualName = tabWidget->tabText(currentIndex);
    QString name;
    bool used;
    while (true) {
        QString message;
        if (used) {
            message.append("That name was already used, please, choose another:");
        } else {
            message.append("Enter the name of the tab:");
        }
        bool ok;
        name = QInputDialog::getText(this, "Enter the name", message, QLineEdit::Normal, actualName, &ok);
        if (!ok) {
            return;
        }
        if (name.isEmpty()) {
            name = actualName;
        }
        used = tabAlreadyExists(name);
        if (!used || name == actualName) {
            break;
        }
    }
    tab->setObjectName("tab_"+name);
    tabWidget->setTabText(currentIndex, name);
}

void MainWindow::duplicateTab() {
    int currentIndex = tabWidget->currentIndex();
    if (currentIndex < 0) {
        return;
    }
    QWidget *tab = tabWidget->widget(currentIndex);
    bool ok, used;
    QString freeName, name;

    if (FiniteAutomataTab* fa = dynamic_cast<FiniteAutomataTab*>(tab)) {
        freeName = findFreeName("FA");
        while (true) {
            QString message;
            if (used) {
                message.append("That name was already used, please, choose another:");
            } else {
                message.append("Enter the name of the new Finite Automata");
            }
            name = QInputDialog::getText(this, "Enter the name", message, QLineEdit::Normal, freeName, &ok);
            if (!ok) {
                return;
            }
            if (name.isEmpty()) {
                name = freeName;
            }
            used = tabAlreadyExists(name);
            if (!used) {
                break;
            }
        }
        FiniteAutomataTab *newTab = new FiniteAutomataTab();
        newTab->fromTab(*fa);
        newTab->setObjectName("tab_"+name);
        this->tabWidget->setCurrentIndex(this->tabWidget->addTab(newTab, name));
    } else if (RegularExpressionTab* re = dynamic_cast<RegularExpressionTab*>(tab)) {
        freeName = findFreeName("RE");
        while (true) {
            QString message;
            if (used) {
                message.append("That name was already used, please, choose another:");
            } else {
                message.append("Enter the name of the new Regular Expression");
            }
            name = QInputDialog::getText(this, "Enter the name", message, QLineEdit::Normal, freeName, &ok);
            if (!ok) {
                return;
            }
            if (name.isEmpty()) {
                name = freeName;
            }
            used = tabAlreadyExists(name);
            if (!used) {
                break;
            }
        }
        RegularExpressionTab *newTab = new RegularExpressionTab();
        newTab->setObjectName("tab_"+name);
        RegularExpression r = re->toRegularExpression();
        newTab->fromRegularExpression(r);
        this->tabWidget->addTab(newTab, name);
    }
}

void MainWindow::createNewAutomata(FiniteAutomata f) {
    bool ok;
    QString freeName = findFreeName("FA");
    QString name;
    bool used;
    while (true) {
        QString message;
        if (used) {
            message.append("That name was already used, please, choose another:");
        } else {
            message.append("Enter the name of the new Finite Automata");
        }
        name = QInputDialog::getText(this, "Enter the name", message, QLineEdit::Normal, freeName, &ok);
        if (!ok) {
            return;
        }
        if (name.isEmpty()) {
            name = freeName;
        }
        used = tabAlreadyExists(name);
        if (!used) {
            break;
        }
    }
    FiniteAutomataTab *tab = new FiniteAutomataTab();
    tab->fromAutomata(f);
    tab->setObjectName("tab_"+name);
    this->tabWidget->setCurrentIndex(this->tabWidget->addTab(tab, name));
}

void MainWindow::createNewRegularExpression() {
    bool ok;
    QString freeName = findFreeName("RE");
    QString name;
    bool used;
    while (true) {
        QString message;
        if (used) {
            message.append("That name was already used, please, choose another:");
        } else {
            message.append("Enter the name of the new Regular Expression");
        }
        name = QInputDialog::getText(this, "Enter the name", message, QLineEdit::Normal, freeName, &ok);
        if (!ok) {
            return;
        }
        if (name.isEmpty()) {
            name = freeName;
        }
        used = tabAlreadyExists(name);
        if (!used) {
            break;
        }
    }
    RegularExpressionTab *tab = new RegularExpressionTab();
    tab->setObjectName("tab_"+name);
    this->tabWidget->setCurrentIndex(this->tabWidget->addTab(tab, name));
}

QString MainWindow::findFreeName(QString prefix) {
    int i = 1;
    bool isFree = false;
    QString name;
    do {
        name = prefix + QString::number(i);
        isFree = !tabAlreadyExists(name);
        i++;
    } while (!isFree);
    return name;
}

bool MainWindow::tabAlreadyExists(QString name) {
    return tabWidget->findChild<QWidget*>("tab_"+name) != 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}
