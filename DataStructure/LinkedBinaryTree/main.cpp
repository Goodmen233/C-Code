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

// ǰ��ݹ����
void preOrder(BiTree bt)
{
    if(bt->elem != '#'){
        // ������ڵ�
        cout<<bt->elem;
        // ���������������
        preOrder(bt->left);
        // ���������������
        preOrder(bt->right);
    }
}

// ǰ��ǵݹ����
void preOrderPlus(BiTree bt)
{
    stack<BiTree> s;// �����洢��Щֻ������������û�б����������Ľڵ�
    BiTree cur = bt;
    while(cur->elem != '#' || ! s.empty()){// ջΪ�ղ���ָ��ָ��Ԫ��Ϊ#ʱ�������
        // �����ǰ�ڵ㲻Ϊ#����ǰ�ڵ����ջ���������ָ��ָ����ڵ�
        if(cur->elem != '#'){
            s.push(cur);
            cout<<cur->elem;
            cur = cur->left;
        // ����
        }else{
             cur = s.top()->right;
             s.pop();// ������ڿ�ʼ�ұ��������Ե���ջ��Ԫ��
        }
    }

}

// ����ݹ����
void inOrder(BiTree bt)
{
    if(bt->elem != '#'){
        inOrder(bt->left);
        cout<<bt->elem;
        inOrder(bt->right);
    }
}

// ����ǵݹ����
void inOrderPlus(BiTree bt)
{
    stack<BiTree> s;// ����ֻ������������û��������ڵ���������Ľڵ�
    BiTree cur = bt;// ����ָ��
    s.push(cur);
    cur = cur->left;
    while(cur->elem != '#' || ! s.empty()){
        // �����ǰ�ڵ����ڵ㲻��#
        if(cur->elem != '#'){
            s.push(cur);
            cur = cur->left;
        }
        // ���������ǰָ��ָ���Ԫ�أ�����Լ�����ָ���ҽڵ�
        else{
            cur = s.top();
            cout<<cur->elem;// �����Ը��ڵ�
            cur = cur->right;
            s.pop();
        }
    }

}

// ����ݹ����
void postOrder(BiTree bt)
{
    if(bt->elem != '#'){
        postOrder(bt->left);
        postOrder(bt->right);
        cout<<bt->elem;
    }
}

// ����ǵݹ������˫ջ����-+a##*b##-c##d##/e##f##
void postOrderPlus(BiTree bt)
{
    stack<BiTree> s1;// ���滹û�з����������Ľڵ�
    stack<BiTree> s2;// �������˳���ջ
    BiTree cur = bt;// ����ָ��
    while(cur->elem != '#' || ! s1.empty()){
        // ��ָ���Ԫ�ز�Ϊ��
        if(cur->elem != '#'){
            s1.push(cur);
            s2.push(cur);// s2�ؼ��㣡����
            cur = cur->right;// ָ��������(ջ����ȳ�)
        }
        else{
            cur = s1.top()->left;// ָ��������
            s1.pop();// �����൱�ڷ�������������ջ����
        }
    }
    while(! s2.empty()){
        cout<<s2.top()->elem;
        s2.pop();
    }
}

//����ǵݹ����(������Ƿ�)
void flag_postOrderPlus(BiTree T){
//    stack<BiTree> s;// ����ֻ������������û��������ڵ���������Ľڵ�
//    BiTree cur = bt;// ����ָ��
//    BiTree flag = NULL;// ���ָ��
//    s.push(cur);
//    cur = cur->left;
//    while(cur->elem != '#' || ! s.empty()){
//        // �����ǰ�ڵ����ڵ㲻��#
//        if(cur->elem != '#'){
//            s.push(cur);
//            cur = cur->left;
//        }
//        // �����ǵ�ǰ�ڵ㣬ָ��������
//        else{
//
//        }
//    }
}

// �������
void levelOrder(BiTree bt)
{
    queue<BiTree> level;
    BiTree temp = bt;// ����ָ��
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

    cout<<"ǰ��ݹ������";
    preOrder(bt);
    cout<<endl;

    cout<<"ǰ��ǵݹ������";
    preOrderPlus(bt);
    cout<<endl;

    cout<<"����ݹ������";
    inOrder(bt);
    cout<<endl;

    cout<<"����ǵݹ������";
    inOrderPlus(bt);
    cout<<endl;

    cout<<"����ݹ������";
    postOrder(bt);
    cout<<endl;

    cout<<"����ǵݹ������˫ջ������";
    postOrderPlus(bt);
    cout<<endl;

    cout<<"����ǵݹ������������Ƿ�����";
    flag_postOrderPlus(bt);
    cout<<endl;

    cout<<"���������";
    levelOrder(bt);
    cout<<endl;

    return 0;
}
