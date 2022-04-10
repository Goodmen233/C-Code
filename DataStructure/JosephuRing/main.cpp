#include <iostream>

using namespace std;

// ����������
typedef struct LNode
{
    int data;
    int pssd;
    struct LNode *next;
}LNode, *LinkedList;

// ����һ����������Ļ�������
void createL(LinkedList &l, int num)
{
    // ����ָ��
    LinkedList cur = l;
    for(int i = 1; i <= num; i++){
        // �����һ���ڵ�
        if(i == 1){
            l->data = i;
            l->next = l;
        }else{
            LinkedList tempL = new LNode;
            // �½ڵ��з�����
            tempL->data = i;
            tempL->next = l;
            // ����ָ��ָ���¸�Ԫ��
            cur->next = tempL;
            cur = cur->next;
        }
    }
}

// ��������
void msg(LinkedList &l)
{
    // ����ָ��
    LinkedList cur = l;
    while(true){
        cout<<cur->data<<endl;
        cur = cur->next;
        if(cur == l) break;
    }
}

// Josephu����
void Josephu(LinkedList l, int startNo, int countNum)
{
    // ʹ�ø���ָ��
    LinkedList nowCur;
    LinkedList preCur = l;
    while(true){
        preCur = preCur->next;
        if(preCur->next == l) break;
    }
    // ʹָ���ƶ�����ʼ������ǰһ�����
    for(int i = 0; i < startNo - 1; i++){
        preCur = preCur->next;
    }
    nowCur = preCur->next;
    while(true){
        // ��ʼ�������ƶ�countNum-1��
        for(int i = 0; i < countNum - 1; i++){
            nowCur = nowCur->next;
            preCur = preCur->next;
        }
        cout<<"ȡ����"<<nowCur->data<<endl;
        // ʹȡ���Ľڵ��ǰһ���ڵ�ָ��ȡ���ڵ����һ���ڵ�
        preCur->next = nowCur->next;
        nowCur = preCur->next;
        // ���ֻʣ��һ��
        if(preCur == nowCur){
            cout<<"���ȡ����"<<nowCur->data<<endl;
            break;
        }
    }

}

// ѧУ��Լɪ��
void Josephu(LinkedList l, int countNum)
{
    // ʹ�ø���ָ��
    LinkedList nowCur;
    LinkedList preCur = l;
    while(true){
        preCur = preCur->next;
        if(preCur->next == l) break;
    }
    nowCur = preCur->next;
    while(true){
        // ��ʼ�������ƶ�countNum-1��
        for(int i = 0; i < countNum - 1; i++){
            nowCur = nowCur->next;
            preCur = preCur->next;
        }
        cout<<"ȡ����"<<nowCur->data<<endl;
        countNum = nowCur->pssd;
        // ʹȡ���Ľڵ��ǰһ���ڵ�ָ��ȡ���ڵ����һ���ڵ�
        preCur->next = nowCur->next;
        nowCur = preCur->next;
        // ���ֻʣ��һ��
        if(preCur == nowCur){
            cout<<"���ȡ����"<<nowCur->data<<endl;
            break;
        }
    }

}

// ����ÿ���˵�����
void inputPssd(LinkedList &l)
{
    // ����ָ��
    LinkedList cur = l;
    while(true){
        int i= 0 ;
        cout<<"Please input the password:";
        cin>>i;
        cur->pssd = i;
        cur = cur->next;
        if(cur == l) break;
    }

}

int main()
{
    LinkedList l = new LNode;
    int n;
    cout<<"����������";
    cin>>n;
    createL(l,n);
    inputPssd(l);
    // ��һ��
    msg(l);
    // �ڶ��� 3 1 7 2 4 8 4
    //Josephu(l,20);
    // 6 1 4 7 2 3 5
    // 1 2 4 3 5
    // ������
    cout<<"������һ����ʼ��countNumֵ��";
    int countNum = 0;
    cin>>countNum;
    Josephu(l,countNum);
    return 0;
}
