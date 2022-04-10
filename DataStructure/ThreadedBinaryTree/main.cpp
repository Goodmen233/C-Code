#include <iostream>
#include <stack>
using namespace std;

typedef char ElemType;
typedef struct Node
{
    ElemType elem;
    struct Node* left;
    struct Node* right;
    int lType;// 0��ʾָ��������1��ʾָ��ǰ��
    int rType;// 0��ʾָ��������1��ʾָ�����
}*BiTree;

// ������
void createBiTree(BiTree &bt)
{
    bt = new Node;
    ElemType e;
    cout<<"����������Ԫ�أ�#��ʾΪ��";
    cin>>e;
    if(e == '#'){
        bt->elem = '#';
    }
    else{
        bt->elem = e;
        createBiTree(bt->left);
        createBiTree(bt->right);
    }
}

// ����������������
void threadBiTree(BiTree bt, BiTree &pre)
{
    BiTree cur = bt;// ����ָ��
    while(cur->elem != '#'){
        // 1.������������
        if(cur->left->elem != '#'){
            threadBiTree(cur->left, pre);
        }

        // 2.��������ǰ�ڵ�
        // ����ǰ���ڵ�
        if(cur->left->elem == '#'){
            cur->left = pre;
            cur->lType = 1;
        }
        // ��������ڵ�
        if(pre != NULL && pre->right->elem == '#'){
            pre->right = cur;
            pre->rType = 1;
        }

        pre = cur;

        // 3.������������
        if(cur->right->elem != '#'){
            threadBiTree(cur->right, pre);
        }
    }
}

// ��������������������
void inOrder(BiTree bt)
{
    BiTree cur = bt;// ����ָ��
    while(cur->elem != '#'){
        // ������ָ��ǰ����ʱ�򣬼���������
        while(cur->lType == 0){
            cur = cur->left;
        }
        cout<<cur->elem;
        // ����Ǻ�����ֱ�����
        while(cur->rType == 1){
            cur = cur->right;
            cout<<cur->elem;
        }
        // ֱ�����Ǻ�����
        cur = cur->right;
    }
}

int main()
{
    BiTree bt;
    BiTree pre = NULL;
    createBiTree(bt);
    threadBiTree(bt, pre);

    return 0;
}
