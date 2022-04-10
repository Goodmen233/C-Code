#include <iostream>
#include <stack>
#include <queue>
using namespace std;

typedef char ElemType;
typedef struct Node
{
    ElemType elem;
    struct Node* left;
    struct Node* right;
    int rType,lType;
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

// 前序递归遍历
void preOrder(BiTree bt)
{
    if(bt->elem != '#'){
        // 输出根节点
        cout<<bt->elem;
        // 接下来输出左子树
        preOrder(bt->left);
        // 接下来输出右子树
        preOrder(bt->right);
    }
}

// 前序非递归遍历
void preOrderPlus(BiTree bt)
{
    stack<BiTree> s;// 用来存储那些只遍历左子树，没有遍历右子树的节点
    BiTree cur = bt;
    while(cur->elem != '#' || ! s.empty()){// 栈为空并且指针指向元素为#时遍历完毕
        // 如果当前节点不为#，当前节点放入栈，并输出，指针指向左节点
        if(cur->elem != '#'){
            s.push(cur);
            cout<<cur->elem;
            cur = cur->left;
        // 否则，
        }else{
             cur = s.top()->right;
             s.pop();// 上面等于开始右遍历，所以弹出栈顶元素
        }
    }

}

// 中序递归遍历
void inOrder(BiTree bt)
{
    if(bt->elem != '#'){
        inOrder(bt->left);
        cout<<bt->elem;
        inOrder(bt->right);
    }
}

// 中序非递归遍历
void inOrderPlus(BiTree bt)
{
    stack<BiTree> s;// 缓存只遍历左子树，没有输出根节点和右子树的节点
    BiTree cur = bt;// 辅助指针
    s.push(cur);
    cur = cur->left;
    while(cur->elem != '#' || ! s.empty()){
        // 如果当前节点的左节点不是#
        if(cur->elem != '#'){
            s.push(cur);
            cur = cur->left;
        }
        // 否则输出当前指针指向的元素，输出自己，再指向右节点
        else{
            cur = s.top();
            cout<<cur->elem;// 输出相对根节点
            cur = cur->right;
            s.pop();
        }
    }

}

// 后序递归遍历
void postOrder(BiTree bt)
{
    if(bt->elem != '#'){
        postOrder(bt->left);
        postOrder(bt->right);
        cout<<bt->elem;
    }
}

// 后序非递归遍历，双栈法。-+a##*b##-c##d##/e##f##
void postOrderPlus(BiTree bt)
{
    stack<BiTree> s1;// 储存还没有访问左子树的节点
    stack<BiTree> s2;// 储存输出顺序的栈
    BiTree cur = bt;// 辅助指针
    while(cur->elem != '#' || ! s1.empty()){
        // 当指向的元素不为空
        if(cur->elem != '#'){
            s1.push(cur);
            s2.push(cur);// s2关键点！！！
            cur = cur->right;// 指向右子树(栈后进先出)
        }
        else{
            cur = s1.top()->left;// 指向左子树
            s1.pop();// 以上相当于访问了左子树，栈顶弹
        }
    }
    while(! s2.empty()){
        cout<<s2.top()->elem;
        s2.pop();
    }
}

//后序非递归遍历(辅助标记法)
void flag_postOrderPlus(BiTree T){
//    stack<BiTree> s;// 缓存只遍历左子树，没有输出根节点和右子树的节点
//    BiTree cur = bt;// 辅助指针
//    BiTree flag = NULL;// 标记指针
//    s.push(cur);
//    cur = cur->left;
//    while(cur->elem != '#' || ! s.empty()){
//        // 如果当前节点的左节点不是#
//        if(cur->elem != '#'){
//            s.push(cur);
//            cur = cur->left;
//        }
//        // 否则标记当前节点，指向右子树
//        else{
//
//        }
//    }
}

// 层序遍历
void levelOrder(BiTree bt)
{
    queue<BiTree> level;
    BiTree temp = bt;// 辅助指针
    level.push(temp);
    bool isStop = false;
    while(! isStop){
        bool isStop = true;
        int len = level.size();
        for(int i = 0; i < len; i++){
            temp = level.front();
            cout<<temp->elem<<" ";
            level.pop();
            if(temp->left->elem != '#'){
                level.push(temp->left);
                isStop = false;
            }
            if(temp->right->elem != '#'){
                level.push(temp->right);
                isStop = false;
            }
        }
        if(isStop) return;

    }
}

int main()
{
    BiTree bt;
    createBiTree(bt);

    cout<<"前序递归遍历：";
    preOrder(bt);
    cout<<endl;

    cout<<"前序非递归遍历：";
    preOrderPlus(bt);
    cout<<endl;

    cout<<"中序递归遍历：";
    inOrder(bt);
    cout<<endl;

    cout<<"中序非递归遍历：";
    inOrderPlus(bt);
    cout<<endl;

    cout<<"后序递归遍历：";
    postOrder(bt);
    cout<<endl;

    cout<<"后序非递归遍历（双栈法）：";
    postOrderPlus(bt);
    cout<<endl;

    cout<<"后序非递归遍历（辅助标记法）：";
    flag_postOrderPlus(bt);
    cout<<endl;

    cout<<"层序遍历：";
    levelOrder(bt);
    cout<<endl;

    return 0;
}
