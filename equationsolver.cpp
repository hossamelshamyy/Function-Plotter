#include "equationsolver.h"

EquationSolver::EquationSolver()
{

}

/**
 * @brief EquationSolver::is_operator function checks whether the given character is operator or not
 * @param c
 * @return true if operator, no otherwise
 */
bool EquationSolver::is_operator(char c)
{
    if(c == '^' || c == '*' || c == '/' || c == '+' || c =='-')
    {
        return true;
    }
    else
    {
        return false;
    }
}
/**
 * @brief EquationSolver::priority function checks the priorty of a given operator
 * @param c
 * @return number from 3 to 0 depending on the priorty of the given operator
 */
int EquationSolver::priority(char c)
{
    if(c == '^')
    {
        return 3;
    }
    else if(c == '*' || c == '/')
    {
        return 2;
    }

    else if(c == '+' || c == '-')
    {
        return 1;
    }
    else
    {
        emit errorMsg(QString("Your function is completely wrong ! please check it again"));
        return 0;
    }
}

/**
 * @brief EquationSolver::infixToPostfix function takes any function to convert it to a postfix form to evaluate it later
 * Note that the given infix expression need to have a space between every integer or operator, ex : 3 * -x - 5 + -3 * ( x ^ 2 ), so we can convert it easily
 * @param infix
 * @return QString contains the postfix expression
 */
QString EquationSolver::infixToPostfix(QString infix){
    QString postfix;
    Item x;
    Item item;
    item.cData = '(';
    stack.push(item);
    infix.append(" )");
    QStringList list = infix.split(' ');
    QString token;
    foreach(token,list)
    {
        if(token =="") continue;
        item.cData = token[0].toLatin1();

        if(token.length() == 1 && is_operator(token[0].toLatin1())){

            x=stack.pop();
            while(is_operator(x.cData)&& priority(x.cData)>= priority(token[0].toLatin1()))
            {
                postfix.append(x.cData);
                postfix.append('\n');
                x = stack.pop();
            }
            stack.push(x);
            stack.push(item);
        } else if(item.cData == '(')
        {
            stack.push(item);
        } else if((token[0].isDigit() || token[0] == 'X') || token.length() > 1){
            postfix.append(token);
            postfix.append('\n');
        } else if(token[0] == ')'){
            x = stack.pop();
            while(x.cData != '('){
                postfix.append(x.cData);
                postfix.append('\n');
                x = stack.pop();
            }
        }else{
            emit errorMsg(QString("Your function is completely wrong ! please check it again"));
            postfix = "";
            return postfix;
        }
    }

    return postfix.replace('\n',' ');;
}

/**
 * @brief EquationSolver::evaluate function perform one operation scince we are dealing with strings so we need a function that take a character and two operands to perfrom mathmeatics operation
 * @param x operator character
 * @param y first operand
 * @param z second operand
 * @return result of the operation
 */
double EquationSolver::evaluate(char x,double y,double z)
{
    if(x=='+')
        return(y+z);
    if(x=='-')
        return(y-z);
    if(x=='*')
        return(y*z);
    if(x=='/')
        return(y/z);
    if(x=='^')
        return pow(y,z);
    emit errorMsg(QString("Your function is completely wrong ! please check it again"));
    return 0;

}

/**
 * @brief EquationSolver::evaluatePostfix function is evaluating given postfix expression to get the final result
 * @param postfix
 * @return
 */
double EquationSolver::evaluatePostfix(QString postfix)
{
    double x,y;
    Item z;
    QStringList list = postfix.split(' ');
    QString token;
    foreach(token,list){
        if(token =="") continue;
        if((token[0].isDigit() || token[0] == 'X')||(token[0]=='-'&&token.length()>1))
        {
            z.dData=token.toDouble();
            stack.push(z);
        }
        else
        {
            if(stack.length() <2){
                if(is_operator(token[0].toLatin1()))
                    emit errorMsg(QString("You forgot the second operand of ").append(token[0]));
                else
                    emit errorMsg(QString("Your function is completely wrong ! please check it again"));
                validExpression = false;
            }else{
                x=stack.pop().dData;
                y=stack.pop().dData;
                z.dData=evaluate(token[0].toLatin1(),y,x);
                stack.push(z);
            }
        }
    }
    if(stack.length()>0)
        return stack.pop().dData;
    else{
        validExpression = false;
        emit errorMsg(QString("Your function is completely wrong ! please check it again"));
        return 0;
    }
}

