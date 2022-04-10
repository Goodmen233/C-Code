#include <iostream>
#include <fstream>
// https://www.docin.com/p-760510162.html
using namespace std;

#define MaxNumber 100
int  PageOrder[MaxNumber];              // ҳ������
int  Simulate[MaxNumber][MaxNumber];    // ģ�����
int  PageCount[MaxNumber];              // �洢ÿ��ҳ�水����ڼ��γ���
int  PageNum,LackNum,BlockNum;          // ҳ�������ȱҳ����������
double  LackPageRate;                   // ȱҳ��
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

    cout<<"��ǰҳ������Ϊ��"<<endl;
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

// ʹָ���е�ֵΪnull
void columnNull(int i)
{
    for(int j = 0; j < BlockNum; j++){
        Simulate[j][i] = -1;
    }
}

// �Ƚ��ȳ�ҳ���û�
void FIFO()
{
    int tempSimulate[BlockNum];// ���浱ǰ������
    // ���õ�һ�����Ľ���
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
    int fi = 1;// �����Ƚ���ҳ��
    // ��������ҳ������
    for(int i = 1; i < PageNum; i++){
        found = false;
        // �ж��Ƿ���������
        for(int j = 0; j < BlockNum; j++){
            if(PageOrder[i] == tempSimulate[j]){
                found = true;
                break;
            }
        }
        if(found){
            // ������е�ģ�ⶼΪnull
            columnNull(i);
            continue;
        }
        // �����滻�ϴ��ȵ���ҳ��,ȱҳ����һ,fiָ����һ���ȵ���ҳ��
        tempSimulate[fi] = PageOrder[i];
        if(++fi == BlockNum) fi = 0;
        LackNum++;
        // �ѽ�����ƹ���
        for(int j = 0; j < BlockNum; j++){
            Simulate[j][i] = tempSimulate[j];
        }
    }
    LackPageRate = (double) LackNum / PageNum * 100;
}

void OPI()
{
    int tempSimulate[BlockNum];// ���浱ǰ������
    // ���õ�һ�����Ľ���
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
    // ��������ҳ������
    for(int i = 1; i < PageNum; i++){
        found = false;
        // �ж��Ƿ���������
        for(int j = 0; j < BlockNum; j++){
            if(PageOrder[i] == tempSimulate[j]){
                found = true;
                break;
            }
        }
        if(found){
            // ������е�ģ�ⶼΪnull
            columnNull(i);
            continue;
        }
        // �����ж��Ƿ��п���λ��
        found = false;
        for(int x = 0; x < BlockNum; x++){
            if(tempSimulate[x] == -1){
                tempSimulate[x] = PageOrder[i];
                found = true;
                break;
            }
        }
        // �����ҵ�����û���ҳ��
        // ����PageCount
        for(int x = 0; x < PageNum; x++){
            PageCount[x] = 0;
        }
        if(!found){
            for(int x = 0; x < BlockNum; x++){
                for(int k = i + 1; k < PageNum; k++){
                    // �����������һ
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
            // �ж��ĸ�������ʹ��
            for(int x = 0; x < BlockNum; x++){
                if(PageCount[x] > maxValue){
                    maxIndex = x;
                    maxValue = PageCount[x];
                }
            }
            tempSimulate[maxIndex] = PageOrder[i];
        }
        // ȱҳ����һ�����ƽ����ģ������
        LackNum++;
        // �ѽ�����ƹ���
        for(int j = 0; j < BlockNum; j++){
            Simulate[j][i] = tempSimulate[j];
        }
    }
    LackPageRate = (double) LackNum / PageNum * 100;
}

void LRU()
{
    int tempSimulate[BlockNum];// ���浱ǰ������
    // ���õ�һ�����Ľ���
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
    // ��������ҳ������
    for(int i = 1; i < PageNum; i++){
        found = false;
        // �ж��Ƿ���������
        for(int j = 0; j < BlockNum; j++){
            if(PageOrder[i] == tempSimulate[j]){
                found = true;
                break;
            }
        }
        if(found){
            // ������е�ģ�ⶼΪnull
            columnNull(i);
            continue;
        }
        // �����ж��Ƿ��п���λ��
        found = false;
        for(int x = 0; x < BlockNum; x++){
            if(tempSimulate[x] == -1){
                tempSimulate[x] = PageOrder[i];
                found = true;
                break;
            }
        }
        // �����ҵ�������δʹ�õ�ҳ��
        // ����PageCount
        for(int x = 0; x < PageNum; x++){
            PageCount[x] = 0;
        }
        if(!found){
            for(int x = 0; x < BlockNum; x++){
                for(int k = i - 1; k >= 0; k--){
                    // �����������һ
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
            // �ж��ĸ�������δʹ��
            for(int x = 0; x < BlockNum; x++){
                if(PageCount[x] > maxValue){
                    maxIndex = x;
                    maxValue = PageCount[x];
                }
            }
            tempSimulate[maxIndex] = PageOrder[i];
        }
        // ȱҳ����һ�����ƽ����ģ������
        LackNum++;
        // �ѽ�����ƹ���
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
    cout<<"---------------------FIFO�Ľ��Ϊ��---------------------"<<endl;
    printSimulate();
    cout<<"ȱҳ��Ϊ��"<<LackNum<<" "<<"ȱҳ��Ϊ��"<<LackPageRate<<"%"<<endl<<endl;

    init();
    OPI();
    cout<<"---------------------OPI�Ľ��Ϊ��---------------------"<<endl;
    printSimulate();
    cout<<"ȱҳ��Ϊ��"<<LackNum<<" "<<"ȱҳ��Ϊ��"<<LackPageRate<<"%"<<endl<<endl;

    init();
    LRU();
    cout<<"---------------------LRU�Ľ��Ϊ��---------------------"<<endl;
    printSimulate();
    cout<<"ȱҳ��Ϊ��"<<LackNum<<" "<<"ȱҳ��Ϊ��"<<LackPageRate<<"%"<<endl<<endl;

    return 0;
}
