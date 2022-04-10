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
// �ҵ�ָ������ĵ�һ���ڽӽڵ�
int getFirstNeighbor(int n)
{
    for(int i = 0; i < VERTEXCOUNT; i++){
        if(arr[n][i] == 1) return i;
    }
    return -1;
}

// z�ҵ�ָ���������һ���ڽӶ���
int getNextNeighbor(int n, int k)
{
    for(int i = k+1; i < VERTEXCOUNT; i++){
        if(arr[n][i] == 1) return i;
    }
    return -1;
}

// ������ȱ���,wrong!!!
void MyBFS()
{
    queue<int> level;
    // ��Ԫ������ĵ�һ��Ԫ�طŽ�ȥ
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
            // ���ʵ�ǰ��������ڽӵ�
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
    // ѡ��һ������Ϊ��ʼ��v,���ʲ�����Ѿ�����
    cout<<vertex[i]<<" ";
    isVisited[i] = true;
    // ���ҽڵ�v�ĵ�һ���ڽӵ�w
    int w = getFirstNeighbor(i);
    // ��w����
    while(w != -1){
        // ��wδ�����ʣ���w����DFS
        if(!isVisited[w]){
            DFS(w);
        }
        // ��w�ѱ����ʣ����ҽڵ�v��w�ڽӽڵ����һ���ڽӽڵ㣬ת��3
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
