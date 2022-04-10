#include <iostream>
#include <queue>

#define VERTEXCOUNT 8
using namespace std;

char vertex[VERTEXCOUNT] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
bool isVisited[VERTEXCOUNT] = {false, false, false, false, false, false, false, false};
int arr[VERTEXCOUNT][VERTEXCOUNT] = {
    {0, 1, 1, 0, 0, 0, 0, 0},
    {1, 0, 0, 1, 1, 0, 0, 0},
    {1, 0, 0, 0, 0, 1, 1, 0},
    {0, 1, 0, 0, 0, 0, 0, 1},
    {0, 1, 0, 0, 0, 0, 0, 1},
    {0, 0, 1, 0, 0, 0, 1, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0}
};
// 找到指定顶点的第一个邻接节点
int getFirstNeighbor(int n)
{
    for(int i = 0; i < VERTEXCOUNT; i++){
        if(arr[n][i] == 1) return i;
    }
    return -1;
}

// z找到指定顶点的下一个邻接顶点
int getNextNeighbor(int n, int k)
{
    for(int i = k+1; i < VERTEXCOUNT; i++){
        if(arr[n][i] == 1) return i;
    }
    return -1;
}

// 广度优先遍历,wrong!!!
void MyBFS()
{
    queue<int> level;
    // 把元素数组的第一个元素放进去
    level.push(0);
    bool isOver = false;
    int temp;
    int len;
    while(! isOver){
        len = level.size();
        isOver = true;
        for(int i = 0; i < len; i++){
            temp = level.front();
            level.pop();
            if(! isVisited[temp]){
                cout<<vertex[temp]<<" ";
                isVisited[temp] = true;
            }
            // 访问当前点的所有邻接点
            for(int j = 0; j < VERTEXCOUNT; j++){
                if(arr[i][j] == 1){
                    if(! isVisited[j]){
                        level.push(j);
                        isOver = false;
                    }
                }
            }

        }
    }

}

void DFS(int i)
{
    // 选择一个点作为初始点v,访问并标记已经访问
    cout<<vertex[i]<<" ";
    isVisited[i] = true;
    // 查找节点v的第一个邻接点w
    int w = getFirstNeighbor(i);
    // 若w存在
    while(w != -1){
        // 若w未被访问，对w进行DFS
        if(!isVisited[w]){
            DFS(w);
        }
        // 若w已被访问，查找节点v的w邻接节点的下一个邻接节点，转到3
        w = getNextNeighbor(i, w);
    }
}

void MyDFS()
{
    for(int i = 0; i < VERTEXCOUNT; i++){
        if(! isVisited[i]){
            DFS(i);
        }
    }
}

int main()
{
    //MyDFS();
    DFS(0);

    cout<<endl;

    //MyBFS();

    return 0;
}
