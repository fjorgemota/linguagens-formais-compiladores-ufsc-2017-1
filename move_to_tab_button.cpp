#include "move_to_tab_button.h"

MoveToTabButton::MoveToTabButton(QWidget *parent, MainWindow *window, FiniteAutomata f) : QPushButton(parent), f(f), window(window)
{
    connect(this, SIGNAL(clicked(bool)), this, SLOT(moveToTab()));
    this->setText("Move to Tab");
}

void MoveToTabButton::moveToTab() {
    emit window->createNewAutomata(f);
}
