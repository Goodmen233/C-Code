#include <iostream>
#include <string>
#include <queue>

#define MAXVERTEX 10
using namespace std;


struct Node
{
    int index;// ָ���ڽӵĽڵ�����λ��
    string data;// ����������
    bool isVisited;
    struct Node *next;// ָ����һ��Ԫ��
};

struct LinkGraph
{
    Node* arr[MAXVERTEX];// �ڽӱ������
    int vexnum,edgnum;// ��������ͱ���
};

int  getIndex(LinkGraph lg, string s)
{
    for(int i = 0; i < lg.vexnum; i++){
        string str = lg.arr[i]->data;
        if(str.compare(s) == 0) return i;
    }
    return -1;
}

// ���ȶ����ͼ
void init(LinkGraph &lg)
{
    cout<<"�����붥������ͱߵĸ�����";
    cin>>lg.vexnum>>lg.edgnum;
    cout<<"���������붥������ƣ�";
    for(int i = 0; i < lg.vexnum; i++){
        Node *cur = new Node;
        string s;
        cin>>s;
        // ��ʼ�����������Ԫ��
        cur->index = -1;
        cur->data = s;
        cur->isVisited = false;
        cur->next = NULL;
        lg.arr[i] = cur;
    }
    string v1,v2;
    Node *cur;// ����β��ĸ���ָ��
    for(int i = 0; i < lg.edgnum; i++){
        cout<<"����������ÿһ�������ӵĵ�:";
        cin>>v1>>v2;
        int loc1 = getIndex(lg, v1);
        int loc2 = getIndex(lg, v2);
        Node *cur1 = new Node;
        cur1->data = "";
        cur1->index = loc2;
        cur1->isVisited = false;
//        cur1->next = lg.arr[loc1]->next;
//        lg.arr[loc1]->next = cur1;// ͷ��
        cur = lg.arr[loc1];
        while(cur->next != NULL) cur = cur->next;
        cur->next = cur1;
        Node *cur2 = new Node;
        cur2->data = "";
        cur2->index = loc1;
        cur2->isVisited = false;
//        cur2->next = lg.arr[loc2]->next;
//        lg.arr[loc2]->next = cur2;// ͷ��
        cur = lg.arr[loc2];
        while(cur->next != NULL) cur = cur->next;
        cur->next = cur2;
    }
}

// DFS,nΪָ�������
void DFS(LinkGraph lg, int n)
{
    Node *cur = lg.arr[n];
    cout<<cur->data<<" ";
    cur->isVisited = true;
    cur = cur->next;// ��ȡ��һ���ڽӵ�
    while(cur != NULL){
        int index = cur->index;
        if(! lg.arr[index]->isVisited){
            DFS(lg, index);
        }
        cur = cur->next;
    }
}

// BFS,nΪָ�������
void BFS(LinkGraph lg, int n)
{
    queue<Node*> level;
    // ����һ��Ԫ�ط��������
    level.push(lg.arr[n]);
    lg.arr[n]->isVisited = true;
    Node* temp;
    // �����в�Ϊ��
    while(! level.empty()){
        // ��ȡ���е�Ԫ�أ�������
        temp = level.front();
        level.pop();
        // ���ֵ��������Ѿ�����
        cout<<temp->data<<" ";
        // ����ǰ�����ڽӵ�
        while(temp->next != NULL){
            temp = temp->next;
            // ��ȡ�ڽӵ������λ��
            int index = temp->index;
            // ���û�б����ʹ�
            if(! lg.arr[index]->isVisited){
                // ���õ�Ž�
                level.push(lg.arr[index]);
                lg.arr[index]->isVisited = true;
            }
        }
    }

}

int main()
{
    LinkGraph lg;
    init(lg);
    //cout<<"BFS:"<<endl;
    //BFS(lg,0);
    cout<<"DFS:"<<endl;
    DFS(lg, 0);
    return 0;
}
