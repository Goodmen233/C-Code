#include <iostream>

using namespace std;

typedef char ElemType;
typedef struct Node
{
    ElemType* node;
    int size;
    const int MAXSIZE=100;// 最大节点数为100，最大层7层
}BiTree;

// 创建一个指定根节点的二叉树
void createTree(BiTree &bt, ElemType e)
{
    bt.node = new ElemType[bt.MAXSIZE];
    bt.node[0] = e;
    bt.size++;
    for(int i = 1; i < bt.MAXSIZE; i++){
        bt.node[i] = '0';
    }
}

// 插入一个元素,num表示第几个结点，drec表示左还是右
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

// 返回二叉树叶子节点总数
int countLeaf(BiTree bt)
{
    int count = 0;
    for(int i = 0, j = 0; j < bt.size && i < bt.MAXSIZE; i++){
        if(bt.node[i] != '0'){
            int n = i + 1;// n表示第几个节点
            if(2*n>bt.MAXSIZE && 2*n+1>bt.MAXSIZE) count++;
            else if(bt.node[2*n - 1] == '0' && bt.node[2 * n + 1 - 1] == '0') count++;
        }
    }
    return count;
}

// 遍历二叉树
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
