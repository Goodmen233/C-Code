#include <iostream>

using namespace std;

typedef char ElemType;
typedef struct Node
{
    ElemType* node;
    int size;
    const int MAXSIZE=100;// ���ڵ���Ϊ100������7��
}BiTree;

// ����һ��ָ�����ڵ�Ķ�����
void createTree(BiTree &bt, ElemType e)
{
    bt.node = new ElemType[bt.MAXSIZE];
    bt.node[0] = e;
    bt.size++;
    for(int i = 1; i < bt.MAXSIZE; i++){
        bt.node[i] = '0';
    }
}

// ����һ��Ԫ��,num��ʾ�ڼ�����㣬drec��ʾ������
bool insertE(BiTree &bt, int num, char drec, ElemType e)
{
    int n = num - 1;
    if(bt.node[n] == '0' || num > bt.MAXSIZE) return false;
    if(drec == 'l'){
        if( 2 * num < bt.MAXSIZE){
            bt.node[2*num-1] = e;
            bt.size++;
            return true;
        }
        return false;
    }else if(drec == 'r'){
        if( 2 * num + 1 < bt.MAXSIZE){
            bt.node[2*num+1-1] = e;
            bt.size++;
            return true;
        }
        return false;
    }
    return false;
}

// ���ض�����Ҷ�ӽڵ�����
int countLeaf(BiTree bt)
{
    int count = 0;
    for(int i = 0, j = 0; j < bt.size && i < bt.MAXSIZE; i++){
        if(bt.node[i] != '0'){
            int n = i + 1;// n��ʾ�ڼ����ڵ�
            if(2*n>bt.MAXSIZE && 2*n+1>bt.MAXSIZE) count++;
            else if(bt.node[2*n - 1] == '0' && bt.node[2 * n + 1 - 1] == '0') count++;
        }
    }
    return count;
}

// ����������
void preOrder(BiTree bt)
{
    for(int i = 0, j = 0; j < bt.size && i < bt.MAXSIZE; i++){
        if(bt.node[i] != '0'){
            cout<<bt.node[i]<<" ";
            j++;
        }
    }
}

int main()
{
    BiTree bt;
    createTree(bt, 'A');
    insertE(bt, 1, 'l', 'B');
    insertE(bt, 1, 'r', 'C');
    insertE(bt, 2, 'r', 'D');
    insertE(bt, 2, 'l', 'G');
    insertE(bt, 3, 'l', 'E');
    preOrder(bt);
    cout<<countLeaf(bt);
    return 0;
}
