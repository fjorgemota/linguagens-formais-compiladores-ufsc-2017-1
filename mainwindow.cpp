#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "finite_automata_table.h"
#include "regular_expression.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    RegularExpression re("(ab|ac)*a?|(ba?c)*");

    FiniteAutomata f = re.getAutomata();
    QGridLayout* layout = this->findChild<QGridLayout*>("gridLayout");
    FiniteAutomataTable *w = new FiniteAutomataTable();
    w->fromAutomata(f);
//    cout << f.toASCIITable() << endl;
//    cout << w->toAutomata().toASCIITable() << endl;
//    cout << w->toAutomata().isEquivalent(f) << endl;
    layout->addWidget(w);
}

MainWindow::~MainWindow()
{
    delete ui;
}
