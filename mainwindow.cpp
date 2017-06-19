#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "finite_automata_tab.h"
#include "regular_expression.h"
#include <QInputDialog>
#include "finite_automata.h"
#include "operation_tab.h"

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

QStringList MainWindow::getAutomatasNames() {
    QStringList result;
    int tabCount = tabWidget->count();
    for (int tab=0; tab<tabCount; tab++) {
        QWidget *widget = tabWidget->widget(tab);
        if (dynamic_cast<FiniteAutomataTab*>(widget)) {
            result.append(tabWidget->tabText(tab));
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
    FiniteAutomataTab *fTab = dynamic_cast<FiniteAutomataTab*>(tab);
    if (!fTab) {
        return;
    }
    bool ok;
    QString name = tabWidget->tabText(currentIndex);
    QString f2Name = QInputDialog::getItem(
                this,
                "Select the automata",
                "Select the automata to realize the union with '"+name+"':",
                getAutomatasNames(),
                0,
                false,
                &ok);
    if (!ok) {
        return;
    }
    QString opName = "op_"+name+"_union_"+f2Name;
    OperationTab *op = tabWidget->findChild<OperationTab*>(opName);
    if (op) {
        tabWidget->setCurrentWidget(op);
        return;
    }
    FiniteAutomata f = fTab->toAutomata();
    FiniteAutomataTab *f2Tab = tabWidget->findChild<FiniteAutomataTab*>("tab_"+f2Name);
    if (!f2Tab) {
        return;
    }
    FiniteAutomata f2 = f2Tab->toAutomata();
    QScrollArea *scroll = new QScrollArea(this);
    op = new OperationTab(this);
    scroll->setWidget(op);
    scroll->setWidgetResizable(true);
    op->setObjectName(opName);
    op->addStep(this, f, "This is the automata '"+name+"':");
    op->addStep(this, f2, "This is the automata '"+f2Name+"':");
    op->addStep(this, f.doUnion(f2), "This is the result of the union between '"+name+"' and '"+f2Name+"':");
    int opIndex = tabWidget->addTab(scroll, "Union between '"+name+"' and '"+f2Name+"'");
    tabWidget->setCurrentIndex(opIndex);
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
    FiniteAutomataTab *fTab = dynamic_cast<FiniteAutomataTab*>(tab);
    if (!fTab) {
        return;
    }
    bool ok;
    QString name = tabWidget->tabText(currentIndex);
    QString f2Name = QInputDialog::getItem(
                this,
                "Select the automata",
                "Select the automata to realize the intersection with '"+name+"':",
                getAutomatasNames(),
                0,
                false,
                &ok);
    if (!ok) {
        return;
    }
    QString opName = "op_"+name+"_intersection_"+f2Name;
    OperationTab *op = tabWidget->findChild<OperationTab*>(opName);
    if (op) {
        tabWidget->setCurrentWidget(op);
        return;
    }
    FiniteAutomata f = fTab->toAutomata();
    cout << "tab_"+f2Name.toStdString() << endl;
    FiniteAutomataTab *f2Tab = tabWidget->findChild<FiniteAutomataTab*>("tab_"+f2Name);
    if (!f2Tab) {
        return;
    }
    FiniteAutomata f2 = f2Tab->toAutomata();
    QScrollArea *scroll = new QScrollArea(this);
    op = new OperationTab(this);
    scroll->setWidget(op);
    scroll->setWidgetResizable(true);
    op->setObjectName(opName);
    op->addStep(this, f, "This is the automata '"+name+"':");
    op->addStep(this, f2, "This is the automata '"+f2Name+"':");
    op->addStep(this, f.doComplement(), "This is the complement of the automata '"+name+"':");
    op->addStep(this, f2.doComplement(), "This is the complement of the automata '"+f2Name+"':");
    op->addStep(this, f.doComplement().doIntersection(f2.doComplement()), "Union of the complement of '"+name+"' with the complement of '"+f2Name+"':");
    op->addStep(this, f.doIntersection(f2), "Complement of the union between the complement of '"+name+"' and the complement of '"+f2Name+"':");
    int opIndex = tabWidget->addTab(scroll, "Union between '"+name+"' and '"+f2Name+"'");
    tabWidget->setCurrentIndex(opIndex);
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
    if (!(dynamic_cast<FiniteAutomataTab* >(tab) || dynamic_cast<OperationTab*>(tab))) {
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
        if (!used && name != actualName) {
            break;
        }
    }
    tab->setObjectName("tab_"+name);
    tabWidget->setTabText(currentIndex, name);
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
    FiniteAutomataTab *tab = new FiniteAutomataTab();
    tab->setObjectName("tab_"+name);
    this->tabWidget->addTab(tab, name);
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
    return tabWidget->findChild<FiniteAutomataTab*>("tab_"+name) != 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}
