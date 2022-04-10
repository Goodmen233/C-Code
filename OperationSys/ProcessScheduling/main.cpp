#include <iostream>

using namespace std;

static int MaxNum = 100;// 进程最大数

// 判断进程是否全部被执行完
bool isOverFun(bool isOver[],int len)
{
    for(int i = 0; i < len; i++){
        if(isOver[i] == false)
            return false;
    }
    return true;
}

int main()
{
    // ----------------------实验所需变量
    int ArrivalTime[MaxNum];// 每个进程到达时间
    int ServiceTime[MaxNum];// 每个进程服务时间
    int FinishTime[MaxNum];// 每个进程的完成时间                           开始运行时间+服务时间
    int WholeTime[MaxNum];// 每个进程的周转时间                            完成时间-到达时间
    double WeightWholeTime[MaxNum];// 每个进程的带权周转时间               周转时间/服务时间
    double AverageWT_FCFS, AverageWT_SJF;// 两种算法的平均周转时间         所有周转时间之和/n
    double AverageWWT_FCFS, AverageWWT_SJF;// 两种算法的带权平均周转时间   所有带权周转时间之和/n

    // ----------------------用户输入----------------------
    // 输入进程个数n
    int n = -1;
    while(n > 100 || n < 0){
        cout<<"请输入进程个数：";
        cin>>n;
        if(n > 100 || n < 0)
            cout<<"输入不合法，请重新输入"<<endl;
    }

    // 输入每个进程对应的到达时间和服务时间
    for(int i = 0; i < n; i++){
        cout<<"请输入第"<<i + 1<<"个进程的到达时间及服务时间：";
        cin>>ArrivalTime[i]>>ServiceTime[i];
    }

    // 计算的一些缓存变量
    int startTime = 0;// 缓存每个进程开始执行时间段的时间节点
    double sumWholeTime = 0;// 缓存总的周期时间
    double sumWeightWholeTime = 0;// 缓存总的带权周转时间

    // ----------------------FCFS算法----------------------
    cout<<"----------------------FCFS算法----------------------"<<endl;
    // 设置进程是否已经被执行布尔标识
    bool isOver[n];
    for(int i = 0; i < n; i++){
        isOver[i] = false;
    }
    int minArrivalTime = 999999999;// 缓存当前最小到达时间
    int index = -1;// 缓存下标
    // 当进程没有被全部执行
    while(!isOverFun(isOver, sizeof(isOver))){
        // 获取剩下没有执行进程中 到达时间最早的进程
        minArrivalTime = 999999999;
        index = -1;
        for(int i = 0; i < n; i++){
            if(isOver[i] == false && minArrivalTime >= ArrivalTime[i]){
                minArrivalTime = ArrivalTime[i];
                index = i;
            }
        }
        // 判断到达时间是否大于上个程序结束时间(),如果是，则延时
        if(ArrivalTime[index] > startTime)
            startTime = ArrivalTime[index];
        // 输出运行状态
        cout<<"时刻"<<startTime<<"，进程"<<index+1<<"开始运行"<<endl;
        // 修改访问标识
        isOver[index] = true;
        // 设置完成时间
        FinishTime[index] = startTime + ServiceTime[index];
        // 设置周转时间
        WholeTime[index] = FinishTime[index] - ArrivalTime[index];
        // 设置带权周转时间
        WeightWholeTime[index] = (double) WholeTime[index] / ServiceTime[index];
        // 设置下一个进程的开始执行时间
        startTime = FinishTime[index];
        // 计算周转时间和带权周转时间总和
        sumWholeTime += WholeTime[index];
        sumWeightWholeTime += WeightWholeTime[index];
    }
    // 结果输出
    for(int i = 0; i < n; i++){
        cout<<"进程"<<i+1<<"的完成时间为"<<FinishTime[i]<<"， 周转时间为"<<WholeTime[i]<<"， 带权周转时间为"<<WeightWholeTime[i]<<""<<endl;
    }
    AverageWT_FCFS = (double)sumWholeTime / n;
    AverageWWT_FCFS = (double)sumWeightWholeTime / n;
    // 输出FCFS的平均周转时间
    cout<<"FCFS的平均周转时间为"<<AverageWT_FCFS<<endl;
    // 输出FCFS的平均带权周转时间
    cout<<"FCFS的平均带权周转时间为"<<AverageWWT_FCFS<<endl;

    // ----------------------SJF算法----------------------
    cout<<"----------------------SJF算法----------------------"<<endl;
    // 重置进程是否已经被执行布尔标识 及 缓存的值
    for(int i = 0; i < n; i++){
        isOver[i] = false;
    }
    startTime = 0;
    sumWholeTime = 0;
    sumWeightWholeTime = 0;
    index = -1;
    minArrivalTime = 999999999;
    int minServiceTime = 999999999;

    // 当进程没有被全部执行
    while(!isOverFun(isOver, sizeof(isOver))){
        // 获取剩下没有执行进程中 服务时间最短的进程
        index = -1;
        minServiceTime = 999999999;
        for(int i = 0; i < n; i++){
            // 条件：到达时间在上个进程执行完时间之前 还未执行 服务时间最小
            if(ArrivalTime[i] < startTime && isOver[i] == false && minServiceTime > ServiceTime[i]){
                minServiceTime = ServiceTime[i];
                index = i;
            }
        }
        // 如果index=-1，表示剩下待执行进程都在上个进程执行完时间之后,获取最先到达的进程
        if(index == -1){
            minArrivalTime = 999999999;
            for(int i = 0; i < n; i++){
                // 条件：还未执行 到达时间最小
                if(isOver[i] == false && minArrivalTime >= ArrivalTime[i]){
                    // 如果同时到达
                    if(minArrivalTime == ArrivalTime[i]){
                        // 根据服务时间决定使用哪一个
                        // 如果i的进程服务时间小，就用i，反之用index
                        if(ServiceTime[index]>ServiceTime[i]){
                            minServiceTime = ServiceTime[i];
                            index = i;
                        }
                    }else{
                        minArrivalTime = ArrivalTime[i];
                        index = i;
                    }
                }
            }
            // 设置时间为到达时间
            startTime = ArrivalTime[index];
        }

        // 输出运行状态
        cout<<"时刻"<<startTime<<"，进程"<<index+1<<"开始运行"<<endl;
        // 修改访问标识
        isOver[index] = true;
        // 设置完成时间
        FinishTime[index] = startTime + ServiceTime[index];
        // 设置周转时间
        WholeTime[index] = FinishTime[index] - ArrivalTime[index];
        // 设置带权周转时间
        WeightWholeTime[index] = (double) WholeTime[index] / ServiceTime[index];
        // 设置下一个进程的开始执行时间
        startTime = FinishTime[index];
        // 计算周转时间和带权周转时间总和
        sumWholeTime += WholeTime[index];
        sumWeightWholeTime += WeightWholeTime[index];
    }

    // 结果输出
    for(int i = 0; i < n; i++){
        cout<<"进程"<<i+1<<"的完成时间为"<<FinishTime[i]<<"， 周转时间为"<<WholeTime[i]<<"， 带权周转时间为"<<WeightWholeTime[i]<<""<<endl;
    }
    AverageWT_SJF = (double)sumWholeTime / n;
    AverageWWT_SJF = (double)sumWeightWholeTime / n;
    // 输出SJF的平均周转时间
    cout<<"SJF的平均周转时间为"<<AverageWT_SJF<<endl;
    // 输出SJF的平均带权周转时间
    cout<<"SJF的平均带权周转时间为"<<AverageWWT_SJF<<endl;

    return 0;
}
