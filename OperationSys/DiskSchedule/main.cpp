#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

#define MaxNumber 100
int TrackOrder[MaxNumber];              // 磁道访问序列
int MoveDistance[MaxNumber];            // 移动距离
double AverageDistance;                 // 平均寻道长度
bool direction;                         // 方向 1表示从里到外 0反之
bool isVisited[MaxNumber];              // 存储是否访问
int DiskNum;
int StartDisk;

void init()
{
    ifstream input("data.txt");
    input>>DiskNum;
    for(int i = 0; i < DiskNum; i++){
        input>>TrackOrder[i];
        isVisited[i] = false;
    }
    input>>StartDisk;
    input>>direction;
    AverageDistance = 0.00;
}

void FCFS()
{
    int cur = StartDisk;// 存储当前所在的磁道号
    // 输出从哪个磁道开始
    cout<<"从"<<StartDisk<<"号磁道开始"<<endl;
    // 遍历所有磁道，输出访问的值及移动距离、并修改当前的磁道号,存储总移动距离
    for(int i = 0; i < DiskNum; i++){
        MoveDistance[i] = abs(cur-TrackOrder[i]);
        cout<<"被访问的下一个磁道号："<<TrackOrder[i]<<"\t移动距离："<<MoveDistance[i]<<""<<endl;
        cur = TrackOrder[i];
        AverageDistance += MoveDistance[i];
    }
    // 计算平均寻道长度
    cout<<"FIFO的平均寻道长度为："<<AverageDistance / DiskNum<<""<<endl;
}

void SSTF()
{
    int cur = StartDisk;// 存储当前所在的磁道号
    // 输出从哪个磁道开始
    cout<<"从"<<StartDisk<<"号磁道开始"<<endl;
    // 需要循环次数=磁道数量(都被访问)
    for(int i = 0; i < DiskNum; i++){
        // 依次计算每次未被访问的磁道需要移动的距离
        for(int j = 0; j < DiskNum; j++){
            if(isVisited[j] == false){
                MoveDistance[j] = abs(cur-TrackOrder[j]);
            }
        }
        // 找出最小移动距离的磁道，并移动(输出访问的值及移动距离、并修改当前的磁道号,存储总移动距离)
        int minDistance = 999999;
        int minIndex = -1;
        for(int k = 0 ; k < DiskNum; k++){
            if(isVisited[k] == false && minDistance > MoveDistance[k]){
                minDistance = MoveDistance[k];
                minIndex = k;
            }
        }
        cout<<"被访问的下一个磁道号："<<TrackOrder[minIndex]<<"\t移动距离："<<minDistance<<""<<endl;
        cur = TrackOrder[minIndex];
        isVisited[minIndex] = true;
        AverageDistance += minDistance;
    }
    // 计算平均寻道长度
    cout<<"SSTF的平均寻道长度为："<<AverageDistance / DiskNum<<""<<endl;
}

void SCAN()
{
    int temp;
    // 先给磁道序列排序
    for(int i = 0; i < DiskNum; i++){
        for(int j = 0; j < DiskNum - 1 - i; j++){
            if(TrackOrder[j] > TrackOrder[j + 1]){
                temp = TrackOrder[j];
                TrackOrder[j] = TrackOrder[j + 1];
                TrackOrder[j + 1] = temp;
            }
        }
    }
    int cur = StartDisk;// 存储当前所在的磁道号
    int index = -1;// 存储小于开始磁道的下标
    // 输出从哪个磁道开始
    cout<<"从"<<StartDisk<<"号磁道开始,";
    // 输出从哪个方向
    if(direction){
        cout<<"从里到外"<<endl;
    }else{
        cout<<"从外到里"<<endl;
    }
    // 找到小于开始磁道的下标并存储
    for(int i = 0; i < DiskNum; i++){
        if(TrackOrder[i] <= StartDisk){
            index = i;
            continue;
        }
        break;
    }
    // 一共需要两次遍历
    for(int i = 0; i < 2; i++){
        // 从里到外
        if(direction){
            // 遍历大于当前的磁道号，输出访问的值及移动距离、并修改当前的磁道号,存储总移动距离
            for(int j = index + 1; j < DiskNum; j++){
                MoveDistance[j] = abs(cur-TrackOrder[j]);
                cout<<"被访问的下一个磁道号："<<TrackOrder[j]<<"\t移动距离："<<MoveDistance[j]<<""<<endl;
                cur = TrackOrder[j];
                AverageDistance += MoveDistance[j];
            }
        // 从外到里
        }else{
            // 从小于开始磁道的下标遍历，输出访问的值及移动距离、并修改当前的磁道号,存储总移动距离
            for(int j = index; j >= 0; j--){
                MoveDistance[j] = abs(cur-TrackOrder[j]);
                cout<<"被访问的下一个磁道号："<<TrackOrder[j]<<"\t移动距离："<<MoveDistance[j]<<""<<endl;
                cur = TrackOrder[j];
                AverageDistance += MoveDistance[j];
            }
        }
        // 修改方向
        direction = !direction;
    }
    // 计算平均寻道长度
    cout<<"SCAN的平均寻道长度为："<<AverageDistance / DiskNum<<""<<endl;
}

