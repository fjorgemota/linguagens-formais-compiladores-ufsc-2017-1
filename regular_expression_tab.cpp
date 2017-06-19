#include "regular_expression_tab.h"

#include <QGridLayout>

RegularExpressionTab::RegularExpressionTab(QWidget *parent) : QWidget(parent)
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
