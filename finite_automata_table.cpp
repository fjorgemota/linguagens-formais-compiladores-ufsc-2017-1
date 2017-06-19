#include "finite_automata_table.h"
#include <QHeaderView>

const QString FiniteAutomataTable::WARNING_COLOR = "yellow";

FiniteAutomataTable::FiniteAutomataTable(QWidget *parent) : QTableWidget(parent)
{
    this->setRowCount(2);
    this->setColumnCount(2);
    this->setSortingEnabled(false);
    this->setMouseTracking(true);
    horizontalHeader()->hide();
    verticalHeader()->hide();
    setDelta();
    connect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(insertRowOrColumn(QTableWidgetItem*)));
}

void FiniteAutomataTable::setDelta() {
    QTableWidgetItem *delta = this->item(0, 0);
    if (!delta) {
        delta = new QTableWidgetItem();
        this->setItem(0, 0, delta);
    }
    delta->setText("𝛿");
    delta->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

bool FiniteAutomataTable::isValid() {

    int colCount = this->columnCount();
    int rowCount = this->rowCount();
    for (int i=0; i<rowCount; i++) {
        for (int j=0; j < colCount; j++) {
            QTableWidgetItem *cell = item(i, j);
            if (!cell) {
                continue;
            }
            if (cell->backgroundColor().name() == WARNING_COLOR) {
                return false;
            }
        }
    }

    return true;
}

void FiniteAutomataTable::insertRowOrColumn(QTableWidgetItem * item) {
    int colCount = this->columnCount();
    int rowCount = this->rowCount();
    if (item->text().size() > 0 && item->row() == rowCount-1) {
        emit this->insertRow(rowCount);
    }
    if (item->text().size() > 0 && item->column() == colCount-1) {
        emit this->insertColumn(colCount);
    }
    // Validate symbol!
    if (item->row() == 0) {
        validateAlphabet();
    }

    // Validate transitions!
    if (item->row() > 0 && item->column() > 0) {
        set<string> states = getStates();
        validateTransition(item, states);
        validateAlphabet();
        validateStates();
    } else if (item->column() == 0) {
        this->validateTransitions();
        this->validateStates();
    }
    if (item->text().isEmpty()) {
        this->fitTable();
    }
}

void FiniteAutomataTable::setReadOnly() {
    int rowCount = this->rowCount();
    int colCount = this->columnCount();
    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j++) {
            if (i == 0 && j == 0) {
                // Do not modify delta!!
                continue;
            }
            QTableWidgetItem *cell = item(i, j);
            if (!cell) {
                cell = new QTableWidgetItem();
                setItem(i, j, cell);
            }
            cell->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        }
    }
}

void FiniteAutomataTable::setReadWrite() {
    int rowCount = this->rowCount();
    int colCount = this->columnCount();
    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j++) {
            if (i == 0 && j == 0) {
                // Do not modify delta!!
                continue;
            }
            QTableWidgetItem *cell = item(i, j);
            if (!cell) {
                cell = new QTableWidgetItem();
                setItem(i, j, cell);
            }
            cell->setFlags(Qt::ItemIsEditable |Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled |Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
        }
    }
}


void FiniteAutomataTable::validateTransition(QTableWidgetItem *item, set<string> states) {
    QString itemText = item->text();
    if (states.count(itemText.toStdString())) {
        warn(item, "");
        return;
    }
    QStringList statesInput = itemText.split(",");
    if (statesInput.size() == 1 && !states.count("-")) {
        statesInput.removeAll("-");
    }
    bool validated = true;
    for (QString qstate: statesInput) {
        string state = qstate.toStdString();
        if (!states.count(state)) {
            validated = false;
            warn(item, "The state '"+state+"' is invalid");
            break;
        }
    }
    if (validated) {
        warn(item, "");
    }
}

void FiniteAutomataTable::fitTable() {
    this->blockSignals(true);
    int r=this->rowCount()-1;
    int c=this->columnCount()-1;
    // Check rows
    for(int i=1; i<r; i++) {
        bool emptyRow = true;
        for (int j=0; j<c; j++) {
            QTableWidgetItem *cell = item(i, j);
            if (!cell) {
                continue;
            }
            if (!cell->text().isEmpty()) {
                emptyRow = false;
            }
        }
        if (emptyRow) {
            emit this->removeRow(i);
            i--;
            r--;
        }
    }
    // Check columns
    for(int i=1; i<c; i++) {
        bool emptyColumn = true;
        for (int j=0; j<r; j++) {
            QTableWidgetItem *cell = item(j, i);
            if (!cell) {
                continue;
            }
            if (!cell->text().isEmpty()) {
                emptyColumn = false;
            }
        }
        if (emptyColumn) {
            emit this->removeColumn(i);
            i--;
            c--;
        }
    }
    this->blockSignals(false);
}