void CSCAN()
{
    int temp;
    // 先给磁道序列排序
    for(int i = 0; i < DiskNum; i++){
        for(int j = 0; j < DiskNum - 1 - i; j++){
            if(TrackOrder[j] > TrackOrder[j + 1]){
                temp = TrackOrder[j];
                TrackOrder[j] = TrackOrder[j + 1];
                TrackOrder[j + 1] = temp;
            }
        }
    }
    int cur = StartDisk;// 存储当前所在的磁道号
    int index = -1;// 存储小于开始磁道的下标
    // 输出从哪个磁道开始
    cout<<"从"<<StartDisk<<"号磁道开始";
    // 输出从哪个方向
    if(direction){
        cout<<"从里到外"<<endl;
    }else{
        cout<<"从外到里"<<endl;
    }
    // 找到小于开始磁道的下标并存储
    for(int i = 0; i < DiskNum; i++){
        if(TrackOrder[i] <= StartDisk){
            index = i;
            continue;
        }
        break;
    }
    // 从里到外
    if(direction){
        // 一共需要两次遍历
        for(int i = 0; i < 2; i++){
            if(i == 0){
                // 遍历大于当前的磁道号，输出访问的值及移动距离、并修改当前的磁道号,存储总移动距离
                for(int j = index + 1; j < DiskNum; j++){
                    MoveDistance[j] = abs(cur-TrackOrder[j]);
                    cout<<"被访问的下一个磁道号："<<TrackOrder[j]<<"\t移动距离："<<MoveDistance[j]<<""<<endl;
                    cur = TrackOrder[j];
                    AverageDistance += MoveDistance[j];
                }
            }else{
                for(int j = 0; j <= index; j++){
                    MoveDistance[j] = abs(cur-TrackOrder[j]);
                    cout<<"被访问的下一个磁道号："<<TrackOrder[j]<<"\t移动距离："<<MoveDistance[j]<<""<<endl;
                    cur = TrackOrder[j];
                    AverageDistance += MoveDistance[j];
                }
            }
        }
    // 从外到里
    }else{
        // 一共需要两次遍历
        for(int i = 0; i < 2; i++){
            if(i == 0){
                // 遍历大于当前的磁道号，输出访问的值及移动距离、并修改当前的磁道号,存储总移动距离
                for(int j = index; j >= 0; j--){
                    MoveDistance[j] = abs(cur-TrackOrder[j]);
                    cout<<"被访问的下一个磁道号："<<TrackOrder[j]<<"\t移动距离："<<MoveDistance[j]<<""<<endl;
                    cur = TrackOrder[j];
                    AverageDistance += MoveDistance[j];
                }
            }else{
                for(int j = DiskNum - 1; j > index; j--){
                    MoveDistance[j] = abs(cur-TrackOrder[j]);
                    cout<<"被访问的下一个磁道号："<<TrackOrder[j]<<"\t移动距离："<<MoveDistance[j]<<""<<endl;
                    cur = TrackOrder[j];
                    AverageDistance += MoveDistance[j];
                }
            }
        }
    }

    // 计算平均寻道长度
    cout<<"SCAN的平均寻道长度为："<<AverageDistance / DiskNum<<""<<endl;
}

int main()
{
    init();
    cout<<"--------------------FCFS-----------------------------"<<endl;
    FCFS();
    init();
    cout<<"--------------------SSTF-----------------------------"<<endl;
    SSTF();
    init();
    cout<<"--------------------SCAN-----------------------------"<<endl;
    SCAN();
    init();
    cout<<"--------------------CSCAN-----------------------------"<<endl;
    CSCAN();
    return 0;
}
