#include <iostream>
#include <fstream>
// https://www.docin.com/p-760510162.html
using namespace std;

#define MaxNumber 100
int  PageOrder[MaxNumber];              // 页面序列
int  Simulate[MaxNumber][MaxNumber];    // 模拟过程
int  PageCount[MaxNumber];              // 存储每个页面按需求第几次出现
int  PageNum,LackNum,BlockNum;          // 页面个数及缺页次数及块数
double  LackPageRate;                   // 缺页率
bool found;


void init()
{
    ifstream input("data.txt");
    input>>BlockNum>>PageNum;
    LackNum = 0;
    LackPageRate = 0.00;
    for(int i = 0; i < PageNum; i++){
        input>>PageOrder[i];
        PageCount[i] = 0;
    }
    for(int i = 0; i < BlockNum; i++){
        for(int j = 0; j < PageNum; j++){
            Simulate[i][j] = -1;
        }
    }
    found = false;

    cout<<"当前页面序列为："<<endl;
    for(int i = 0; i < PageNum; i++){
        cout<<PageOrder[i]<<" ";
    }
    cout<<endl;
}

void printSimulate()
{
    for(int i = 0; i < BlockNum; i++){
        for(int j = 0; j < PageNum; j++){
            if(Simulate[i][j] != -1)
                cout<<Simulate[i][j]<<" ";
            else
                cout<<" "<<" ";
        }
        cout<<endl;
    }
}

// 使指定列的值为null
void columnNull(int i)
{
    for(int j = 0; j < BlockNum; j++){
        Simulate[j][i] = -1;
    }
}

// 先进先出页面置换
void FIFO()
{
    int tempSimulate[BlockNum];// 缓存当前块的情况
    // 先让第一个到的进入
    for(int i = 0; i < BlockNum; i++){
        if(i == 0){
            tempSimulate[i] = PageOrder[0];
            Simulate[0][i] = PageOrder[0];
        }else{
            tempSimulate[i] = -1;
            Simulate[0][i] = -1;
        }
    }
    LackNum++;
    int fi = 1;// 缓存先进的页面
    // 遍历所有页面序列
    for(int i = 1; i < PageNum; i++){
        found = false;
        // 判断是否在序列中
        for(int j = 0; j < BlockNum; j++){
            if(PageOrder[i] == tempSimulate[j]){
                found = true;
                break;
            }
        }
        if(found){
            // 是则该列的模拟都为null
            columnNull(i);
            continue;
        }
        // 否则替换上次先到的页面,缺页数加一,fi指向下一个先到的页面
        tempSimulate[fi] = PageOrder[i];
        if(++fi == BlockNum) fi = 0;
        LackNum++;
        // 把结果复制过来
        for(int j = 0; j < BlockNum; j++){
            Simulate[j][i] = tempSimulate[j];
        }
    }
    LackPageRate = (double) LackNum / PageNum * 100;
}

void OPI()
{
    int tempSimulate[BlockNum];// 缓存当前块的情况
    // 先让第一个到的进入
    for(int i = 0; i < BlockNum; i++){
        if(i == 0){
            tempSimulate[i] = PageOrder[0];
            Simulate[0][i] = PageOrder[0];
        }else{
            tempSimulate[i] = -1;
            Simulate[0][i] = -1;
        }
    }
    LackNum++;
    // 遍历所有页面序列
    for(int i = 1; i < PageNum; i++){
        found = false;
        // 判断是否在序列中
        for(int j = 0; j < BlockNum; j++){
            if(PageOrder[i] == tempSimulate[j]){
                found = true;
                break;
            }
        }
        if(found){
            // 是则该列的模拟都为null
            columnNull(i);
            continue;
        }
        // 否则判断是否有空余位置
        found = false;
        for(int x = 0; x < BlockNum; x++){
            if(tempSimulate[x] == -1){
                tempSimulate[x] = PageOrder[i];
                found = true;
                break;
            }
        }
        // 否则找到最佳置换的页面
        // 重置PageCount
        for(int x = 0; x < PageNum; x++){
            PageCount[x] = 0;
        }
        if(!found){
            for(int x = 0; x < BlockNum; x++){
                for(int k = i + 1; k < PageNum; k++){
                    // 如果不相等则加一
                    if(PageOrder[k] != tempSimulate[x]){
                        PageCount[x]++;
                    }else{
                        PageCount[x]++;
                        break;
                    }
                }
            }
            int maxIndex = 0;
            int maxValue = PageCount[maxIndex];
            // 判断哪个是最晚使用
            for(int x = 0; x < BlockNum; x++){
                if(PageCount[x] > maxValue){
                    maxIndex = x;
                    maxValue = PageCount[x];
                }
            }
            tempSimulate[maxIndex] = PageOrder[i];
        }
        // 缺页数加一并复制结果到模拟数组
        LackNum++;
        // 把结果复制过来
        for(int j = 0; j < BlockNum; j++){
            Simulate[j][i] = tempSimulate[j];
        }
    }
    LackPageRate = (double) LackNum / PageNum * 100;
}

