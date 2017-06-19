#include "finite_automata_tab.h"
#include <QGridLayout>

FiniteAutomataTab::FiniteAutomataTab(QWidget *parent) : QWidget(parent)
{
    QLayout *layout = this->layout();
    if (!layout) {
        layout = new QGridLayout(this);
    }
    this->table = new FiniteAutomataTable(this);
    layout->addWidget(this->table);
}


void FiniteAutomataTab::fromAutomata(FiniteAutomata &f) {
    this->table->fromAutomata(f);
}


FiniteAutomata FiniteAutomataTab::toAutomata() {
    return this->table->toAutomata();
}
