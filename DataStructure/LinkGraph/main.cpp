#include <iostream>
#include <string>
#include <queue>

#define MAXVERTEX 10
using namespace std;


struct Node
{
    int index;// 指向邻接的节点数组位置
    string data;// 用来放数据
    bool isVisited;
    struct Node *next;// 指向下一个元素
};

struct LinkGraph
{
    Node* arr[MAXVERTEX];// 邻接表的数组
    int vexnum,edgnum;// 顶点个数和边数
};

int  getIndex(LinkGraph lg, string s)
{
    for(int i = 0; i < lg.vexnum; i++){
        string str = lg.arr[i]->data;
        if(str.compare(s) == 0) return i;
    }
    return -1;
}

// 事先定义好图
void init(LinkGraph &lg)
{
    cout<<"请输入顶点个数和边的个数：";
    cin>>lg.vexnum>>lg.edgnum;
    cout<<"请依次输入顶点的名称：";
    for(int i = 0; i < lg.vexnum; i++){
        Node *cur = new Node;
        string s;
        cin>>s;
        // 初始化数组里面的元素
        cur->index = -1;
        cur->data = s;
        cur->isVisited = false;
        cur->next = NULL;
        lg.arr[i] = cur;
    }
    string v1,v2;
    Node *cur;// 用来尾插的辅助指针
    for(int i = 0; i < lg.edgnum; i++){
        cout<<"请依次输入每一条边连接的点:";
        cin>>v1>>v2;
        int loc1 = getIndex(lg, v1);
        int loc2 = getIndex(lg, v2);
        Node *cur1 = new Node;
        cur1->data = "";
        cur1->index = loc2;
        cur1->isVisited = false;
//        cur1->next = lg.arr[loc1]->next;
//        lg.arr[loc1]->next = cur1;// 头插
        cur = lg.arr[loc1];
        while(cur->next != NULL) cur = cur->next;
        cur->next = cur1;
        Node *cur2 = new Node;
        cur2->data = "";
        cur2->index = loc1;
        cur2->isVisited = false;
//        cur2->next = lg.arr[loc2]->next;
//        lg.arr[loc2]->next = cur2;// 头插
        cur = lg.arr[loc2];
        while(cur->next != NULL) cur = cur->next;
        cur->next = cur2;
    }
}

// DFS,n为指定的起点
void DFS(LinkGraph lg, int n)
{
    Node *cur = lg.arr[n];
    cout<<cur->data<<" ";
    cur->isVisited = true;
    cur = cur->next;// 获取第一个邻接点
    while(cur != NULL){
        int index = cur->index;
        if(! lg.arr[index]->isVisited){
            DFS(lg, index);
        }
        cur = cur->next;
    }
}

// BFS,n为指定的起点
void BFS(LinkGraph lg, int n)
{
    queue<Node*> level;
    // 将第一个元素放入队列中
    level.push(lg.arr[n]);
    lg.arr[n]->isVisited = true;
    Node* temp;
    // 当队列不为空
    while(! level.empty()){
        // 获取队列的元素，并弹出
        temp = level.front();
        level.pop();
        // 输出值，并标记已经访问
        cout<<temp->data<<" ";
        // 当当前点有邻接点
        while(temp->next != NULL){
            temp = temp->next;
            // 获取邻接点的数组位置
            int index = temp->index;
            // 如果没有被访问过
            if(! lg.arr[index]->isVisited){
                // 将该点放进
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
