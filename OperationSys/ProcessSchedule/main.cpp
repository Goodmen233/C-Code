#include <iostream>
#include <queue>

using namespace std;

static int MaxNum = 100;// 进程最大数

// 判断进程是否全部被执行完
bool isOverFun(bool isOver[], int len)
{
    for(int i = 0; i < len; i++){
        if(isOver[i] == false)
            return false;
    }
    return true;
}

// ----------------------------------------------该程序只适用于到达时间依次排序的数据---------------------------------------------------------------------------
int main()
{
    // ----------------------实验所需变量
    int ArrivalTime[MaxNum];// 每个进程到达时间
    int ServiceTime[MaxNum];// 每个进程服务时间
    int PServiceTime[MaxNum];// 存取每个进程重新进入就绪状态之后所需要的服务时间
    int FinishTime[MaxNum];// 每个进程的完成时间                                            开始运行时间+服务时间
    int WholeTime[MaxNum];// 每个进程的周转时间                                             完成时间-到达时间
    double WeightWholeTime[MaxNum];// 每个进程的带权周转时间                                周转时间/服务时间
    double AverageWT = 0;
    double AverageWWT = 0;// 算法的平均周转时间和带权周转时间
    bool Finished[MaxNum];// 设置进程是否已经被执行布尔标识
    bool isArrival[MaxNum];// 设置到达就绪状态

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
        PServiceTime[i] = ServiceTime[i];
    }

    // 初始化访问标记
    for(int i = 0; i < n; i++){
        Finished[i] = false;
        isArrival[i] = false;
    }

    // 输入时间片
    int q = -1;
    cout<<"输入时间片的大小：";
    cin>>q;

    // 计算的一些缓存变量
    int startTime = 0;// 缓存每个进程开始执行时间段的时间节点,也就是上个进程的结束时间
    queue<int> queueP;// 缓存进程的执行顺序

    // ---------------------RR调度算法----------------------
    cout<<"----------------------RR调度算法----------------------"<<endl;

    // 先让最先到达的进程进入队列
    int index = 0;
    for(int i = 1; i < n; i++){
        if(Finished[i] == false && ArrivalTime[i] < ArrivalTime[index]){
            index = i;
        }
    }
    queueP.push(index);
    isArrival[index] = true;

    // 当进程没有被全部执行
    while(!isOverFun(Finished, n)){
        // 拿出队列的首元素，判断服务时间和时间片的大小
        int index = queueP.front();
        queueP.pop();
        bool isOver = true;// 判断当前进程是否执行完的标识
        // 如果PServiceTime<=q，则该进程运行结束，进行计算，并加载之后到达的进程入队列
        if(PServiceTime[index] <= q){
            // 输出运行状态
            cout<<"时刻"<<startTime<<"，进程"<<index+1<<"开始运行"<<endl;
            // 设置完成时间
            FinishTime[index] = startTime + PServiceTime[index];
            // 设置周转时间
            WholeTime[index] = FinishTime[index] - ArrivalTime[index];
            // 修改访问标识
            Finished[index] = true;
            // 修改剩下的服务时间
            PServiceTime[index] = 0;
            // 设置带权周转时间
            WeightWholeTime[index] = (double) WholeTime[index] / ServiceTime[index];
            // 设置下一个进程的开始执行时间
            startTime = FinishTime[index];
            // 计算周转时间和带权周转时间总和
            AverageWT += WholeTime[index];
            AverageWWT += WeightWholeTime[index];

        }
        // 如果PServiceTime>q，则该进程运行未结束，缓存剩下需要服务时间，加载之后到达的进程入队列，当前进程也入队列
        else{
            // 修改是否小于等于q的标识
            isOver = false;
            // 输出运行状态
            cout<<"时刻"<<startTime<<"，进程"<<index+1<<"开始运行"<<endl;
            PServiceTime[index] -= q;
            startTime += q;
        }
        // 判断哪些进程已经到达，进入队列
        bool exitArrival = false;
        for(int i = 0; i < n; i++){
            if(isArrival[i] == false && ArrivalTime[i] <= startTime){
                queueP.push(i);
                isArrival[i] = true;
                exitArrival = true;
            }
        }
        // 如果之后的进程都还未到达，选取最先到达的进程
        if(!exitArrival){
            int minArrivalTime = 99999999;
            int tempIndex = -1;
            for(int i = 0; i < n; i++){
                if(isArrival[i] == false && minArrivalTime > ArrivalTime[i]){
                    minArrivalTime = ArrivalTime[i];
                    tempIndex = i;
                }
            }
            // 如果还有未到达才装载
            if(tempIndex != -1){
                queueP.push(tempIndex);
                isArrival[tempIndex] = true;
                startTime = ArrivalTime[tempIndex];
            }

        }
        // 判断是否需要把当前运行进程装载入队列
        if(!isOver)
            queueP.push(index);// 把自己装入
    }


    // 结果输出
    for(int i = 0; i < n; i++){
        cout<<"进程"<<i+1<<"的完成时间为"<<FinishTime[i]<<"， 周转时间为"<<WholeTime[i]<<"， 带权周转时间为"<<WeightWholeTime[i]<<""<<endl;
    }
    // 计算平均周转时间和带权周转时间
    AverageWT = AverageWT / n;
    AverageWWT = AverageWWT / n;
    // 输出RR的平均周转时间
    cout<<"RR的平均周转时间为"<<AverageWT<<endl;
    // 输出RR的平均带权周转时间
    cout<<"RR的平均带权周转时间为"<<AverageWWT<<endl;

    return 0;
}
