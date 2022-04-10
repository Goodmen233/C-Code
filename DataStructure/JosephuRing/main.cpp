#include <iostream>

using namespace std;

// 单向环形链表
typedef struct LNode
{
    int data;
    int pssd;
    struct LNode *next;
}LNode, *LinkedList;

// 创建一个编号连续的环形链表
void createL(LinkedList &l, int num)
{
    // 辅助指针
    LinkedList cur = l;
    for(int i = 1; i <= num; i++){
        // 如果第一个节点
        if(i == 1){
            l->data = i;
            l->next = l;
        }else{
            LinkedList tempL = new LNode;
            // 新节点中放数据
            tempL->data = i;
            tempL->next = l;
            // 辅助指针指向下个元素
            cur->next = tempL;
            cur = cur->next;
        }
    }
}

// 遍历链表
void msg(LinkedList &l)
{
    // 辅助指针
    LinkedList cur = l;
    while(true){
        cout<<cur->data<<endl;
        cur = cur->next;
        if(cur == l) break;
    }
}

// Josephu问题
void Josephu(LinkedList l, int startNo, int countNum)
{
    // 使用辅助指针
    LinkedList nowCur;
    LinkedList preCur = l;
    while(true){
        preCur = preCur->next;
        if(preCur->next == l) break;
    }
    // 使指针移动到开始数数的前一个序号
    for(int i = 0; i < startNo - 1; i++){
        preCur = preCur->next;
    }
    nowCur = preCur->next;
    while(true){
        // 开始计数，移动countNum-1次
        for(int i = 0; i < countNum - 1; i++){
            nowCur = nowCur->next;
            preCur = preCur->next;
        }
        cout<<"取出："<<nowCur->data<<endl;
        // 使取出的节点的前一个节点指向取出节点的下一个节点
        preCur->next = nowCur->next;
        nowCur = preCur->next;
        // 如果只剩下一个
        if(preCur == nowCur){
            cout<<"最后取出："<<nowCur->data<<endl;
            break;
        }
    }

}

// 学校的约瑟夫
void Josephu(LinkedList l, int countNum)
{
    // 使用辅助指针
    LinkedList nowCur;
    LinkedList preCur = l;
    while(true){
        preCur = preCur->next;
        if(preCur->next == l) break;
    }
    nowCur = preCur->next;
    while(true){
        // 开始计数，移动countNum-1次
        for(int i = 0; i < countNum - 1; i++){
            nowCur = nowCur->next;
            preCur = preCur->next;
        }
        cout<<"取出："<<nowCur->data<<endl;
        countNum = nowCur->pssd;
        // 使取出的节点的前一个节点指向取出节点的下一个节点
        preCur->next = nowCur->next;
        nowCur = preCur->next;
        // 如果只剩下一个
        if(preCur == nowCur){
            cout<<"最后取出："<<nowCur->data<<endl;
            break;
        }
    }

}

// 输入每个人的密码
void inputPssd(LinkedList &l)
{
    // 辅助指针
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
    cout<<"参与人数：";
    cin>>n;
    createL(l,n);
    inputPssd(l);
    // 第一题
    msg(l);
    // 第二题 3 1 7 2 4 8 4
    //Josephu(l,20);
    // 6 1 4 7 2 3 5
    // 1 2 4 3 5
    // 第三题
    cout<<"请输入一个初始的countNum值：";
    int countNum = 0;
    cin>>countNum;
    Josephu(l,countNum);
    return 0;
}
