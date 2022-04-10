#include <iostream>

using namespace std;

// 单向链表
typedef struct LNode
{
    int no;
    int num;
    struct LNode *next;
}LNode, *Polynomial;

// 创建一个头节点为空链表
void createP(Polynomial &p)
{
    p = new LNode;
    p->no = -1;
    p->next = NULL;
}

// 遍历链表
void msg(Polynomial &p)
{
    if(p == NULL) return;
    // 辅助指针
    Polynomial cur = p;
    while(cur->next != NULL){
        cout<<cur->next->no<<" "<<cur->next->num<<endl;
        cur = cur->next;
    }
}

// 添加序号及元素,有序
void addP(Polynomial &p, int no, int num, bool isOver)
{
    if(no < 0) return;
    Polynomial cur = p;
    // 新节点
    Polynomial temp = new LNode;
    temp->no = no;
    temp->num = num;
    temp->next = NULL;
    while(cur->next != NULL){
        if(no < cur->next->no){
            temp->next = cur->next;
            cur->next=temp;
            return;
        }
        // 如果相等，并且isOver为true，则覆盖，否则两者相加
        if(no == cur->next->no){
            if(isOver == true){
                cur->next->num = num;
                return;
            }else{
                cur->next->num += num;
                return;
            }

        }
        cur = cur->next;
    }
    // 运行到这说明no大于所有值,尾插
    cur->next = temp;
}

// 两个多项式相加
void add(Polynomial p1, Polynomial p2, Polynomial &p3)
{
    Polynomial cur1 = p1;
    Polynomial cur2 = p2;
    while(cur1->next != NULL && cur2->next != NULL){
        if(cur1->next->no > cur2->next->no){
            addP(p3, cur2->next->no, cur2->next->num, true);
            cur2 = cur2->next;
        }else if(cur1->next->no < cur2->next->no){
            addP(p3, cur1->next->no, cur1->next->num, true);
            cur1 = cur1->next;
        }else{
            addP(p3, cur2->next->no, cur2->next->num+cur1->next->num, true);
            cur2 = cur2->next;
            cur1 = cur1->next;
        }
    }
    while(cur1->next != NULL){
        addP(p3, cur1->next->no, cur1->next->num, true);
        cur1 = cur1->next;
    }
    while(cur2->next != NULL){
        addP(p3, cur2->next->no, cur2->next->num, true);
        cur2 = cur2->next;
    }
}

// 两个多项式相减，p1被减数，p2减数
void sub(Polynomial p1, Polynomial p2, Polynomial &p3)
{
    Polynomial cur1 = p1;
    Polynomial cur2 = p2;
    while(cur1->next != NULL && cur2->next != NULL){
        if(cur1->next->no > cur2->next->no){
            addP(p3, cur2->next->no, cur2->next->num, true);
            cur2 = cur2->next;
        }else if(cur1->next->no < cur2->next->no){
            addP(p3, cur1->next->no, cur1->next->num, true);
            cur1 = cur1->next;
        }else{
            if(cur2->next->num-cur1->next->num == 0){
                cur2 = cur2->next;
                cur1 = cur1->next;
                continue;
            }
            addP(p3, cur2->next->no, cur2->next->num-cur1->next->num, true);
            cur2 = cur2->next;
            cur1 = cur1->next;
        }
    }
    while(cur1->next != NULL){
        addP(p3, cur1->next->no, cur1->next->num, true);
        cur1 = cur1->next;
    }
    // p2减数，改变符号
    while(cur2->next != NULL){
        addP(p3, cur2->next->no, 0 - cur2->next->num, true);
        cur2 = cur2->next;
    }
}

// 两个多项式相乘
void multi(Polynomial p1, Polynomial p2, Polynomial &p3)
{
    Polynomial cur1 = p1;
    Polynomial cur2 = p2;
    while(cur1->next != NULL){
        while(cur2->next != NULL){
            addP(p3, cur1->next->no + cur2->next->no, cur1->next->num * cur2->next->num, false);
            cur2 = cur2->next;
        }
        cur2 = p2;
        cur1 = cur1->next;
    }
}

int main()
{
    Polynomial p1;
    createP(p1);
    addP(p1, 1, 18, true);
    addP(p1, 4, -9, true);
   /* addP(p1, 2, 5, true);
    addP(p1, 5, 4, true);
    addP(p1, 3, 1, true);*/
    msg(p1);
    cout<<"------------------"<<endl;

    Polynomial p2;
    createP(p2);
    addP(p2, 1, 18, true);
    addP(p2, 1, -9, true);
    /*addP(p2, 2, 5, true);
    addP(p2, 5, 4, true);
    addP(p2, 3, 1, true);
    addP(p2, 11, 1, true);*/
    msg(p2);
    cout<<"------------------"<<endl;

    Polynomial addp;
    createP(addp);
    add(p1, p2, addp);
    msg(addp);
    cout<<"以上是加法------------------"<<endl;

    Polynomial subp;
    createP(subp);
    sub(p1, p2, subp);
    msg(subp);
    cout<<"以上是减法------------------"<<endl;

    Polynomial multip;
    createP(multip);
    multi(p1, p2, multip);
    msg(multip);
    cout<<"以上是乘法------------------"<<endl;

    return 0;
}
