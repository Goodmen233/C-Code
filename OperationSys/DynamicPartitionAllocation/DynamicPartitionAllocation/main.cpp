#include <iostream>
#include <fstream>

using namespace std;

#define MaxNumber 100
int  FreePartition[MaxNumber];          // 空闲分区大小
int  FirstPartition[MaxNumber];         // FF算法
int  CycleFirstPartition[MaxNumber];    // NF算法
int  BestPartition[MaxNumber];          // BF算法
int  WorstPartition[MaxNumber];         // WF算法
int  ProcessNeed[MaxNumber];            // 进程需要的分区大小
int  PartitionNum,ProcessNum;           // 分区数量、进程数量

// 通过记事本输入数据
void initByTxt()
{
    ifstream input("data.txt");
    input>>PartitionNum>>ProcessNum;
    for(int i = 0; i < ProcessNum; i++){
        input>>ProcessNeed[i];
    }
    for(int i = 0; i < PartitionNum; i++){
        input>>FreePartition[i];
        FirstPartition[i] = FreePartition[i];
        CycleFirstPartition[i] = FreePartition[i];
        BestPartition[i] = FreePartition[i];
        WorstPartition[i] = FreePartition[i];
    }

}

// 控制台输入数据
void init()
{
    // 输入分区数量、进程数量
    cout<<"请输入分区数量及进程数量(不超过100)：";
    cin>>PartitionNum>>ProcessNum;
    // 输入进程需要的分区大小
    cout<<"请输入每个进程需要的分区大小：";
    for(int i = 0; i < ProcessNum; i++){
        cin>>ProcessNeed[i];
    }
    // 输入分区大小并初始化四种算法的初始值
    cout<<"请输入每个分区大小：";
    for(int i = 0; i < PartitionNum; i++){
        cin>>FreePartition[i];
        FirstPartition[i] = FreePartition[i];
        CycleFirstPartition[i] = FreePartition[i];
        BestPartition[i] = FreePartition[i];
        WorstPartition[i] = FreePartition[i];
    }
}

void FF()
{
    cout<<"---------------FF---------------------"<<endl;
    bool isIn = false;
    // 遍历每个进程
    for(int i = 0; i < ProcessNum; i++){
        isIn = false;
        // 遍历分区，找到可以存放当前进程的分区
        for(int j = 0; j < PartitionNum; j++){
            // 如果找到则输出信息，修改分配分区大小
            if(FirstPartition[j] >= ProcessNeed[i]){
                isIn = true;
                cout<<"进程"<<ProcessNeed[i]<<"分配到"<<FirstPartition[j]<<"分区。";
                FirstPartition[j] -= ProcessNeed[i];
                cout<<"当前分区情况为:";
                for(int k = 0; k < PartitionNum; k++){
                    cout<<FirstPartition[k]<<" ";
                }
                cout<<endl;
                break;
            }
        }
        // 如果没找到，输出提示
        if(!isIn)
            cout<<"进程"<<ProcessNeed[i]<<"无法分配到分区！"<<endl;
    }
}

void NF()
{
    cout<<"---------------NF---------------------"<<endl;
    // 定义一个指针
    int cur = 0;
    bool isIn = false;
    // 遍历所有进程
    for(int i = 0; i < ProcessNum; i++){
        isIn = false;
        // 遍历所有分区，从上次分配的位置开始，找到可以存放的分区
        int temp = PartitionNum;// 循环次数
        while(temp > 0){
            // 如果找到则输出信息，修改分配分区大小,指针缓存当前分区位置
            if(CycleFirstPartition[cur] >= ProcessNeed[i]){
                isIn = true;
                cout<<"进程"<<ProcessNeed[i]<<"分配到"<<CycleFirstPartition[cur]<<"分区。";
                CycleFirstPartition[cur] -= ProcessNeed[i];
                cout<<"当前分区情况为:";
                for(int k = 0; k < PartitionNum; k++){
                    cout<<CycleFirstPartition[k]<<" ";
                }
                cout<<endl;
                break;
            }
            if(++cur == PartitionNum) cur = 0;// 回到首个
            temp--;
        }
        // 如果没找到，输出提示
        if(!isIn)
            cout<<"进程"<<ProcessNeed[i]<<"无法分配到分区！"<<endl;
    }

}

