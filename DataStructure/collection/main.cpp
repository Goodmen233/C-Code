#include <iostream>

using namespace std;

#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10
typedef int ElemType;
typedef struct
{
    ElemType *elem;
    int length;
    int listsize;
}SqList;

// ����һ����˳���
bool initSqlist(SqList &l)
{
    l.elem = new ElemType[LIST_INIT_SIZE];
    if(! l.elem) return false;
    l.length = 0;
    l.listsize = LIST_INIT_SIZE;
    return true;
}

// ��β�����Ԫ��
bool insertElem(SqList &l, ElemType e)
{
    // �ж�˳����Ƿ���
    if(l.length == l.listsize){
        // ����
        ElemType *newbase = new ElemType[LIST_INIT_SIZE + LISTINCREMENT];
        // ����Ԫ��
        for(int i = 0; i < l.length; i++){
            newbase[i] = l.elem[i];
        }
        // ָ�����ݺ������
        l.elem = newbase;
    }
     // ����Ԫ��
    l.elem[l.length++] = e;
    return true;
}

// ��ָ��λ�ò���Ԫ��
bool insertElem(SqList &l, ElemType e, int index)
{
    // �ж�λ���Ƿ�Ϸ�
    if(index < 1 || index > l.length - 1) return false;
    // �ж�˳����Ƿ���
    if(l.length == l.listsize){
        // ����
        ElemType *newbase = new ElemType[LIST_INIT_SIZE + LISTINCREMENT];
        // ����Ԫ��
        for(int i = 0; i < l.length; i++){
            newbase[i] = l.elem[i];
        }
        // ָ�����ݺ������
        l.elem = newbase;
    }
    // Ԫ�غ���
    for(int i = l.length - 1; i >= index - 1; i--){
        l.elem[i+1] = l.elem[i];
    }
    // ����Ԫ��
    l.elem[index - 1] = e;
    l.length++;
    return true;
}

// ɾ��ָ�����
ElemType deleteIndex(SqList &l, int index)
{
    // �ж�λ���Ƿ�Ϸ�
    if(index < 1 || index > l.length - 1) return false;
    // ���ɾ��Ԫ��
    ElemType e = l.elem[index - 1];
    // Ԫ��ǰ�ƣ�ֱ�Ӹ���
    for(int i = index; i <= l.length - 1; i++){
        l.elem[i - 1] = l.elem[i];
    }
    l.length--;
    return e;
}

// ��ȡ��Ԫ
ElemType getElem(SqList l, int index)
{
    // �ж�λ���Ƿ�Ϸ�
    if(index < 1 || index > l.length - 1) return false;
    // ����ָ���Ԫ��
    return l.elem[index - 1];
}

// ��ȡ���Ԫ��
ElemType maxElem(SqList l)
{
    ElemType max = l.elem[0];
    for(int i = 0; i < l.length; i++){
        if(max < l.elem[i]) max = l.elem[i];
    }
    return max;
}

// ��ȡ��СԪ��
ElemType minElem(SqList l)
{
    ElemType min = l.elem[0];
    for(int i = 0; i < l.length; i++){
        if(min > l.elem[i]) min = l.elem[i];
    }
    return min;
}

// ����Ԫ�أ����ص�һ�γ��ֵ�λ��
int getIndex(SqList l, ElemType e)
{
    for(int i = 0; i < l.length; i++){
        if(e == l.elem[i]) return i + 1;
    }
    return -1;
}

// ����Ԫ�أ��������һ�γ��ֵ�λ��
int getLastIndex(SqList l, ElemType e)
{
    for(int i = l.length - 1; i >= 0; i--){
        if(e == l.elem[i]) return i + 1;
    }
    return -1;
}

// ��Ԫ�ص�����
void sortSqList(SqList &l)
{
    bool changeable = true;
    for(int i = l.length - 1; i >= 1 && changeable; i--){
        changeable = false;
        for(int j = 0; j < i; j++){
            if(l.elem[j] > l.elem[j + 1]){
                int temp = l.elem[j];
                l.elem[j] = l.elem[j + 1];
                l.elem[j + 1] = temp;
                changeable = true;
            }
        }
    }
}

