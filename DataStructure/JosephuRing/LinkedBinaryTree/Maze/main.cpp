#include <iostream>

using namespace std;

// 定义一个结构体来表示小球当前位置
struct Point
{
    int x;
    int y;
};

//------------------------------栈----------------------------------
typedef struct Point Elemtype;
// 定义一个链栈
typedef struct Node
{
    Elemtype elem;
    int direc;// 1表示右，2表示下，3表示左，4表示上
    struct Node* next;
}Node, *LinkedStack;

// 创建一个空栈
void createStack(LinkedStack &l)
{
    l = new Node;
    l->next = NULL;
}

// 栈是否为空
bool isEmpty(LinkedStack l)
{
    if(l->next == NULL){
        return true;
    }
    return false;
}

// 压栈
void push(LinkedStack &l, Elemtype e)
{
    LinkedStack temp = new Node;
    temp->elem = e;
    temp->next = l->next;;
    l->next = temp;
}

// 出栈
void pop(Elemtype &e, LinkedStack &l)
{
    if(isEmpty(l)) return;
    e = l->next->elem;
    l->next = l->next->next;
}

// 查看栈顶元素
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
        // 如果起点等于终点
        if(mazeMap[point.x][point.y] == mazeMap[endP.x][endP.y]) return;
        // 起点不等于终点，按照右下左上的顺序检索
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
            // 该点不可达，标记
            mazeMap[point.x][point.y] = 3;
            Point temp;
            pop(temp, ls);
            // 再拿栈顶作为新坐标
            if(isEmpty(ls)) return;
            point = peek(ls);
        }
    }
}

//-----------------------------------------------------------------------
int main()
{
    // 定义一个二维数组作为mazeMap，其中1表示墙，0表示可达，2表示走过，3表示不可达
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
    // 定义一个栈来存放走过的路径
    LinkedStack ls;
    createStack(ls);
    // 起点
    Point startP = {1, 1};
    // 终点
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
    // 栈的倒序输出
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