void BF()
{
    int temp = -1;
    bool isIn = false;
    cout<<"---------------BF---------------------"<<endl;
    // 遍历所有进程
    for(int i = 0; i < ProcessNum; i++){
        // 先从小到大排序分区
        for(int x = 0; x < PartitionNum; x++){
            int min = BestPartition[x];
            int cur = x;
            for(int y = x + 1; y < PartitionNum; y++){
                if(BestPartition[y] < min){
                    min = BestPartition[y];
                    cur = y;
                }
            }
            if(cur != x){
                temp = BestPartition[cur];
                BestPartition[cur] = BestPartition[x];
                BestPartition[x] = temp;
            }
        }

        // 和FF一样
        isIn = false;
        // 遍历分区，找到可以存放当前进程的分区
        for(int j = 0; j < PartitionNum; j++){
            // 如果找到则输出信息，修改分配分区大小
            if(BestPartition[j] >= ProcessNeed[i]){
                isIn = true;
                cout<<"进程"<<ProcessNeed[i]<<"分配到"<<BestPartition[j]<<"分区。";
                BestPartition[j] -= ProcessNeed[i];
                cout<<"当前分区情况为:";
                for(int k = 0; k < PartitionNum; k++){
                    cout<<BestPartition[k]<<" ";
                }
                cout<<endl;
                break;
            }
        }
        // 如果没找到，输出提示
        if(!isIn)
            cout<<"进程"<<ProcessNeed[i]<<"无法分配到分区！"<<endl;
    }

}

void WF()
{
    int temp = -1;
    bool isIn = false;
    cout<<"---------------WF---------------------"<<endl;
    // 遍历所有进程
    for(int i = 0; i < ProcessNum; i++){
        // 先从大到小排序分区
        for(int x = 0; x < PartitionNum; x++){
            int max = WorstPartition[x];
            int cur = x;
            for(int y = x + 1; y < PartitionNum; y++){
                if(WorstPartition[y] > max){
                    max = WorstPartition[y];
                    cur = y;
                }
            }
            if(cur != x){
                temp = WorstPartition[cur];
                WorstPartition[cur] = WorstPartition[x];
                WorstPartition[x] = temp;
            }
        }

        // 和FF一样
        isIn = false;
        // 遍历分区，找到可以存放当前进程的分区
        for(int j = 0; j < PartitionNum; j++){
            // 如果找到则输出信息，修改分配分区大小
            if(WorstPartition[j] >= ProcessNeed[i]){
                isIn = true;
                cout<<"进程"<<ProcessNeed[i]<<"分配到"<<WorstPartition[j]<<"分区。";
                WorstPartition[j] -= ProcessNeed[i];
                cout<<"当前分区情况为:";
                for(int k = 0; k < PartitionNum; k++){
                    cout<<WorstPartition[k]<<" ";
                }
                cout<<endl;
                break;
            }
        }
        // 如果没找到，输出提示
        if(!isIn)
            cout<<"进程"<<ProcessNeed[i]<<"无法分配到分区！"<<endl;
    }

}

int main()
{
    init();
    cout<<"当前进程情况：";
    for(int i = 0; i < ProcessNum; i++){
        cout<<ProcessNeed[i]<<" ";
    }
    cout<<endl;
    cout<<"当前分区情况：";
    for(int i = 0; i < PartitionNum; i++){
        cout<<FreePartition[i]<<" ";
    }
    cout<<endl;
    // ---------------FF---------------------
    FF();
    // ---------------NF---------------------
    NF();
    // ---------------BF---------------------
    BF();
    // ---------------WF---------------------
    WF();
    return 0;
}