void FiniteAutomataTable::validateTransitions() {
    int r = this->rowCount();
    int c = this->columnCount();
    set<string> states = this->getStates();
    for (int i = 1; i < r; i++) {
        for (int j = 1; j < c; j++) {
           QTableWidgetItem *item = this->item(i, j);
           if (!item) {
               continue;
           }
           validateTransition(item, states);
        }
    }
}

void FiniteAutomataTable::warn(QTableWidgetItem *cell, string message) {
    this->blockSignals(true);
    if (message.empty()) {
        cell->setBackgroundColor(QColor("white"));
    } else {
        cell->setBackgroundColor(QColor(WARNING_COLOR));
    }
    cell->setStatusTip(message.c_str());
    this->blockSignals(false);
}

set<string> FiniteAutomataTable::getStates() {
    set<string> states;

    int l = this->rowCount();
    for (int c = 1; c < l; c++) {
        QTableWidgetItem *item = this->item(c, 0);
        if (!item) {
            continue;
        }
        states.insert(fixStateName(item->text().toStdString()));
    }
    return states;
}

void FiniteAutomataTable::validateAlphabet() {
    map<string, set<QTableWidgetItem*>> result;
    int c = 1;
    int l = this->columnCount();
    for (; c < l; c++) {
        QTableWidgetItem *item = this->item(0, c);
        if (!item) {
            item = new QTableWidgetItem();
            this->setItem(0, c, item);
        }
        if (c+1 == l) {
            continue;
        }
        string symbol = item->text().toStdString();

        if (!symbol.empty() && symbol[0] != '&' && ((symbol[0] < '0' || symbol[0] > '9') && (symbol[0] < 'a' || symbol[0] > 'z'))) {
            warn(item, "The symbol '"+symbol+"' is not valid");
            continue;
        }
        if (symbol.size() != 1) {
            warn(item, "The symbols of the alphabet should have size 1");
            continue;
        }
        warn(item, "");
        string s = symbol.substr(0, 1);
        result[s].insert(item);
    }
    for (auto item: result){
        if (item.second.size() < 2) {
            continue;
        }
        for (QTableWidgetItem *cell: item.second) {
            warn(cell, "Symbol duplicated: '"+item.first+"'");
        }
    }
}


void FiniteAutomataTable::validateStates() {
    map<string, set<QTableWidgetItem*>> result;
    int c = 1;
    int l = this->rowCount();
    bool hasInitialState = false;
    for (; c < l; c++) {
        QTableWidgetItem *item = this->item(c, 0);
        if (!item) {
            item = new QTableWidgetItem();
            this->setItem(c, 0, item);
        }
        warn(item, "");
        if (c+1 == l) {
            continue;
        }
        string state = item->text().toStdString();
        if((state.length() > 1 && state[0] == '-' && state[1] == '>') ||
                (state.length() > 2 && state[1] == '-' && state[2] == '>')) {
            if (hasInitialState) {
                warn(item, "A finite automata can have only one initial state");
            }
            hasInitialState = true;
        }
        state = fixStateName(state);
        if (state.empty()) {
            warn(item, "State cannot be empty");
            continue;
        }
        result[state].insert(item);
    }
    for (auto item: result){
        if (item.second.size() < 2 && hasInitialState) {
            continue;
        }
        for (QTableWidgetItem *cell: item.second) {
            if (hasInitialState) {
                warn(cell, "State duplicated: '"+item.first+"'");
            } else {
                warn(cell, "At least one should have initial state (starting with '->' without quotes)");
            }
        }
    }
}

set<char> FiniteAutomataTable::getAlphabet() {
    set<char> result;
    int c = 1;
    int l = this->columnCount();
    for (; c < l; c++) {
        QTableWidgetItem *item = this->item(0, c);
        if (!item) {
            continue;
        }
        string symbol = item->text().toStdString();
        if (symbol.empty()) {
            continue;
        }
        result.insert(symbol.at(0));
    }
    return result;
}

