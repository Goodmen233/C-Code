#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

#define MaxNumber 100
int TrackOrder[MaxNumber];              // �ŵ���������
int MoveDistance[MaxNumber];            // �ƶ�����
double AverageDistance;                 // ƽ��Ѱ������
bool direction;                         // ���� 1��ʾ���ﵽ�� 0��֮
bool isVisited[MaxNumber];              // �洢�Ƿ����
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
    int cur = StartDisk;// �洢��ǰ���ڵĴŵ���
    // ������ĸ��ŵ���ʼ
    cout<<"��"<<StartDisk<<"�Ŵŵ���ʼ"<<endl;
    // �������дŵ���������ʵ�ֵ���ƶ����롢���޸ĵ�ǰ�Ĵŵ���,�洢���ƶ�����
    for(int i = 0; i < DiskNum; i++){
        MoveDistance[i] = abs(cur-TrackOrder[i]);
        cout<<"�����ʵ���һ���ŵ��ţ�"<<TrackOrder[i]<<"\t�ƶ����룺"<<MoveDistance[i]<<""<<endl;
        cur = TrackOrder[i];
        AverageDistance += MoveDistance[i];
    }
    // ����ƽ��Ѱ������
    cout<<"FIFO��ƽ��Ѱ������Ϊ��"<<AverageDistance / DiskNum<<""<<endl;
}

void SSTF()
{
    int cur = StartDisk;// �洢��ǰ���ڵĴŵ���
    // ������ĸ��ŵ���ʼ
    cout<<"��"<<StartDisk<<"�Ŵŵ���ʼ"<<endl;
    // ��Ҫѭ������=�ŵ�����(��������)
    for(int i = 0; i < DiskNum; i++){
        // ���μ���ÿ��δ�����ʵĴŵ���Ҫ�ƶ��ľ���
        for(int j = 0; j < DiskNum; j++){
            if(isVisited[j] == false){
                MoveDistance[j] = abs(cur-TrackOrder[j]);
            }
        }
        // �ҳ���С�ƶ�����Ĵŵ������ƶ�(������ʵ�ֵ���ƶ����롢���޸ĵ�ǰ�Ĵŵ���,�洢���ƶ�����)
        int minDistance = 999999;
        int minIndex = -1;
        for(int k = 0 ; k < DiskNum; k++){
            if(isVisited[k] == false && minDistance > MoveDistance[k]){
                minDistance = MoveDistance[k];
                minIndex = k;
            }
        }
        cout<<"�����ʵ���һ���ŵ��ţ�"<<TrackOrder[minIndex]<<"\t�ƶ����룺"<<minDistance<<""<<endl;
        cur = TrackOrder[minIndex];
        isVisited[minIndex] = true;
        AverageDistance += minDistance;
    }
    // ����ƽ��Ѱ������
    cout<<"SSTF��ƽ��Ѱ������Ϊ��"<<AverageDistance / DiskNum<<""<<endl;
}

void SCAN()
{
    int temp;
    // �ȸ��ŵ���������
    for(int i = 0; i < DiskNum; i++){
        for(int j = 0; j < DiskNum - 1 - i; j++){
            if(TrackOrder[j] > TrackOrder[j + 1]){
                temp = TrackOrder[j];
                TrackOrder[j] = TrackOrder[j + 1];
                TrackOrder[j + 1] = temp;
            }
        }
    }
    int cur = StartDisk;// �洢��ǰ���ڵĴŵ���
    int index = -1;// �洢С�ڿ�ʼ�ŵ����±�
    // ������ĸ��ŵ���ʼ
    cout<<"��"<<StartDisk<<"�Ŵŵ���ʼ,";
    // ������ĸ�����
    if(direction){
        cout<<"���ﵽ��"<<endl;
    }else{
        cout<<"���⵽��"<<endl;
    }
    // �ҵ�С�ڿ�ʼ�ŵ����±겢�洢
    for(int i = 0; i < DiskNum; i++){
        if(TrackOrder[i] <= StartDisk){
            index = i;
            continue;
        }
        break;
    }
    // һ����Ҫ���α���
    for(int i = 0; i < 2; i++){
        // ���ﵽ��
        if(direction){
            // �������ڵ�ǰ�Ĵŵ��ţ�������ʵ�ֵ���ƶ����롢���޸ĵ�ǰ�Ĵŵ���,�洢���ƶ�����
            for(int j = index + 1; j < DiskNum; j++){
                MoveDistance[j] = abs(cur-TrackOrder[j]);
                cout<<"�����ʵ���һ���ŵ��ţ�"<<TrackOrder[j]<<"\t�ƶ����룺"<<MoveDistance[j]<<""<<endl;
                cur = TrackOrder[j];
                AverageDistance += MoveDistance[j];
            }
        // ���⵽��
        }else{
            // ��С�ڿ�ʼ�ŵ����±������������ʵ�ֵ���ƶ����롢���޸ĵ�ǰ�Ĵŵ���,�洢���ƶ�����
            for(int j = index; j >= 0; j--){
                MoveDistance[j] = abs(cur-TrackOrder[j]);
                cout<<"�����ʵ���һ���ŵ��ţ�"<<TrackOrder[j]<<"\t�ƶ����룺"<<MoveDistance[j]<<""<<endl;
                cur = TrackOrder[j];
                AverageDistance += MoveDistance[j];
            }
        }
        // �޸ķ���
        direction = !direction;
    }
    // ����ƽ��Ѱ������
    cout<<"SCAN��ƽ��Ѱ������Ϊ��"<<AverageDistance / DiskNum<<""<<endl;
}

