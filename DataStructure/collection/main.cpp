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

// 创建一个空顺序表
bool initSqlist(SqList &l)
{
    l.elem = new ElemType[LIST_INIT_SIZE];
    if(! l.elem) return false;
    l.length = 0;
    l.listsize = LIST_INIT_SIZE;
    return true;
}

// 往尾部添加元素
bool insertElem(SqList &l, ElemType e)
{
    // 判断顺序表是否满
    if(l.length == l.listsize){
        // 扩容
        ElemType *newbase = new ElemType[LIST_INIT_SIZE + LISTINCREMENT];
        // 拷贝元素
        for(int i = 0; i < l.length; i++){
            newbase[i] = l.elem[i];
        }
        // 指向扩容后的数组
        l.elem = newbase;
    }
     // 插入元素
    l.elem[l.length++] = e;
    return true;
}

// 在指定位置插入元素
bool insertElem(SqList &l, ElemType e, int index)
{
    // 判断位置是否合法
    if(index < 1 || index > l.length - 1) return false;
    // 判断顺序表是否满
    if(l.length == l.listsize){
        // 扩容
        ElemType *newbase = new ElemType[LIST_INIT_SIZE + LISTINCREMENT];
        // 拷贝元素
        for(int i = 0; i < l.length; i++){
            newbase[i] = l.elem[i];
        }
        // 指向扩容后的数组
        l.elem = newbase;
    }
    // 元素后移
    for(int i = l.length - 1; i >= index - 1; i--){
        l.elem[i+1] = l.elem[i];
    }
    // 插入元素
    l.elem[index - 1] = e;
    l.length++;
    return true;
}

// 删除指定序号
ElemType deleteIndex(SqList &l, int index)
{
    // 判断位置是否合法
    if(index < 1 || index > l.length - 1) return false;
    // 标记删除元素
    ElemType e = l.elem[index - 1];
    // 元素前移，直接覆盖
    for(int i = index; i <= l.length - 1; i++){
        l.elem[i - 1] = l.elem[i];
    }
    l.length--;
    return e;
}

// 读取表元
ElemType getElem(SqList l, int index)
{
    // 判断位置是否合法
    if(index < 1 || index > l.length - 1) return false;
    // 返回指向的元素
    return l.elem[index - 1];
}

// 获取最大元素
ElemType maxElem(SqList l)
{
    ElemType max = l.elem[0];
    for(int i = 0; i < l.length; i++){
        if(max < l.elem[i]) max = l.elem[i];
    }
    return max;
}

// 获取最小元素
ElemType minElem(SqList l)
{
    ElemType min = l.elem[0];
    for(int i = 0; i < l.length; i++){
        if(min > l.elem[i]) min = l.elem[i];
    }
    return min;
}

// 查找元素，返回第一次出现的位置
int getIndex(SqList l, ElemType e)
{
    for(int i = 0; i < l.length; i++){
        if(e == l.elem[i]) return i + 1;
    }
    return -1;
}

// 查找元素，返回最后一次出现的位置
int getLastIndex(SqList l, ElemType e)
{
    for(int i = l.length - 1; i >= 0; i--){
        if(e == l.elem[i]) return i + 1;
    }
    return -1;
}

// 表元素的排序
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

// 表元素的逆置
void contrarySqList(SqList &l)
{
    for(int i = 0; i < l.length / 2; i++){
        int temp = l.elem[i];
        l.elem[i] = l.elem[l.length-1-i];
        l.elem[l.length-1-i] = temp;
    }
}

// 顺序表的输入
void sysin(SqList &l, int num)
{
    cout<<"请输入"<<num<<"个数："<<endl;
    for(int i = 0; i < num; i++){
        ElemType e;
        cin>>e;
        insertElem(l, e);
    }
}

// 顺序表的输出
void sysout(SqList l)
{
    for(int i = 0; i < l.length; i++){
        cout<<l.elem[i]<<" ";
    }
    cout<<endl;
}

// 合并两个表
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

// 去重
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

// 取交集
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

// 取差集
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

// 实现集合的交并差
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
    cout<<"集合一中的元素：";
    sysout(sq1);
    cout<<"集合二中的元素：";
    sysout(sq2);
    // 实现集合的并-------------------
    mergeSqList(sq1, sq2, sq3);
    distinctSqList(sq3);
    cout<<"集合的并集：";
    sysout(sq3);
    // 实现集合的交-------------------
    SqList sq4;
    initSqlist(sq4);
    unionSqList(sq1, sq2, sq4);
    cout<<"集合的交集：";
    sysout(sq4);
    // 实现集合的差
    SqList sq5;
    initSqlist(sq5);
    descSqList(sq1, sq2, sq5);
    cout<<"集合的差集：";
    sysout(sq5);
    return 0;
}
//
