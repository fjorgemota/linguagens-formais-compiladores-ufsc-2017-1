#include "finite_automata_tab.h"
#include <QGridLayout>

FiniteAutomataTab::FiniteAutomataTab(QWidget *parent) : AutomataTab(parent)
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

void FiniteAutomataTab::fromTab(FiniteAutomataTab &f) {
    this->table->fromTable(f.table);
}

FiniteAutomata FiniteAutomataTab::toAutomata() {
    return this->table->toAutomata();
}

bool FiniteAutomataTab::isValid() {
    return this->table->isValid();
}
