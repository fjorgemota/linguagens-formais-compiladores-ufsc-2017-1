#include "operation_tab.h"

OperationTab::OperationTab(QWidget *widget) : QWidget(widget)
{
    gridLayout = new QGridLayout();
    setLayout(gridLayout);
}

void OperationTab::addStep(MainWindow *window, FiniteAutomata f, QString title) {
    int rowCount = this->gridLayout->rowCount();
    QLabel *label = new QLabel(title, this);
    label->setWordWrap(true);
    gridLayout->addWidget(label, rowCount, 0);
    FiniteAutomataTable *table = new FiniteAutomataTable();
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->fromAutomata(f);
    int rowHeight = 0;
    int tableRowCount = table->rowCount()-1;
    for (int c=0; c<tableRowCount; c++) {
        rowHeight += table->rowHeight(c);
    }
    table->setMinimumHeight(rowHeight);
    table->setMaximumHeight(rowHeight);
    table->setReadOnly();
    table->removeColumn(table->columnCount()-1);
    table->removeRow(tableRowCount);
    gridLayout->addWidget(table, rowCount+1, 0);
    MoveToTabButton *button = new MoveToTabButton(this, window, f);
    gridLayout->addWidget(button, rowCount, 1, 2, 1);
}


void OperationTab::addStep(QString title) {
    int rowCount = this->gridLayout->rowCount();
    QLabel *label = new QLabel(title, this);
    label->setWordWrap(true);
    gridLayout->addWidget(label, rowCount, 0, 1, 2);
}
