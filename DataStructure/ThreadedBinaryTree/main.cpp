#include <iostream>
#include <stack>
using namespace std;

typedef char ElemType;
typedef struct Node
{
    ElemType elem;
    struct Node* left;
    struct Node* right;
    int lType;// 0表示指向子树，1表示指向前驱
    int rType;// 0表示指向子树，1表示指向后驱
}*BiTree;

// 创建树
void createBiTree(BiTree &bt)
{
    bt = new Node;
    ElemType e;
    cout<<"请输入插入的元素，#表示为空";
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

// 中序线索化二叉树
void threadBiTree(BiTree bt, BiTree &pre)
{
    BiTree cur = bt;// 辅助指针
    while(cur->elem != '#'){
        // 1.线索化左子树
        if(cur->left->elem != '#'){
            threadBiTree(cur->left, pre);
        }

        // 2.线索化当前节点
        // 处理前驱节点
        if(cur->left->elem == '#'){
            cur->left = pre;
            cur->lType = 1;
        }
        // 处理后驱节点
        if(pre != NULL && pre->right->elem == '#'){
            pre->right = cur;
            pre->rType = 1;
        }

        pre = cur;

        // 3.线索化右子树
        if(cur->right->elem != '#'){
            threadBiTree(cur->right, pre);
        }
    }
}

// 遍历中序线索化二叉树
void inOrder(BiTree bt)
{
    BiTree cur = bt;// 辅助指针
    while(cur->elem != '#'){
        // 当左不是指向前驱的时候，继续往下走
        while(cur->lType == 0){
            cur = cur->left;
        }
        cout<<cur->elem;
        // 如果是后驱，直接输出
        while(cur->rType == 1){
            cur = cur->right;
            cout<<cur->elem;
        }
        // 直到不是后驱了
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
