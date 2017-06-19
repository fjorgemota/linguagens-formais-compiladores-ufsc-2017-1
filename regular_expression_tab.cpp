#include "regular_expression_tab.h"

#include <QGridLayout>

RegularExpressionTab::RegularExpressionTab(QWidget *parent) : AutomataTab(parent)
{
    QLayout *layout = this->layout();
    if (!layout) {
        layout = new QGridLayout(this);
    }
    this->editor = new RegularExpressionInput(this);
    layout->addWidget(this->editor);
}

void RegularExpressionTab::fromRegularExpression(RegularExpression &re) {
    this->editor->fromRegularExpression(re);
}

RegularExpression RegularExpressionTab::toRegularExpression() {
    return this->editor->toRegularExpression();
}

FiniteAutomata RegularExpressionTab::toAutomata() {
    return toRegularExpression().getAutomata();
}

bool RegularExpressionTab::isValid() {
    return editor->isValid();
}

QString RegularExpressionTab::getRegularExpression() {
    return editor->toPlainText();
}
