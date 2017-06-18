#ifndef FINITE_AUTOMATA_TABLE_H
#define FINITE_AUTOMATA_TABLE_H
#include "finite_automata.h"
#include <QTableWidget>

class FiniteAutomataTable : public QTableWidget
{
    Q_OBJECT

public:
    explicit FiniteAutomataTable(QWidget *parent = 0);

    ~FiniteAutomataTable();

    void fromAutomata(FiniteAutomata &f);

    FiniteAutomata toAutomata();
    bool isValid();
private:
    void warn(QTableWidgetItem *item, string message = "");
    set<string> getStates();
    set<char> getAlphabet();
    void validateTransitions();
    void validateState(QTableWidgetItem *item, set<string> states);
    void validateAlphabet();
    void validateStates();
    string fixStateName(string state);
    void fitTable();

public slots:
    void insertRowOrColumn(QTableWidgetItem *item);
};

#endif // FINITE_AUTOMATA_TABLE_H
