#include <iostream>
#include <fstream>

using namespace std;

#define MaxNumber 100
int  FreePartition[MaxNumber];          // ���з�����С
int  FirstPartition[MaxNumber];         // FF�㷨
int  CycleFirstPartition[MaxNumber];    // NF�㷨
int  BestPartition[MaxNumber];          // BF�㷨
int  WorstPartition[MaxNumber];         // WF�㷨
int  ProcessNeed[MaxNumber];            // ������Ҫ�ķ�����С
int  PartitionNum,ProcessNum;           // ������������������

// ͨ�����±���������
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

// ����̨��������
void init()
{
    // ���������������������
    cout<<"�����������������������(������100)��";
    cin>>PartitionNum>>ProcessNum;
    // ���������Ҫ�ķ�����С
    cout<<"������ÿ��������Ҫ�ķ�����С��";
    for(int i = 0; i < ProcessNum; i++){
        cin>>ProcessNeed[i];
    }
    // ���������С����ʼ�������㷨�ĳ�ʼֵ
    cout<<"������ÿ��������С��";
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
    // ����ÿ������
    for(int i = 0; i < ProcessNum; i++){
        isIn = false;
        // �����������ҵ����Դ�ŵ�ǰ���̵ķ���
        for(int j = 0; j < PartitionNum; j++){
            // ����ҵ��������Ϣ���޸ķ��������С
            if(FirstPartition[j] >= ProcessNeed[i]){
                isIn = true;
                cout<<"����"<<ProcessNeed[i]<<"���䵽"<<FirstPartition[j]<<"������";
                FirstPartition[j] -= ProcessNeed[i];
                cout<<"��ǰ�������Ϊ:";
                for(int k = 0; k < PartitionNum; k++){
                    cout<<FirstPartition[k]<<" ";
                }
                cout<<endl;
                break;
            }
        }
        // ���û�ҵ��������ʾ
        if(!isIn)
            cout<<"����"<<ProcessNeed[i]<<"�޷����䵽������"<<endl;
    }
}

void NF()
{
    cout<<"---------------NF---------------------"<<endl;
    // ����һ��ָ��
    int cur = 0;
    bool isIn = false;
    // �������н���
    for(int i = 0; i < ProcessNum; i++){
        isIn = false;
        // �������з��������ϴη����λ�ÿ�ʼ���ҵ����Դ�ŵķ���
        int temp = PartitionNum;// ѭ������
        while(temp > 0){
            // ����ҵ��������Ϣ���޸ķ��������С,ָ�뻺�浱ǰ����λ��
            if(CycleFirstPartition[cur] >= ProcessNeed[i]){
                isIn = true;
                cout<<"����"<<ProcessNeed[i]<<"���䵽"<<CycleFirstPartition[cur]<<"������";
                CycleFirstPartition[cur] -= ProcessNeed[i];
                cout<<"��ǰ�������Ϊ:";
                for(int k = 0; k < PartitionNum; k++){
                    cout<<CycleFirstPartition[k]<<" ";
                }
                cout<<endl;
                break;
            }
            if(++cur == PartitionNum) cur = 0;// �ص��׸�
            temp--;
        }
        // ���û�ҵ��������ʾ
        if(!isIn)
            cout<<"����"<<ProcessNeed[i]<<"�޷����䵽������"<<endl;
    }

}

void BF()
{
    int temp = -1;
    bool isIn = false;
    cout<<"---------------BF---------------------"<<endl;
    // �������н���
    for(int i = 0; i < ProcessNum; i++){
        // �ȴ�С�����������
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

        // ��FFһ��
        isIn = false;
        // �����������ҵ����Դ�ŵ�ǰ���̵ķ���
        for(int j = 0; j < PartitionNum; j++){
            // ����ҵ��������Ϣ���޸ķ��������С
            if(BestPartition[j] >= ProcessNeed[i]){
                isIn = true;
                cout<<"����"<<ProcessNeed[i]<<"���䵽"<<BestPartition[j]<<"������";
                BestPartition[j] -= ProcessNeed[i];
                cout<<"��ǰ�������Ϊ:";
                for(int k = 0; k < PartitionNum; k++){
                    cout<<BestPartition[k]<<" ";
                }
                cout<<endl;
                break;
            }
        }
        // ���û�ҵ��������ʾ
        if(!isIn)
            cout<<"����"<<ProcessNeed[i]<<"�޷����䵽������"<<endl;
    }

}

void WF()
{
    int temp = -1;
    bool isIn = false;
    cout<<"---------------WF---------------------"<<endl;
    // �������н���
    for(int i = 0; i < ProcessNum; i++){
        // �ȴӴ�С�������
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

        // ��FFһ��
        isIn = false;
        // �����������ҵ����Դ�ŵ�ǰ���̵ķ���
        for(int j = 0; j < PartitionNum; j++){
            // ����ҵ��������Ϣ���޸ķ��������С
            if(WorstPartition[j] >= ProcessNeed[i]){
                isIn = true;
                cout<<"����"<<ProcessNeed[i]<<"���䵽"<<WorstPartition[j]<<"������";
                WorstPartition[j] -= ProcessNeed[i];
                cout<<"��ǰ�������Ϊ:";
                for(int k = 0; k < PartitionNum; k++){
                    cout<<WorstPartition[k]<<" ";
                }
                cout<<endl;
                break;
            }
        }
        // ���û�ҵ��������ʾ
        if(!isIn)
            cout<<"����"<<ProcessNeed[i]<<"�޷����䵽������"<<endl;
    }

}

int main()
{
    init();
    cout<<"��ǰ���������";
    for(int i = 0; i < ProcessNum; i++){
        cout<<ProcessNeed[i]<<" ";
    }
    cout<<endl;
    cout<<"��ǰ���������";
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
