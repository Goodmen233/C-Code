#include <iostream>
#include <string>
#include <sstream>

using namespace std;

//------------------------------栈----------------------------------
typedef string Elemtype;
// 定义一个链栈
typedef struct Node
{
    Elemtype elem;
    struct Node* next;
}Node, *LinkedStack;

// 创建一个空栈
void createStack(LinkedStack &l)
{
    l = new Node;
    l->next = NULL;
}

// 栈是否为空
bool isEmpty(LinkedStack l)
{
    if(l->next == NULL){
        return true;
    }
    return false;
}

// 压栈
void push(LinkedStack &l, Elemtype e)
{
    LinkedStack temp = new Node;
    temp->elem = e;
    temp->next = l->next;;
    l->next = temp;
}

// 出栈
void pop(Elemtype &e, LinkedStack &l)
{
    if(isEmpty(l)) return;
    e = l->next->elem;
    l->next = l->next->next;
}

// 查看栈顶元素
Elemtype peek(LinkedStack l)
{
    return l->next->elem;
}

// 判断该字符是否是小数点或者数字
bool isNum(char ch)
{
    if(ch >= '0' && ch <= '9' || ch == '.') return true;
    return false;
}

// 判断该字符是否为运算符
bool isOper(char ch)
{
    if(ch != '+' && ch != '-' && ch != '*' && ch != '/') return false;
    return true;
}

// 判断该字符是否为括号
bool isBracket(char ch)
{
    if(ch != '(' && ch != ')') return false;
    return true;
}

// 判断优先级
bool highPriority(char ch1, char ch2)
{
    if(ch2 == '+' || ch2 == '-'){
        if(ch1 == '*' || ch1 == '/') return true;
    }
    return false;
}

// 获取数字
string getNum(string str, int index)
{
    // 缓存数字字符串
    string numStr = "";
    for(int i = index; i < str.size(); i++){
        if(isNum(str[i])) numStr += str[i];
        else break;
    }
    return numStr;
}

//前缀表达式转后缀表达式
LinkedStack toPostfixStr(string prefix)
{
    LinkedStack result;
    LinkedStack oper;
    createStack(result);
    createStack(oper);
    // 缓存字符
    char ch;
    // 多一个布尔标记来表示需不需要判断是否为数字
    bool isJudge = true;
    for(int i = 0; i < prefix.size(); i++){
        // 判断是否为数字
        if(isJudge && isNum(prefix[i])){
            push(result, getNum(prefix, i));
            // 改变布尔标识符
            isJudge = false;
        }else if(isOper(ch = prefix[i])){// 如果是运算符
            while(true){
                // 如果栈为空，或栈顶为（，或优先级高，直接压栈
                if(isEmpty(oper)){
                    string tempS = "a";
                    tempS[0] = ch;
                    push(oper, tempS);
                    break;
                }
                // 到这栈不为空，直接peek
                string temp = peek(oper);// 获取栈顶元素
                if(temp[0] == '(' || highPriority(ch, temp[0])){
                    string tempS = "a";
                    tempS[0] = ch;
                    push(oper, tempS);
                    break;
                }else{// 否则将运算符弹出压到result里，重复以上
                    Elemtype e;
                    pop(e, oper);
                    push(result, e);
                }
            }
            // 更改判断标记
            isJudge = true;
        }else if(isBracket(ch = prefix[i])){// 如果是括号
            // 如果是（，直接压栈
            if(ch == '('){
                string tempS = "a";
                tempS[0] = ch;
                push(oper, tempS);
            }
            else{// 如果是），依次弹出运算符，知道遇到(,并将括号丢弃
                Elemtype e;
                string temp = peek(oper);
                while(temp[0] != '('){
                    pop(e, oper);
                    push(result, e);
                    temp = peek(oper);
                }
                pop(e, oper);// 去掉运算符中的（
            }
            // 更改判断标记
            isJudge = true;
        }
    }
    // 将剩余运算符压入
    while(!isEmpty(oper)){
        Elemtype e;
        pop(e, oper);
        push(result, e);
    }
    LinkedStack postfix;
    createStack(postfix);
    while(!isEmpty(result)){
        Elemtype e;
        pop(e, result);
        push(postfix, e);
    }
    return postfix;
}

// 将字符串的数字转换为数字
double toNum(string str)
{
    bool isNeg = false;
    bool isPoint = false;
    double integer = 0;
    double minnum = 0;
    int j = 0;// 计算小数点追加次数
    for(int i = 0; i < str.size(); i++){
        if(str[i] == '-'){
            isNeg = true;
            continue;
        }
        if(str[i] == '.'){
            isPoint = true;
            continue;
        }
        if(!isPoint){
            double temp = str[i] - '0';
            integer *= 10;
            integer += temp;
        }else{
            double temp = str[i] - '0';
            minnum *= 10;
            minnum += temp;
            j++;
        }
    }
    // 将nimnum转换为小数
    for(int i = 0; i < j; i++){
        minnum /= 10;
    }
    if(isNeg){
        return (0-(integer+minnum));
    }else{
        return integer+minnum;
    }
}

// 将数字转换为字符串
string toStr(double num)
{
    stringstream ss;
    ss << num;
    return ss.str();
}

// 求解后缀表达式
double getVal(LinkedStack result)
{
    LinkedStack operS;
    createStack(operS);
    // 遍历表达式
    while(!isEmpty(result)){
        Elemtype e;
        double res;
        pop(e, result);
        // 如果是数字，直接压栈
        if(isNum(e[0])){
            push(operS, e);
        }else{// 不然就是运算符
            //弹出两个元素进行计算
            Elemtype num1;
            Elemtype num2;
            pop(num1, operS);
            pop(num2, operS);
            if(e[0] == '+'){
                res = toNum(num1) + toNum(num2);
                push(operS, toStr(res));
            }else if(e[0] == '-'){
                res = toNum(num2) - toNum(num1);
                push(operS, toStr(res));
            }else if(e[0] == '*'){
                res = toNum(num1) * toNum(num2);
                push(operS, toStr(res));
            }else{
                res = toNum(num2) / toNum(num1);
                push(operS, toStr(res));
            }
        }
    }
    Elemtype elem;
    pop(elem, operS);
    return toNum(elem);
}

// 输出后缀表达式
void msg(LinkedStack ls)
{
    // 遍历表达式
    while(!isEmpty(ls)){
        Elemtype e;
        pop(e, ls);
        cout<<e<<" ";
    }
}

int main()
{
    string prefix = "(5.3+4.7)*5+(5.9-(4+4))";
    LinkedStack ls;
    ls = toPostfixStr(prefix);
    //msg(toPostfixStr(prefix));
    cout<<prefix<<"="<<getVal(toPostfixStr(prefix));
    return 0;
}