void LRU()
{
    int tempSimulate[BlockNum];// 缓存当前块的情况
    // 先让第一个到的进入
    for(int i = 0; i < BlockNum; i++){
        if(i == 0){
            tempSimulate[i] = PageOrder[0];
            Simulate[0][i] = PageOrder[0];
        }else{
            tempSimulate[i] = -1;
            Simulate[0][i] = -1;
        }
    }
    LackNum++;
    // 遍历所有页面序列
    for(int i = 1; i < PageNum; i++){
        found = false;
        // 判断是否在序列中
        for(int j = 0; j < BlockNum; j++){
            if(PageOrder[i] == tempSimulate[j]){
                found = true;
                break;
            }
        }
        if(found){
            // 是则该列的模拟都为null
            columnNull(i);
            continue;
        }
        // 否则判断是否有空余位置
        found = false;
        for(int x = 0; x < BlockNum; x++){
            if(tempSimulate[x] == -1){
                tempSimulate[x] = PageOrder[i];
                found = true;
                break;
            }
        }
        // 否则找到最近最久未使用的页面
        // 重置PageCount
        for(int x = 0; x < PageNum; x++){
            PageCount[x] = 0;
        }
        if(!found){
            for(int x = 0; x < BlockNum; x++){
                for(int k = i - 1; k >= 0; k--){
                    // 如果不相等则加一
                    if(PageOrder[k] != tempSimulate[x]){
                        PageCount[x]++;
                    }else{
                        PageCount[x]++;
                        break;
                    }
                }
            }
            int maxIndex = 0;
            int maxValue = PageCount[maxIndex];
            // 判断哪个最近最久未使用
            for(int x = 0; x < BlockNum; x++){
                if(PageCount[x] > maxValue){
                    maxIndex = x;
                    maxValue = PageCount[x];
                }
            }
            tempSimulate[maxIndex] = PageOrder[i];
        }
        // 缺页数加一并复制结果到模拟数组
        LackNum++;
        // 把结果复制过来
        for(int j = 0; j < BlockNum; j++){
            Simulate[j][i] = tempSimulate[j];
        }
    }
    LackPageRate = (double) LackNum / PageNum * 100;

}

int main()
{
    init();
    FIFO();
    cout<<"---------------------FIFO的结果为：---------------------"<<endl;
    printSimulate();
    cout<<"缺页数为："<<LackNum<<" "<<"缺页率为："<<LackPageRate<<"%"<<endl<<endl;

    init();
    OPI();
    cout<<"---------------------OPI的结果为：---------------------"<<endl;
    printSimulate();
    cout<<"缺页数为："<<LackNum<<" "<<"缺页率为："<<LackPageRate<<"%"<<endl<<endl;

    init();
    LRU();
    cout<<"---------------------LRU的结果为：---------------------"<<endl;
    printSimulate();
    cout<<"缺页数为："<<LackNum<<" "<<"缺页率为："<<LackPageRate<<"%"<<endl<<endl;

    return 0;
}