void CSCAN()
{
    int temp;
    // �ȸ��ŵ���������
    for(int i = 0; i < DiskNum; i++){
        for(int j = 0; j < DiskNum - 1 - i; j++){
            if(TrackOrder[j] > TrackOrder[j + 1]){
                temp = TrackOrder[j];
                TrackOrder[j] = TrackOrder[j + 1];
                TrackOrder[j + 1] = temp;
            }
        }
    }
    int cur = StartDisk;// �洢��ǰ���ڵĴŵ���
    int index = -1;// �洢С�ڿ�ʼ�ŵ����±�
    // ������ĸ��ŵ���ʼ
    cout<<"��"<<StartDisk<<"�Ŵŵ���ʼ";
    // ������ĸ�����
    if(direction){
        cout<<"���ﵽ��"<<endl;
    }else{
        cout<<"���⵽��"<<endl;
    }
    // �ҵ�С�ڿ�ʼ�ŵ����±겢�洢
    for(int i = 0; i < DiskNum; i++){
        if(TrackOrder[i] <= StartDisk){
            index = i;
            continue;
        }
        break;
    }
    // ���ﵽ��
    if(direction){
        // һ����Ҫ���α���
        for(int i = 0; i < 2; i++){
            if(i == 0){
                // �������ڵ�ǰ�Ĵŵ��ţ�������ʵ�ֵ���ƶ����롢���޸ĵ�ǰ�Ĵŵ���,�洢���ƶ�����
                for(int j = index + 1; j < DiskNum; j++){
                    MoveDistance[j] = abs(cur-TrackOrder[j]);
                    cout<<"�����ʵ���һ���ŵ��ţ�"<<TrackOrder[j]<<"\t�ƶ����룺"<<MoveDistance[j]<<""<<endl;
                    cur = TrackOrder[j];
                    AverageDistance += MoveDistance[j];
                }
            }else{
                for(int j = 0; j <= index; j++){
                    MoveDistance[j] = abs(cur-TrackOrder[j]);
                    cout<<"�����ʵ���һ���ŵ��ţ�"<<TrackOrder[j]<<"\t�ƶ����룺"<<MoveDistance[j]<<""<<endl;
                    cur = TrackOrder[j];
                    AverageDistance += MoveDistance[j];
                }
            }
        }
    // ���⵽��
    }else{
        // һ����Ҫ���α���
        for(int i = 0; i < 2; i++){
            if(i == 0){
                // �������ڵ�ǰ�Ĵŵ��ţ�������ʵ�ֵ���ƶ����롢���޸ĵ�ǰ�Ĵŵ���,�洢���ƶ�����
                for(int j = index; j >= 0; j--){
                    MoveDistance[j] = abs(cur-TrackOrder[j]);
                    cout<<"�����ʵ���һ���ŵ��ţ�"<<TrackOrder[j]<<"\t�ƶ����룺"<<MoveDistance[j]<<""<<endl;
                    cur = TrackOrder[j];
                    AverageDistance += MoveDistance[j];
                }
            }else{
                for(int j = DiskNum - 1; j > index; j--){
                    MoveDistance[j] = abs(cur-TrackOrder[j]);
                    cout<<"�����ʵ���һ���ŵ��ţ�"<<TrackOrder[j]<<"\t�ƶ����룺"<<MoveDistance[j]<<""<<endl;
                    cur = TrackOrder[j];
                    AverageDistance += MoveDistance[j];
                }
            }
        }
    }

    // ����ƽ��Ѱ������
    cout<<"SCAN��ƽ��Ѱ������Ϊ��"<<AverageDistance / DiskNum<<""<<endl;
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
