#include <iostream>
#include <fstream>

using namespace std;

#define MaxNumber 100
// 1:1 0 2   4:3 3 0   0:0 2 0
int main()
{
    int Available[MaxNumber];// ������Դ��
    int Max[MaxNumber][MaxNumber];// ���������
    int Allocation[MaxNumber][MaxNumber];// �Ѿ�������
    int Need[MaxNumber][MaxNumber];// ��ǰ��Ҫ����
    int Request[MaxNumber];// �������Դ��
    int SafeOrder[MaxNumber];// ��ȫ����

    ifstream input("data.txt");
    // nΪ���̸�����mΪ��Դ����
    int n, m;
    input>>n;
    input>>m;
    // �������������
    for(int i = 0; i < n; i++){
        for(int j =0; j < m; j++){
            input>>Max[i][j];
        }
    }
    // �����Ѿ�������
    for(int i = 0; i < n; i++){
        for(int j =0; j < m; j++){
            input>>Allocation[i][j];
        }
    }
    // ���ÿ�����Դ��
    for(int j = 0; j < m; j++){
        input>>Available[j];
    }
    // ���õ�ǰ��Ҫ����
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
    }

    int RequestI = 0;
    // �����Ƿ��һ�η�����Դ
    bool isFirst = 1;
    while(1){
        if(isFirst){
            // ������������,һ��ʼĬ��û��
            RequestI = 0;
            for(int j = 0; j < m; j++){
                Request[j] = 0;
            }
            isFirst = 0;
        }else{
            // ���û������������У��������1������ֹ����
            cout<<"�Ƿ���ֹ����(Y/N):";
            char res;
            cin>>res;
            if(res == 'N' || res == 'n'){
                cout<<"������ڼ������̣�0-n��������Դ��";
                cin>>RequestI;
                cout<<"�������������Դ��:";
                for(int i = 0; i < m; i++){
                    cin>>Request[i];
                }
            }else if(res == 'Y' || res == 'y'){
                break;
            }else{
                cout<<"���벻�Ϸ�������������"<<endl;
                continue;
            }
        }

        // ���Request<=Need
        bool isGood = 1;
        for(int i = 0; i < m; i++){
            if(Request[i] > Need[RequestI][i]){
                isGood = 0;
                break;
            }
        }
        if(isGood){
            // ���Request<=Available
            for(int i = 0; i < m; i++){
                if(Request[i] > Available[i]){
                    isGood = 0;
                    break;
                }
            }
            if(isGood){
                // ����Դ ��̽ �����������̣��޸�����ֵ
                for(int i = 0; i < m; i++){
                    Available[i] -= Request[i];
                    Allocation[RequestI][i] += Request[i];
                    Need[RequestI][i] -= Request[i];
                }
                // ���а�ȫ�Լ�⣬�ɹ������ȫ���У���֮�ָ�֮ǰ����Դ������ʾ����ȫ
                // ���ù�������
                int Work[m];
                for(int i = 0; i < m; i++){
                    Work[i] = Available[i];
                }
                // �����������
                int Finish[n];
                for(int i = 0; i < n; i++){
                    Finish[i] = 0;
                }
                // �����ж� Finish=false && Need<=Work �Ľ���
                bool isRun = 1;// �ж��Ƿ�������������һ��ֵ����Ϊ0������ǽ���
                int temp = 0;// ��Ű�ȫ���е��±��¼
                while(isRun){
                    isRun = 0;// �������һ��ֵ����Ϊ0�������û�п����еĽ���
                    for(int i = 0; i < n; i++){
                        isGood = 1;
                        // ��������ʾ��ִ���겢�ͷ���Դ
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
                                SafeOrder[temp++] = i;// �洢��ȫ����
                            }
                        }


                    }
                    if(!isRun) break;// ���û�н��̼���ִ�б�ʾ����
                }
                isGood = 1;
                // �ж����еĽ����Ƿ���Finish������ȫ
                for(int i = 0; i < n; i++){
                    // ��֮����ȫ���ָ�֮ǰ����Դ����ʾ
                    if(Finish[i] == 0){
                        isGood = 0;
                        break;
                    }
                }
                if(isGood){
                    cout<<"��ȫ����ȫ����Ϊ��";
                    for(int i = 0; i < n; i++){
                        cout<<SafeOrder[i]<<" ";
                    }
                    cout<<endl;
                }else{
                    cout<<"����ȫ��"<<endl;
                    // ��ԭ֮ǰ��ֵ
                    for(int i = 0; i < m; i++){
                        Available[i] += Request[i];
                        Allocation[RequestI][i] -= Request[i];
                        Need[RequestI][i] += Request[i];
                    }
                    break;// ����
                }
            }else{
                // ��֮�����㹻��Դ���ȴ���
                cout<<"�����㹻��Դ�����Ժ�����"<<endl;
            }
        }else{
            // ��֮���Request>Need,��ʾ������Դ���ڽ���������Դ
            cout<<"������Դʧ�ܣ���Ϊ��Ҫ����Դ�ѳ��������������ֵ"<<endl;
        }
    }
    return 0;
}
