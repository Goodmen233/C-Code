#include <iostream>
#include <string>
#include <sstream>

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

// ��ȡ����
string getNum(string str, int index)
{
    // ���������ַ���
    string numStr = "";
    for(int i = index; i < str.size(); i++){
        if(isNum(str[i])) numStr += str[i];
        else break;
    }
    return numStr;
}

//ǰ׺���ʽת��׺���ʽ
LinkedStack toPostfixStr(string prefix)
{
    LinkedStack result;
    LinkedStack oper;
    createStack(result);
    createStack(oper);
    // �����ַ�
    char ch;
    // ��һ�������������ʾ�費��Ҫ�ж��Ƿ�Ϊ����
    bool isJudge = true;
    for(int i = 0; i < prefix.size(); i++){
        // �ж��Ƿ�Ϊ����
        if(isJudge && isNum(prefix[i])){
            push(result, getNum(prefix, i));
            // �ı䲼����ʶ��
            isJudge = false;
        }else if(isOper(ch = prefix[i])){// ����������
            while(true){
                // ���ջΪ�գ���ջ��Ϊ���������ȼ��ߣ�ֱ��ѹջ
                if(isEmpty(oper)){
                    string tempS = "a";
                    tempS[0] = ch;
                    push(oper, tempS);
                    break;
                }
                // ����ջ��Ϊ�գ�ֱ��peek
                string temp = peek(oper);// ��ȡջ��Ԫ��
                if(temp[0] == '(' || highPriority(ch, temp[0])){
                    string tempS = "a";
                    tempS[0] = ch;
                    push(oper, tempS);
                    break;
                }else{// �������������ѹ��result��ظ�����
                    Elemtype e;
                    pop(e, oper);
                    push(result, e);
                }
            }
            // �����жϱ��
            isJudge = true;
        }else if(isBracket(ch = prefix[i])){// ���������
            // ����ǣ���ֱ��ѹջ
            if(ch == '('){
                string tempS = "a";
                tempS[0] = ch;
                push(oper, tempS);
            }
            else{// ����ǣ������ε����������֪������(,�������Ŷ���
                Elemtype e;
                string temp = peek(oper);
                while(temp[0] != '('){
                    pop(e, oper);
                    push(result, e);
                    temp = peek(oper);
                }
                pop(e, oper);// ȥ��������еģ�
            }
            // �����жϱ��
            isJudge = true;
        }
    }
    // ��ʣ�������ѹ��
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

// ���ַ���������ת��Ϊ����
double toNum(string str)
{
    bool isNeg = false;
    bool isPoint = false;
    double integer = 0;
    double minnum = 0;
    int j = 0;// ����С����׷�Ӵ���
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
    // ��nimnumת��ΪС��
    for(int i = 0; i < j; i++){
        minnum /= 10;
    }
    if(isNeg){
        return (0-(integer+minnum));
    }else{
        return integer+minnum;
    }
}

// ������ת��Ϊ�ַ���
string toStr(double num)
{
    stringstream ss;
    ss << num;
    return ss.str();
}

// ����׺���ʽ
double getVal(LinkedStack result)
{
    LinkedStack operS;
    createStack(operS);
    // �������ʽ
    while(!isEmpty(result)){
        Elemtype e;
        double res;
        pop(e, result);
        // ��������֣�ֱ��ѹջ
        if(isNum(e[0])){
            push(operS, e);
        }else{// ��Ȼ���������
            //��������Ԫ�ؽ��м���
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

// �����׺���ʽ
void msg(LinkedStack ls)
{
    // �������ʽ
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
