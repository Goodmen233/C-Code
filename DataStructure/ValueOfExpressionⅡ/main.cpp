#include <iostream>

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

int main()
{

    return 0;
}
