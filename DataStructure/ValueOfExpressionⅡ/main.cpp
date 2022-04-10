#include <iostream>

using namespace std;

//------------------------------ջ----------------------------------
typedef string Elemtype;
// ����һ����ջ
typedef struct Node
{
    Elemtype elem;
    struct Node* next;
}Node, *LinkedStack;

// ����һ����ջ
void createStack(LinkedStack &l)
{
    l = new Node;
    l->next = NULL;
}

// ջ�Ƿ�Ϊ��
bool isEmpty(LinkedStack l)
{
    if(l->next == NULL){
        return true;
    }
    return false;
}

// ѹջ
void push(LinkedStack &l, Elemtype e)
{
    LinkedStack temp = new Node;
    temp->elem = e;
    temp->next = l->next;;
    l->next = temp;
}

// ��ջ
void pop(Elemtype &e, LinkedStack &l)
{
    if(isEmpty(l)) return;
    e = l->next->elem;
    l->next = l->next->next;
}

// �鿴ջ��Ԫ��
Elemtype peek(LinkedStack l)
{
    return l->next->elem;
}

// �жϸ��ַ��Ƿ���С�����������
bool isNum(char ch)
{
    if(ch >= '0' && ch <= '9' || ch == '.') return true;
    return false;
}

// �жϸ��ַ��Ƿ�Ϊ�����
bool isOper(char ch)
{
    if(ch != '+' && ch != '-' && ch != '*' && ch != '/') return false;
    return true;
}

// �жϸ��ַ��Ƿ�Ϊ����
bool isBracket(char ch)
{
    if(ch != '(' && ch != ')') return false;
    return true;
}

// �ж����ȼ�
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
