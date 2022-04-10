#include <iostream>

using namespace std;

// ����һ���ṹ������ʾС��ǰλ��
struct Point
{
    int x;
    int y;
};

//------------------------------ջ----------------------------------
typedef struct Point Elemtype;
// ����һ����ջ
typedef struct Node
{
    Elemtype elem;
    int direc;// 1��ʾ�ң�2��ʾ�£�3��ʾ��4��ʾ��
    struct Node* next;
}Node, *LinkedStack;

// ����һ����ջ
void createStack(LinkedStack &l)
{
    l = new Node;
    l->next = NULL;
}

// ջ�Ƿ�Ϊ��
bool isEmpty(LinkedStack l)
{
    if(l->next == NULL){
        return true;
    }
    return false;
}

// ѹջ
void push(LinkedStack &l, Elemtype e)
{
    LinkedStack temp = new Node;
    temp->elem = e;
    temp->next = l->next;;
    l->next = temp;
}

// ��ջ
void pop(Elemtype &e, LinkedStack &l)
{
    if(isEmpty(l)) return;
    e = l->next->elem;
    l->next = l->next->next;
}

// �鿴ջ��Ԫ��
Elemtype peek(LinkedStack l)
{
    return l->next->elem;
}

void runMaze(LinkedStack &ls, int mazeMap[10][10], Point startP, Point endP)
{
    Point point = startP;
    push(ls, point);
    mazeMap[point.x][point.y] = 2;
    while(true){
        // ����������յ�
        if(mazeMap[point.x][point.y] == mazeMap[endP.x][endP.y]) return;
        // ��㲻�����յ㣬�����������ϵ�˳�����
        if(mazeMap[point.x][point.y + 1] == 0){
            ls->next->direc = 1;
            point = {point.x, point.y + 1};
            mazeMap[point.x][point.y] = 2;
            push(ls, point);
        }else if(mazeMap[point.x + 1][point.y] == 0){
            ls->next->direc = 2;
            point = {point.x+1, point.y};
            mazeMap[point.x][point.y] = 2;
            push(ls, point);
        }else if(mazeMap[point.x][point.y - 1] == 0){
            ls->next->direc = 3;
            point = {point.x, point.y-1};
            mazeMap[point.x][point.y] = 2;
            push(ls,point);
        }else if(mazeMap[point.x - 1][point.y] == 0){
            ls->next->direc = 4;
            point = {point.x-1, point.y};
            mazeMap[point.x][point.y] = 2;
            push(ls,point);
        }else{
            // �õ㲻�ɴ���
            mazeMap[point.x][point.y] = 3;
            Point temp;
            pop(temp, ls);
            // ����ջ����Ϊ������
            if(isEmpty(ls)) return;
            point = peek(ls);
        }
    }
}

//-----------------------------------------------------------------------
int main()
{
    // ����һ����ά������ΪmazeMap������1��ʾǽ��0��ʾ�ɴ2��ʾ�߹���3��ʾ���ɴ�
    int mazeMap[10][10] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 1, 0, 0, 0, 1, 0, 1},
        {1, 0, 0, 1, 0, 0, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 1, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };
    // ����һ��ջ������߹���·��
    LinkedStack ls;
    createStack(ls);
    // ���
    Point startP = {1, 1};
    // �յ�
    Point endP = {8, 8};

    runMaze(ls, mazeMap, startP, endP);

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            cout<<mazeMap[i][j]<<" ";
        }
        cout<<endl;
    }

    LinkedStack templs;
    createStack(templs);
    // ջ�ĵ������
    while(ls->next != NULL){
        int tempd = ls->next->direc;
        Elemtype e;
        pop(e, ls);
        push(templs, e);
        templs->next->direc = tempd;
    }
    while(templs->next != NULL){
        int tempd = templs->next->direc;
        Elemtype e;
        pop(e , templs);
        cout<<"("<<e.x<<", "<<e.y<<", "<<tempd<<")"<<" ";
    }

    return 0;
}
