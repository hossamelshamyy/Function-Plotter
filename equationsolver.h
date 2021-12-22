#ifndef EQUATIONSOLVER_H
#define EQUATIONSOLVER_H
#include "QStack"
#include <string.h>
#include <math.h>
#include "QDebug"
#include "QObject"
typedef union
{
    double dData;
    char  cData;
} Item;

class EquationSolver : public QObject
{
    Q_OBJECT
public:
    EquationSolver();
    QStack<Item> stack;
    QVector<double> calcFunc(QString,double,double,double);
signals:
    void errorMsg(QString);
private:
    bool validExpression;
    bool is_operator(char c);
    int priority(char c);
    QString infixToPostfix(QString);
    double evaluate (char x,double y,double z);
    double evaluatePostfix(QString);


};

#endif // EQUATIONSOLVER_H
