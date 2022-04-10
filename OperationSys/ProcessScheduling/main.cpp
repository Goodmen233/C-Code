#include <iostream>

using namespace std;

static int MaxNum = 100;// ���������

// �жϽ����Ƿ�ȫ����ִ����
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
    // ----------------------ʵ���������
    int ArrivalTime[MaxNum];// ÿ�����̵���ʱ��
    int ServiceTime[MaxNum];// ÿ�����̷���ʱ��
    int FinishTime[MaxNum];// ÿ�����̵����ʱ��                           ��ʼ����ʱ��+����ʱ��
    int WholeTime[MaxNum];// ÿ�����̵���תʱ��                            ���ʱ��-����ʱ��
    double WeightWholeTime[MaxNum];// ÿ�����̵Ĵ�Ȩ��תʱ��               ��תʱ��/����ʱ��
    double AverageWT_FCFS, AverageWT_SJF;// �����㷨��ƽ����תʱ��         ������תʱ��֮��/n
    double AverageWWT_FCFS, AverageWWT_SJF;// �����㷨�Ĵ�Ȩƽ����תʱ��   ���д�Ȩ��תʱ��֮��/n

    // ----------------------�û�����----------------------
    // ������̸���n
    int n = -1;
    while(n > 100 || n < 0){
        cout<<"��������̸�����";
        cin>>n;
        if(n > 100 || n < 0)
            cout<<"���벻�Ϸ�������������"<<endl;
    }

    // ����ÿ�����̶�Ӧ�ĵ���ʱ��ͷ���ʱ��
    for(int i = 0; i < n; i++){
        cout<<"�������"<<i + 1<<"�����̵ĵ���ʱ�估����ʱ�䣺";
        cin>>ArrivalTime[i]>>ServiceTime[i];
    }

    // �����һЩ�������
    int startTime = 0;// ����ÿ�����̿�ʼִ��ʱ��ε�ʱ��ڵ�
    double sumWholeTime = 0;// �����ܵ�����ʱ��
    double sumWeightWholeTime = 0;// �����ܵĴ�Ȩ��תʱ��

    // ----------------------FCFS�㷨----------------------
    cout<<"----------------------FCFS�㷨----------------------"<<endl;
    // ���ý����Ƿ��Ѿ���ִ�в�����ʶ
    bool isOver[n];
    for(int i = 0; i < n; i++){
        isOver[i] = false;
    }
    int minArrivalTime = 999999999;// ���浱ǰ��С����ʱ��
    int index = -1;// �����±�
    // ������û�б�ȫ��ִ��
    while(!isOverFun(isOver, sizeof(isOver))){
        // ��ȡʣ��û��ִ�н����� ����ʱ������Ľ���
        minArrivalTime = 999999999;
        index = -1;
        for(int i = 0; i < n; i++){
            if(isOver[i] == false && minArrivalTime >= ArrivalTime[i]){
                minArrivalTime = ArrivalTime[i];
                index = i;
            }
        }
        // �жϵ���ʱ���Ƿ�����ϸ��������ʱ��(),����ǣ�����ʱ
        if(ArrivalTime[index] > startTime)
            startTime = ArrivalTime[index];
        // �������״̬
        cout<<"ʱ��"<<startTime<<"������"<<index+1<<"��ʼ����"<<endl;
        // �޸ķ��ʱ�ʶ
        isOver[index] = true;
        // �������ʱ��
        FinishTime[index] = startTime + ServiceTime[index];
        // ������תʱ��
        WholeTime[index] = FinishTime[index] - ArrivalTime[index];
        // ���ô�Ȩ��תʱ��
        WeightWholeTime[index] = (double) WholeTime[index] / ServiceTime[index];
        // ������һ�����̵Ŀ�ʼִ��ʱ��
        startTime = FinishTime[index];
        // ������תʱ��ʹ�Ȩ��תʱ���ܺ�
        sumWholeTime += WholeTime[index];
        sumWeightWholeTime += WeightWholeTime[index];
    }
    // ������
    for(int i = 0; i < n; i++){
        cout<<"����"<<i+1<<"�����ʱ��Ϊ"<<FinishTime[i]<<"�� ��תʱ��Ϊ"<<WholeTime[i]<<"�� ��Ȩ��תʱ��Ϊ"<<WeightWholeTime[i]<<""<<endl;
    }
    AverageWT_FCFS = (double)sumWholeTime / n;
    AverageWWT_FCFS = (double)sumWeightWholeTime / n;
    // ���FCFS��ƽ����תʱ��
    cout<<"FCFS��ƽ����תʱ��Ϊ"<<AverageWT_FCFS<<endl;
    // ���FCFS��ƽ����Ȩ��תʱ��
    cout<<"FCFS��ƽ����Ȩ��תʱ��Ϊ"<<AverageWWT_FCFS<<endl;

    // ----------------------SJF�㷨----------------------
    cout<<"----------------------SJF�㷨----------------------"<<endl;
    // ���ý����Ƿ��Ѿ���ִ�в�����ʶ �� �����ֵ
    for(int i = 0; i < n; i++){
        isOver[i] = false;
    }
    startTime = 0;
    sumWholeTime = 0;
    sumWeightWholeTime = 0;
    index = -1;
    minArrivalTime = 999999999;
    int minServiceTime = 999999999;

    // ������û�б�ȫ��ִ��
    while(!isOverFun(isOver, sizeof(isOver))){
        // ��ȡʣ��û��ִ�н����� ����ʱ����̵Ľ���
        index = -1;
        minServiceTime = 999999999;
        for(int i = 0; i < n; i++){
            // ����������ʱ�����ϸ�����ִ����ʱ��֮ǰ ��δִ�� ����ʱ����С
            if(ArrivalTime[i] < startTime && isOver[i] == false && minServiceTime > ServiceTime[i]){
                minServiceTime = ServiceTime[i];
                index = i;
            }
        }
        // ���index=-1����ʾʣ�´�ִ�н��̶����ϸ�����ִ����ʱ��֮��,��ȡ���ȵ���Ľ���
        if(index == -1){
            minArrivalTime = 999999999;
            for(int i = 0; i < n; i++){
                // ��������δִ�� ����ʱ����С
                if(isOver[i] == false && minArrivalTime >= ArrivalTime[i]){
                    // ���ͬʱ����
                    if(minArrivalTime == ArrivalTime[i]){
                        // ���ݷ���ʱ�����ʹ����һ��
                        // ���i�Ľ��̷���ʱ��С������i����֮��index
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
            // ����ʱ��Ϊ����ʱ��
            startTime = ArrivalTime[index];
        }

        // �������״̬
        cout<<"ʱ��"<<startTime<<"������"<<index+1<<"��ʼ����"<<endl;
        // �޸ķ��ʱ�ʶ
        isOver[index] = true;
        // �������ʱ��
        FinishTime[index] = startTime + ServiceTime[index];
        // ������תʱ��
        WholeTime[index] = FinishTime[index] - ArrivalTime[index];
        // ���ô�Ȩ��תʱ��
        WeightWholeTime[index] = (double) WholeTime[index] / ServiceTime[index];
        // ������һ�����̵Ŀ�ʼִ��ʱ��
        startTime = FinishTime[index];
        // ������תʱ��ʹ�Ȩ��תʱ���ܺ�
        sumWholeTime += WholeTime[index];
        sumWeightWholeTime += WeightWholeTime[index];
    }

    // ������
    for(int i = 0; i < n; i++){
        cout<<"����"<<i+1<<"�����ʱ��Ϊ"<<FinishTime[i]<<"�� ��תʱ��Ϊ"<<WholeTime[i]<<"�� ��Ȩ��תʱ��Ϊ"<<WeightWholeTime[i]<<""<<endl;
    }
    AverageWT_SJF = (double)sumWholeTime / n;
    AverageWWT_SJF = (double)sumWeightWholeTime / n;
    // ���SJF��ƽ����תʱ��
    cout<<"SJF��ƽ����תʱ��Ϊ"<<AverageWT_SJF<<endl;
    // ���SJF��ƽ����Ȩ��תʱ��
    cout<<"SJF��ƽ����Ȩ��תʱ��Ϊ"<<AverageWWT_SJF<<endl;

    return 0;
}
