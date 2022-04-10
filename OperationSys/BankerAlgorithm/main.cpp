#include <iostream>
#include <fstream>

using namespace std;

#define MaxNumber 100
// 1:1 0 2   4:3 3 0   0:0 2 0
int main()
{
    int Available[MaxNumber];// 可用资源量
    int Max[MaxNumber][MaxNumber];// 最大需求量
    int Allocation[MaxNumber][MaxNumber];// 已经分配量
    int Need[MaxNumber][MaxNumber];// 当前需要的量
    int Request[MaxNumber];// 请求的资源量
    int SafeOrder[MaxNumber];// 安全序列

    ifstream input("data.txt");
    // n为进程个数，m为资源个数
    int n, m;
    input>>n;
    input>>m;
    // 设置最大需求量
    for(int i = 0; i < n; i++){
        for(int j =0; j < m; j++){
            input>>Max[i][j];
        }
    }
    // 设置已经分配量
    for(int i = 0; i < n; i++){
        for(int j =0; j < m; j++){
            input>>Allocation[i][j];
        }
    }
    // 设置可用资源量
    for(int j = 0; j < m; j++){
        input>>Available[j];
    }
    // 设置当前需要的量
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
    }

    int RequestI = 0;
    // 设置是否第一次访问资源
    bool isFirst = 1;
    while(1){
        if(isFirst){
            // 输入请求向量,一开始默认没有
            RequestI = 0;
            for(int j = 0; j < m; j++){
                Request[j] = 0;
            }
            isFirst = 0;
        }else{
            // 让用户继续输入运行，如果输入1，则终止运行
            cout<<"是否终止运行(Y/N):";
            char res;
            cin>>res;
            if(res == 'N' || res == 'n'){
                cout<<"请输入第几个进程（0-n）请求资源：";
                cin>>RequestI;
                cout<<"请输入请求的资源数:";
                for(int i = 0; i < m; i++){
                    cin>>Request[i];
                }
            }else if(res == 'Y' || res == 'y'){
                break;
            }else{
                cout<<"输入不合法，请重新输入"<<endl;
                continue;
            }
        }

        // 如果Request<=Need
        bool isGood = 1;
        for(int i = 0; i < m; i++){
            if(Request[i] > Need[RequestI][i]){
                isGood = 0;
                break;
            }
        }
        if(isGood){
            // 如果Request<=Available
            for(int i = 0; i < m; i++){
                if(Request[i] > Available[i]){
                    isGood = 0;
                    break;
                }
            }
            if(isGood){
                // 将资源 试探 分配给请求进程，修改三项值
                for(int i = 0; i < m; i++){
                    Available[i] -= Request[i];
                    Allocation[RequestI][i] += Request[i];
                    Need[RequestI][i] -= Request[i];
                }
                // 进行安全性检测，成功输出安全序列；反之恢复之前的资源并且提示不安全
                // 设置工作向量
                int Work[m];
                for(int i = 0; i < m; i++){
                    Work[i] = Available[i];
                }
                // 设置完成向量
                int Finish[n];
                for(int i = 0; i < n; i++){
                    Finish[i] = 0;
                }
                // 遍历判断 Finish=false && Need<=Work 的进程
                bool isRun = 1;// 判断是否结束，如果遍历一遍值还是为0，则就是结束
                int temp = 0;// 存放安全序列的下标记录
                while(isRun){
                    isRun = 0;// 如果遍历一遍值还是为0，则就是没有可运行的进程
                    for(int i = 0; i < n; i++){
                        isGood = 1;
                        // 如果有则表示可执行完并释放资源
                        if(Finish[i] == 0){
                            for(int j = 0; j < m; j++){
                                if(Need[i][j] <= Work[j]){
                                    continue;
                                }else{
                                    isGood = 0;
                                    break;
                                }
                            }
                            if(isGood){
                                isRun = 1;
                                for(int j = 0; j < m; j++){
                                    Work[j] += Allocation[i][j];
                                }
                                Finish[i] = 1;
                                SafeOrder[temp++] = i;// 存储安全序列
                            }
                        }


                    }
                    if(!isRun) break;// 如果没有进程继续执行表示结束
                }
                isGood = 1;
                // 判断所有的进程是否都是Finish，是则安全
                for(int i = 0; i < n; i++){
                    // 反之不安全，恢复之前的资源并提示
                    if(Finish[i] == 0){
                        isGood = 0;
                        break;
                    }
                }
                if(isGood){
                    cout<<"安全，安全序列为：";
                    for(int i = 0; i < n; i++){
                        cout<<SafeOrder[i]<<" ";
                    }
                    cout<<endl;
                }else{
                    cout<<"不安全！"<<endl;
                    // 还原之前的值
                    for(int i = 0; i < m; i++){
                        Available[i] += Request[i];
                        Allocation[RequestI][i] -= Request[i];
                        Need[RequestI][i] += Request[i];
                    }
                    break;// 结束
                }
            }else{
                // 反之尚无足够资源，等待！
                cout<<"尚无足够资源，请稍后再试"<<endl;
            }
        }else{
            // 反之如果Request>Need,提示请求资源大于进程所需资源
            cout<<"请求资源失败，因为需要的资源已超过它宣布的最大值"<<endl;
        }
    }
    return 0;
}