void FiniteAutomataTable::fromAutomata(FiniteAutomata &f) {
    set<string> states = f.getStates();
    emit this->clearContents();
    this->setDelta();
    int c = 1;
    bool hasEpsilon = false;
    for (string state: states) {
        QTableWidgetItem *cell = this->item(c, 0);
        if (cell) {
            warn(cell, "");
        } else {
            cell = new QTableWidgetItem();
            this->setItem(c, 0, cell);
        }
        QString text;
        if (f.isFinalState(state)) {
            text.append("*");
        }
        if (f.isInitialState(state)) {
            text.append("->");
        }
        text.append(state.c_str());
        cell->setText(text);
        if (!f.getTransitions(state, FiniteAutomata::EPSILON).empty()) {
            hasEpsilon = true;
        }
        c++;
    }
    c = 1;
    set<char> alphabet = f.getAlphabet();
    if (!hasEpsilon) {
        alphabet.erase(FiniteAutomata::EPSILON);
    }

    for (char symbol: alphabet) {
        QTableWidgetItem *cell = this->item(0, c);
        if (cell) {
            warn(cell, "");
        } else {
            cell = new QTableWidgetItem();
            this->setItem(0, c, cell);
        }
        QString text;
        text.append(symbol);
        cell->setText(text);
        c++;
    }
    int i = 1;
    for (string state: states) {
        int j = 1;
        for (char symbol: alphabet) {
            set<string> to = f.getTransitions(state, symbol);
            QTableWidgetItem *cell = this->item(i, j);
            if (cell) {
                warn(cell, "");
            } else {
                cell = new QTableWidgetItem();
                this->setItem(i, j, cell);
            }
            QString text;
            if (to.empty()) {
                text.append("-");
            } else {
                text.append(FiniteAutomata::formatStates(to, false).c_str());
            }
            cell->setText(text);
            j++;
        }
        i++;
    }
}

string FiniteAutomataTable::fixStateName(string state) {
    if ((state.length() > 1 && state[0] == '-' && state[1] == '>') ||
        (state.length() > 2 && state[1] == '-' && state[2] == '>')) {
        if (state[1] == '-' && state[2] == '>') {
            state.erase(1,2);
        } else {
            state.erase(0, 2);
        }
    }
    if (state.length() > 1 && state[0] == '*') {
        state.erase(0, 1);
    }
    return state;
}

FiniteAutomata FiniteAutomataTable::toAutomata() {
    if (!isValid()) {
        throw new FiniteAutomataException("Invalid automata");
    }
    FiniteAutomata f;
    int r = this->rowCount()-1;
    for (int c = 1; c < r; c++) {
        QTableWidgetItem *item = this->item(c, 0);
        if (!item) {
            continue;
        }
        QString text = item->text();
        if (!text.size()) {
            continue;
        }
        f.addState(text.toStdString());
    }
    int col = this->columnCount()-1;
    for (int c = 1; c < col; c++) {
        QTableWidgetItem *item = this->item(0, c);
        if (!item) {
            continue;
        }
        string symbol = item->text().toStdString();
        if (symbol.empty()) {
            continue;
        }
        f.addSymbol(symbol.at(0));
    }

    for (int i = 1; i < r; i++) {
        QTableWidgetItem *stateCell = this->item(i, 0);
        if (!stateCell) {
            continue;
        }

        string fromState = fixStateName(stateCell->text().toStdString());
        if (fromState.empty()) {
            continue;
        }

        for (int j = 1; j < col; j++) {
           QTableWidgetItem *symbolCell = this->item(0, j);
           if (!symbolCell) {
               continue;
           }

           string symbolStr = symbolCell->text().toStdString();
           if (symbolStr.empty()) {
               continue;
           }
           char symbol = symbolStr.at(0);

           QTableWidgetItem *item = this->item(i, j);
           if (!item) {
               continue;
           }
           QStringList statesInput = item->text().split(",");
           if (statesInput.size() == 1 && !f.hasState("-")) {
               statesInput.removeAll("-");
           }
           for (QString qstate: statesInput) {
               string state = qstate.toStdString();
               f.addTransition(fromState, symbol, state);
           }
        }
    }

    return f;
}

FiniteAutomataTable::~FiniteAutomataTable() {

}