QVector<double> EquationSolver::calcFunc(QString func,double from,double to,double step){
    QVector<double> y;
    /**
     * We will do some checks first before doing any operation, to avoid any trivial mistakes and make the program more flexible and give best UX :D
     **/

    if (func.isEmpty()) return y; // Check if the function is empty string
    func = func.toUpper(); // Convert X variable to uppercase incase the user entered it in a lowercase

    // Here we check if the function contains any alphabet except X or contains any special character except the allowed operators
    if(func.contains(QRegExp("[A-WY-Z]+"))|| func.contains(QRegExp("[!@#$%&_=`~'\";:<>?\\|\\{\\}]"))) {
        emit errorMsg(QString("Your function is completely wrong ! please check it again"));
        return y;
    }

    // Here we check if the case "num(expression)" happened then we will replace it with "num*(expression)"
    // Ex: "9(2*X+1)" -> "9*(2*X+1)"
    int expIndex = func.indexOf(QRegExp("\\d\\("));
    expIndex!= -1 ? func.insert(expIndex+1,'*'):NULL;

    // Here we check if the case "-(expression)" happened then we will replace it with "-1*(expression)"
    // Ex: "-(2*X+1)" -> "-1 *(2*X+1)"
    func.replace(QRegExp("[-]\\("),"-1 *(");

    func.append(' '); // We appenend this useless whitespace to avoid out of bound error when accessing characters in the followin for loop

    // The following for loop will seperate the numbers and operators as i wrote the infix to postfix function that needs to have space between every operand and operator so we can iterate over the expression correctly
    // Ex: "2.5*X + 3-8^2 /(5+-30)" -> "2.5 * X + 3 - 8 ^ 2 / ( 5 + -30 ) "
    for(int i = 0;i < func.length(); i++){
        if(func[i] =='.'){ // Dot operator must be followed by a number
            if(func[i+1].isDigit())
                continue;
            else{
                emit errorMsg(QString("The Dot \".\" operator has no meaning ! please check your function"));
                return y;
            }
        }
        else if(func[i].isDigit() ||(func[i] == '-' && (func[i+1].isDigit() || func[i+1] =='X'))){ // if it is a postive or negative number or -X
            if(func[i+1]=='-' && func[i+2].isDigit() ) func.insert(i+1,"+ "); // if this case happened "2-3" will convert it to "2 + -3", so we can evaluate the expression correctly later
            else continue;
        }
        if(func[i] == 'X'){
            if(func[i+1]=='-') func.insert(i+1,"+ "); // if this case happened "X-3" will convert it to "X + -3", so we can evaluate the expression correctly later
            if(i !=0)
                func[i-1].isDigit() ? func.insert(i,'*'):NULL; // if this case happened "3X" will convert it to "3 * X", so we can evaluate the expression correctly later
        }
        if(is_operator(func[i].toLatin1()) || func[i] == '(' || func[i] == ')'){ // if the current character is operator or '(' or ')' , then will check if there are whitespaces before and after the character, otherwise will add the needed whitespaces
            char x = func[i].toLatin1();
            if(i !=0)
                func[i-1]!= ' '?func.insert(i,' '):NULL;
            func[func.indexOf(x,i)+1]!= ' '?func.insert(func.indexOf(x,i)+1,' '):NULL;
        }
    }

    qDebug() << "Function = " << func;
    QString postfixExpr= infixToPostfix(func.toLocal8Bit().data());


    if(postfixExpr == NULL || postfixExpr ==""){
        emit errorMsg(QString("Your function is completely wrong ! please check it again"));
        return y;
    }

    qDebug() << "Postfix" << postfixExpr;
    validExpression = true; // This flag changes whenever error occurred

    // final stage is to replace the variable X in the postfix expression with each number in the range specified by the user and evaluate the result then add it to Y vector
    while(from<= to){
        // Check the flag on every iteration
        if(!validExpression){
            y.empty();
            return y;
        }
        QString postix = QString(postfixExpr).replace('X',QString::number(from));
        double value = evaluatePostfix(postix.toLocal8Bit().data());
        y.append(value);
        from += step;
    }
    return y;
}
