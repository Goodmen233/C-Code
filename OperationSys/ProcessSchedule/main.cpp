#include <iostream>
#include <queue>

using namespace std;

static int MaxNum = 100;// ���������

// �жϽ����Ƿ�ȫ����ִ����
bool isOverFun(bool isOver[], int len)
{
    for(int i = 0; i < len; i++){
        if(isOver[i] == false)
            return false;
    }
    return true;
}

// ----------------------------------------------�ó���ֻ�����ڵ���ʱ���������������---------------------------------------------------------------------------
int main()
{
    // ----------------------ʵ���������
    int ArrivalTime[MaxNum];// ÿ�����̵���ʱ��
    int ServiceTime[MaxNum];// ÿ�����̷���ʱ��
    int PServiceTime[MaxNum];// ��ȡÿ���������½������״̬֮������Ҫ�ķ���ʱ��
    int FinishTime[MaxNum];// ÿ�����̵����ʱ��                                            ��ʼ����ʱ��+����ʱ��
    int WholeTime[MaxNum];// ÿ�����̵���תʱ��                                             ���ʱ��-����ʱ��
    double WeightWholeTime[MaxNum];// ÿ�����̵Ĵ�Ȩ��תʱ��                                ��תʱ��/����ʱ��
    double AverageWT = 0;
    double AverageWWT = 0;// �㷨��ƽ����תʱ��ʹ�Ȩ��תʱ��
    bool Finished[MaxNum];// ���ý����Ƿ��Ѿ���ִ�в�����ʶ
    bool isArrival[MaxNum];// ���õ������״̬

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
        PServiceTime[i] = ServiceTime[i];
    }

    // ��ʼ�����ʱ��
    for(int i = 0; i < n; i++){
        Finished[i] = false;
        isArrival[i] = false;
    }

    // ����ʱ��Ƭ
    int q = -1;
    cout<<"����ʱ��Ƭ�Ĵ�С��";
    cin>>q;

    // �����һЩ�������
    int startTime = 0;// ����ÿ�����̿�ʼִ��ʱ��ε�ʱ��ڵ�,Ҳ�����ϸ����̵Ľ���ʱ��
    queue<int> queueP;// ������̵�ִ��˳��

    // ---------------------RR�����㷨----------------------
    cout<<"----------------------RR�����㷨----------------------"<<endl;

    // �������ȵ���Ľ��̽������
    int index = 0;
    for(int i = 1; i < n; i++){
        if(Finished[i] == false && ArrivalTime[i] < ArrivalTime[index]){
            index = i;
        }
    }
    queueP.push(index);
    isArrival[index] = true;

    // ������û�б�ȫ��ִ��
    while(!isOverFun(Finished, n)){
        // �ó����е���Ԫ�أ��жϷ���ʱ���ʱ��Ƭ�Ĵ�С
        int index = queueP.front();
        queueP.pop();
        bool isOver = true;// �жϵ�ǰ�����Ƿ�ִ����ı�ʶ
        // ���PServiceTime<=q����ý������н��������м��㣬������֮�󵽴�Ľ��������
        if(PServiceTime[index] <= q){
            // �������״̬
            cout<<"ʱ��"<<startTime<<"������"<<index+1<<"��ʼ����"<<endl;
            // �������ʱ��
            FinishTime[index] = startTime + PServiceTime[index];
            // ������תʱ��
            WholeTime[index] = FinishTime[index] - ArrivalTime[index];
            // �޸ķ��ʱ�ʶ
            Finished[index] = true;
            // �޸�ʣ�µķ���ʱ��
            PServiceTime[index] = 0;
            // ���ô�Ȩ��תʱ��
            WeightWholeTime[index] = (double) WholeTime[index] / ServiceTime[index];
            // ������һ�����̵Ŀ�ʼִ��ʱ��
            startTime = FinishTime[index];
            // ������תʱ��ʹ�Ȩ��תʱ���ܺ�
            AverageWT += WholeTime[index];
            AverageWWT += WeightWholeTime[index];

        }
        // ���PServiceTime>q����ý�������δ����������ʣ����Ҫ����ʱ�䣬����֮�󵽴�Ľ�������У���ǰ����Ҳ�����
        else{
            // �޸��Ƿ�С�ڵ���q�ı�ʶ
            isOver = false;
            // �������״̬
            cout<<"ʱ��"<<startTime<<"������"<<index+1<<"��ʼ����"<<endl;
            PServiceTime[index] -= q;
            startTime += q;
        }
        // �ж���Щ�����Ѿ�����������
        bool exitArrival = false;
        for(int i = 0; i < n; i++){
            if(isArrival[i] == false && ArrivalTime[i] <= startTime){
                queueP.push(i);
                isArrival[i] = true;
                exitArrival = true;
            }
        }
        // ���֮��Ľ��̶���δ���ѡȡ���ȵ���Ľ���
        if(!exitArrival){
            int minArrivalTime = 99999999;
            int tempIndex = -1;
            for(int i = 0; i < n; i++){
                if(isArrival[i] == false && minArrivalTime > ArrivalTime[i]){
                    minArrivalTime = ArrivalTime[i];
                    tempIndex = i;
                }
            }
            // �������δ�����װ��
            if(tempIndex != -1){
                queueP.push(tempIndex);
                isArrival[tempIndex] = true;
                startTime = ArrivalTime[tempIndex];
            }

        }
        // �ж��Ƿ���Ҫ�ѵ�ǰ���н���װ�������
        if(!isOver)
            queueP.push(index);// ���Լ�װ��
    }


    // ������
    for(int i = 0; i < n; i++){
        cout<<"����"<<i+1<<"�����ʱ��Ϊ"<<FinishTime[i]<<"�� ��תʱ��Ϊ"<<WholeTime[i]<<"�� ��Ȩ��תʱ��Ϊ"<<WeightWholeTime[i]<<""<<endl;
    }
    // ����ƽ����תʱ��ʹ�Ȩ��תʱ��
    AverageWT = AverageWT / n;
    AverageWWT = AverageWWT / n;
    // ���RR��ƽ����תʱ��
    cout<<"RR��ƽ����תʱ��Ϊ"<<AverageWT<<endl;
    // ���RR��ƽ����Ȩ��תʱ��
    cout<<"RR��ƽ����Ȩ��תʱ��Ϊ"<<AverageWWT<<endl;

    return 0;
}