// ��Ԫ�ص�����
void contrarySqList(SqList &l)
{
    for(int i = 0; i < l.length / 2; i++){
        int temp = l.elem[i];
        l.elem[i] = l.elem[l.length-1-i];
        l.elem[l.length-1-i] = temp;
    }
}

// ˳��������
void sysin(SqList &l, int num)
{
    cout<<"������"<<num<<"������"<<endl;
    for(int i = 0; i < num; i++){
        ElemType e;
        cin>>e;
        insertElem(l, e);
    }
}

// ˳�������
void sysout(SqList l)
{
    for(int i = 0; i < l.length; i++){
        cout<<l.elem[i]<<" ";
    }
    cout<<endl;
}

// �ϲ�������
void mergeSqList(SqList l1, SqList l2, SqList &l3)
{
    sortSqList(l1);
    sortSqList(l2);
    int i = 0;
    int j = 0;
    while(i < l1.length && j < l2.length){
        if(l1.elem[i] < l2.elem[j]){
            insertElem(l3, l1.elem[i]);
            if(i < l1.length) i++;
            continue;
        }else if(l1.elem[i] > l2.elem[j]){
            insertElem(l3, l2.elem[j]);
            if(j < l2.length) j++;
            continue;
        }else{
            insertElem(l3, l2.elem[j]);
            if(i < l1.length) i++;
            if(j < l2.length) j++;
            continue;
        }
    }
    while(i < l1.length){
        insertElem(l3, l1.elem[i++]);
    }
    while(j < l2.length){
        insertElem(l3, l2.elem[j++]);
    }
}

// ȥ��
void distinctSqList(SqList &l)
{
    for(int i = 0; i < l.length; i++){
        for(int j = i + 1; j < l.length; ){
            if(l.elem[i] == l.elem[j]){
                deleteIndex(l, j);
                continue;
            }
            j++;
        }
    }
}

// ȡ����
void unionSqList(SqList sq1, SqList sq2, SqList &sq3)
{
    for(int i = 0; i < sq1.length; i++){
        for(int j = 0; j < sq2.length; j++){
            if(sq1.elem[i] == sq2.elem[j]){
                insertElem(sq3, sq1.elem[i]);
                break;
            }
        }
    }
    distinctSqList(sq3);
}

// ȡ�
void descSqList(SqList sq1, SqList sq2, SqList &sq3)
{
    for(int i = 0; i < sq1.length; i++){
        int count = 0;
        for(int j = 0; j < sq2.length; j++){
            if(sq1.elem[i] == sq2.elem[j]){
                count++;
            }
        }
        if(count == 0) insertElem(sq3, sq1.elem[i]);
    }
}

// ʵ�ּ��ϵĽ�����
int main()
{
    SqList sq1;
    SqList sq2;
    SqList sq3;
    initSqlist(sq3);
    initSqlist(sq1);
    initSqlist(sq2);
    insertElem(sq1, 2);
    insertElem(sq1, 3);
    insertElem(sq1, 5);
    insertElem(sq1, 6);
    insertElem(sq1, 5);
    insertElem(sq2, 7);
    insertElem(sq2, 3);
    insertElem(sq2, 10);
    insertElem(sq2, 5);
    insertElem(sq2, 54);
    cout<<"����һ�е�Ԫ�أ�";
    sysout(sq1);
    cout<<"���϶��е�Ԫ�أ�";
    sysout(sq2);
    // ʵ�ּ��ϵĲ�-------------------
    mergeSqList(sq1, sq2, sq3);
    distinctSqList(sq3);
    cout<<"���ϵĲ�����";
    sysout(sq3);
    // ʵ�ּ��ϵĽ�-------------------
    SqList sq4;
    initSqlist(sq4);
    unionSqList(sq1, sq2, sq4);
    cout<<"���ϵĽ�����";
    sysout(sq4);
    // ʵ�ּ��ϵĲ�
    SqList sq5;
    initSqlist(sq5);
    descSqList(sq1, sq2, sq5);
    cout<<"���ϵĲ��";
    sysout(sq5);
    return 0;
}
//
