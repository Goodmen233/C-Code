#include <iostream>
#include <typeinfo>
#include <stdio.h>
#include <iomanip>
#include <set>
#include <vector>
#include <queue>
#include <map>
#include <string>

#define Maxnum 99
using namespace std;
//-------------------------------------------------ZZC start-------------------------------------
//-------------------------------------------------表达式转NFA--------------------------------------
typedef struct Node{//根节点
    char type;
    char Char;
    Node *lc;
    Node *rc;
    int start_S;
    int end_S;
}*Tree_root;

void default_Node(Node* N){//初始化根节点
    N->lc = N->rc = NULL;
}

Node* default_L_Node(char c){//包装叶子节点
    Node* N = new Node;
    default_Node(N);
    N->Char = c;
    N->type = 'l';//将根节点的类型设置为l
    return N;
}

int Bvisit_Tree(Tree_root Tree){//后序遍历这个树
    if(Tree->type=='l'){
        cout<<Tree->Char<<" ";
        return 1;
    }
    else{
        Bvisit_Tree(Tree->lc);
        if(Tree->rc != NULL) Bvisit_Tree(Tree->rc);
            cout<<Tree->type<<" ";
        return 1;
    }
}

Node* default_R_Node(char type,Node* lchild,Node* rchild){//包装根节点
    Node* N = new Node;
    default_Node(N);
    N->type = type;
    N->lc = lchild;
    N->rc = rchild;
    return N;
}

typedef struct op_Stack{//定义操作符栈
    char* top;
    char* bottom;
    char St[Maxnum];//用数组作为栈
    };

void Init_op_Stack(op_Stack &S){//对符号栈进行初始化
    S.top = S.bottom = S.St;
}

void push_op(op_Stack &S,char a){//入栈
    *(S.top)=a;
    S.top++;
}

char pop_op(op_Stack &S){//出战
    S.top--;
    return *S.top;
}

char top_op(op_Stack &S){//返回栈顶元素
    return *(S.top-1);
}

typedef struct yufa_Stack{//定义操作数栈
    int top;
    int bottom;
    Node* St[Maxnum];
    };

void Init_yufa_Stack(yufa_Stack &S){//初始化操作数栈
    S.top=S.bottom=0;
}

void push_yufa(yufa_Stack &S,Node* c){//入栈
    S.St[S.top] = c;
    S.top++;
}

Node* pop_yufa(yufa_Stack &S){//出战
    S.top--;
    return S.St[S.top];
}

int o_youxianji(char o){//判断操作符的优先级
    switch ( o ){
    case '#':
        return 1;
    case  '('   :
       return  2;
    case  '|':
        return 3;
    case  '+':
        return 4;
    case  ')':
        return 5;
    case '*':
        return 6;
    default  :
        return 0; //不是操作符
    }
}

bool if_cat(char l,char r){//判断是否加入连接运算符

    if(!o_youxianji(l)&&r=='(')  //char+(
        return 1;
    else if(!o_youxianji(l)&&!o_youxianji(r)) //char+char
        return 1;
    else if(l==')'&&!o_youxianji(r))  //)+char
        return 1;
    else if(l=='*'&&!o_youxianji(r))  //*+char
        return 1;
    else if(l==')'&&r=='(')  //)+(
        return 1;
    else if(l=='*'&&r=='(')  //*+(
        return 1;
    else return 0;
       }

string cin_Regular_Exp(){//输入正则表达式
    cout<<"请输入正则表达式：";
    string R_exp;
    cin>>R_exp;
    R_exp.insert(0,"#");
    R_exp.append("#");  //加入结束符
    int l=0,r=1;
    while(R_exp[r]!='#'){    //将正则表达式加入cat连接符
        if(if_cat(R_exp[l],R_exp[r]))
            R_exp.insert(l+1,"+");
        l++;r++;}
    return R_exp;
}

bool if_high(char odd_op,char new_op){//比较运算符优先级
    int odd_=o_youxianji(odd_op);
    int new_=o_youxianji(new_op);
    if(new_>odd_) return 1;
    return 0;
}

Node* Connect_Nodes(char op,yufa_Stack& char_st){//根据运算符连接节点
    switch ( op ){
    case  '|': //双目运算符
        return default_R_Node(op,pop_yufa(char_st),pop_yufa(char_st));
    case  '+': //双目运算符
        return default_R_Node(op,pop_yufa(char_st),pop_yufa(char_st));;
    case  '*': //单目运算符
        return default_R_Node(op,pop_yufa(char_st),NULL);
    }
}

Tree_root Yufa_Tree(string exp){//构造语法树
    op_Stack op_st;//符号栈
    yufa_Stack char_st;//语法栈
    Init_op_Stack(op_st);
    Init_yufa_Stack(char_st);//初始化语法栈和符号栈
    push_op(op_st,exp[0]);
    for(int i = 1; i < exp.size()-1; i++){
        char c = exp[i];
        if(!o_youxianji(c)){
            push_yufa(char_st,default_L_Node(c));
        }
        else{
            if(c == '('){
                push_op(op_st,'(');//如果是（则直接入栈
               }
            else if(c == ')'){//将遇到‘（’之间栈的内容清空
                while(top_op(op_st) != '('){
                        char op = pop_op(op_st);
                        push_yufa(char_st,Connect_Nodes(op,char_st));
                      }
                      pop_op(op_st);

               }
            else if(c == '*'){
                push_yufa(char_st,Connect_Nodes(c,char_st));
            }
            else {
                while(!if_high(top_op(op_st),c)){//循环比较
                    char op = pop_op(op_st);
                    push_yufa(char_st,Connect_Nodes(op,char_st));
                }
                push_op(op_st,c);
            }
        }
    }
    while(top_op(op_st)!='#'){
        char op = pop_op(op_st);
        push_yufa(char_st,Connect_Nodes(op,char_st));
    }
    return char_st.St[char_st.bottom];
}


int add_find_char(char *A,char a,int& size_){//在字符数组中加入新元素,若已经存在，则返回数组编号
    if(size_==0) {A[0]=a;size_++;return Maxnum;}
    else{
        int i;
        for(i=0;i<size_;i++)
            if(*(A+i)==a) return i;  //找到了 不添加
        A[i]=a;size_++;
        return Maxnum; //没找到，添加
    } }
//得到正则表达式的吸收符号
int Get_absorb_char(string exp,char *ab_chars){
    int char_size = 0;
    for(int i=0;i<exp.size();i++){
        if(!o_youxianji(exp[i])) //不是操作符
            add_find_char(ab_chars,exp[i],char_size);}
    return char_size;
}
//状态集
typedef struct states_set{
    int st_set[Maxnum];
    int length=0;};
//图的矩阵表示
typedef struct Skips_graph{
    int S_size;//状态数
    int ab_char_size; //吸收字符种类数
    char absorb_char[Maxnum]; //吸收字符
    int Skips[Maxnum][Maxnum]; //状态跳转
    int empty_Skips[Maxnum][Maxnum]; //空状态跳转 每行第一列放该状态的空跳转数
    int start_S; //起点
    int end_S; //终点(对nfa而言，只有一个终点)
    states_set end_set; //终点集(对于dfa而言，有多个终点)
};
//初始化状态转移矩阵
void default_graph(Skips_graph &S,char *ab_char,int ab_char_size){
    S.S_size=0;
    S.ab_char_size=ab_char_size;
    for(int i=0;i<ab_char_size;i++)
        S.absorb_char[i]=ab_char[i];
    for(int i=0;i<Maxnum;i++)
        for(int j=0;j<Maxnum;j++)
            S.Skips[i][j]=Maxnum;   //表示无跳转
    for(int i=0;i<Maxnum;i++)
        S.empty_Skips[i][0]=0;   //空跳转个数为0
    }
//添加新的状态跳转 图 起点 终点 吸收字符
void add_Skips(Skips_graph& S,int B,int E,char c){
    int ab_char=add_find_char(S.absorb_char,c,S.ab_char_size);
    S.Skips[B][ab_char]=E;
    }
void add_empty_Skips(Skips_graph& S,int B,int E){
    S.empty_Skips[B][0]=S.empty_Skips[B][0]+1; //空跳转数加一
    int size_=S.empty_Skips[B][0];
    S.empty_Skips[B][size_]=E;}
//构造叶子节点状态转移
void Leaf_Skip(Skips_graph& S,Node& N){
    N.start_S=S.S_size;S.S_size++;
    N.end_S=S.S_size;S.S_size++;
    add_Skips(S,N.start_S,N.end_S,N.Char); //添加始末状态转移
    }
//构造根状态转移
void root_Skip(Skips_graph& S,Node& N){
    //或根构造
    if(N.type=='|'){
        N.start_S=S.S_size;S.S_size++;
        N.end_S=S.S_size;S.S_size++;
        add_empty_Skips(S,N.start_S,N.lc->start_S);
        add_empty_Skips(S,N.start_S,N.rc->start_S);
        add_empty_Skips(S,N.lc->end_S,N.end_S);
        add_empty_Skips(S,N.rc->end_S,N.end_S);}
    //闭包构造
    else if(N.type=='*'){
        N.start_S=S.S_size;S.S_size++;
        N.end_S=S.S_size;S.S_size++;
        add_empty_Skips(S,N.start_S,N.lc->start_S);
        add_empty_Skips(S,N.start_S,N.end_S);
        add_empty_Skips(S,N.lc->end_S,N.lc->start_S);
        add_empty_Skips(S,N.lc->end_S,N.end_S);}
    //连接构造
    else if(N.type=='+'){
        N.start_S=N.lc->start_S;
        N.end_S=N.rc->end_S;
        add_empty_Skips(S,N.lc->end_S,N.rc->start_S);}
    else {cout<<"fault!";}
}

//递归构造NFA
void NFA(Tree_root Tree, Skips_graph &S, string exp){
    //构造状态转移矩阵
    if(Tree->type=='l') //叶子状态转移
        Leaf_Skip(S,*Tree);
    else
    {
        NFA(Tree->lc,S,exp); //构造左子树状态转移
        if(Tree->rc!=NULL) NFA(Tree->rc,S,exp); //构造右子树状态转移
        root_Skip(S,*Tree);//构造根状态转移}
}}

void NFA_(Tree_root Tree, Skips_graph &S, string exp){
    //得到吸收符号
    char ab_chars[Maxnum]; //正则表达式中的吸收符号
    int ab_chars_size=Get_absorb_char(exp,ab_chars);
    //初始化状态转移(NFA)
    default_graph(S,ab_chars,ab_chars_size);
    //构造
    NFA(Tree,S,exp);
    S.start_S=Tree->start_S;S.end_S=Tree->end_S;}
//读NFA
void vist_Graph(Skips_graph &S,string type){
    cout<<"状态跳转矩阵:"<<endl;
    cout<<"状态";
    for(int i=0;i<S.ab_char_size;i++)
        cout<<setw(6)<<S.absorb_char[i];
    cout<<setw(8)<<"空跳转";
    cout<<endl;
    for(int i=0;i<S.S_size;i++)
        {cout<<setw(3)<<i;
        for(int j=0;j<S.ab_char_size;j++)
            if(S.Skips[i][j]==Maxnum) cout<<setw(6)<<"-";
            else cout<<setw(6)<<S.Skips[i][j];
        //输出空跳转
        cout<<setw(6)<<" ";
        for(int j=0;j<S.empty_Skips[i][0];j++)
            cout<<S.empty_Skips[i][j+1]<<" ";
        cout<<endl;}
    cout<<"起点:"<<S.start_S<<" ";
    if(type=="nfa") cout<<"终点:"<<S.end_S<<" ";
    else{
        cout<<"终点:";
        for(int i=0;i<S.end_set.length;i++)
            cout<<S.end_set.st_set[i]<<" ";
    }
    cout<<endl;}

Skips_graph exp2Nfa()
{
  string exp=cin_Regular_Exp();
  cout<<exp<<endl;
  //构建语法树
  Node* Tree=Yufa_Tree(exp);
  //遍历语法树
  cout<<"后序遍历语法树: ";
  Bvisit_Tree(Tree);
  cout<<endl;
  //构造NFA
  cout<<endl<<"********************Thompson算法构造NFA***********************"<<endl;
  Skips_graph nfa;
  NFA_(Tree,nfa,exp);
  vist_Graph(nfa,"nfa");
  return nfa;
}
//-------------------------------------------------ZZC end---------------------------------------


//-------------------------------------------------LLC start-------------------------------------
//-------------------------------------------------NFA->DFA--------------------------------------
// 存储DFA的矩阵结构
struct Matrix
{
    char *vertex;           //节点名称
    set<char>  transChar;    //状态转移字母表
    char** transArray;            //转移矩阵
    int  edgeCount;           //边数统计
    int  nodeCount;           //节点数统计
    char  starNode;         //开始节点
    set<char>  endNodeSet;   //终止节点集
};
//向状态集中添加元素
bool add_s_to_set(states_set &Set,int s){
    for(int i=0;i<Set.length;i++){
        if(Set.st_set[i]==s) return 0;
    }
    Set.st_set[Set.length]=s;
    Set.length++;
    return 1;
}
//求集合的空闭包 空跳转
void empty_closure(states_set &Set,int empty_Skips[][Maxnum]){
    for(int i=0;i<Set.length;i++){
        int p=Set.st_set[i];
        for(int j=0;j<empty_Skips[p][0];j++){
            add_s_to_set(Set,empty_Skips[p][j+1]);}
        }
    cout<<"闭包："<<endl;
    for(int i=0;i<Set.length;i++) cout<<Set.st_set[i]<<" ";cout<<endl;
}

//计算从当前状态集出发，经过a字符，能到的下一个状态集
void Smove(states_set Set,states_set &next_Set,int absorb_char,Skips_graph &nfa){
    //对每个状态都计算经过a得到的新状态
    for(int i=0;i<Set.length;i++)
    {   int nfa_i=Set.st_set[i];  //在nfa中的状态
        if(nfa.Skips[nfa_i][absorb_char]!=Maxnum) //有跳转
            add_s_to_set(next_Set,nfa.Skips[nfa_i][absorb_char]);}
}

//判断当前状态集是否存在，若存在则返回新状态，若不存在则返回0
int if_exist(states_set *Sets,states_set Set,int sets_length){
    //找到长度相等的进行比对
    int T; //判断是否有相同的状态集
    for(int i=0;i<sets_length;i++){
        if(Sets[i].length==Set.length){
            T=1; //先假设该集合是
            for(int j=0;j<Set.length;j++){
                int t=0; //判断是否找到相同元素
                for(int k=0;k<Sets[i].length;k++)
                    if(Set.st_set[j]==Sets[i].st_set[k]){t=1;break;}
                if(t==0) {T=0;break;} //找不到，该集合不是
                }
            if(T==1) return i;
        }}
    return 0;
}

//NFA转为DFA
void DFA(Skips_graph &nfa,Skips_graph &dfa,states_set *sets){
    for(int i=0;i<dfa.S_size;i++){
        //计算当前状态的跳转状态集 c为吸收字符
        for(int c=0;c<dfa.ab_char_size;c++){
            states_set next_set;
            Smove(sets[i],next_set,c,nfa);//计算Smove(j,char)
            if(next_set.length==0) continue; //该状态不吸收该字符
            empty_closure(next_set,nfa.empty_Skips);//计算Smove的空闭包
            //判断该状态集是否已经存在,如果不存在则作为新状态集加入dfa
            int S=if_exist(sets,next_set,dfa.S_size);
            if(!S){
                S=dfa.S_size;
                sets[S]=next_set;
                dfa.S_size++;
            }
            //在dfa上添加当前状态吸收字符的跳转
            dfa.Skips[i][c]=S;
        }
    }
}

//判断集合中是否有某个元素
bool find_s(states_set set_,int s){
    for(int i=0;i<set_.length;i++)
        if(set_.st_set[i]==s) return 1;
    return 0;
}

void DFA_(Skips_graph &nfa,Skips_graph &dfa){
    //初始化dfa
    default_graph(dfa,nfa.absorb_char,nfa.ab_char_size);
    states_set sets[Maxnum];//新旧状态表
    //先求起始状态的空闭包
    states_set begin_set;
    add_s_to_set(begin_set,nfa.start_S);
    empty_closure(begin_set,nfa.empty_Skips);
    //将起始状态集作为DFA的起始状态加入
    dfa.start_S=0;
    //将起始状态集加入状态集与新状态的对照表中
    sets[dfa.S_size]=begin_set;
    dfa.S_size++;
    DFA(nfa,dfa,sets);
    //找到dfa的所有终点状态
    int end_s=nfa.end_S;
    for(int i=0;i<dfa.S_size;i++){
        if(find_s(sets[i],end_s)){
            add_s_to_set(dfa.end_set,i);
        }}
}
Matrix Skips_graph2Matrix(Skips_graph dfa){
    Matrix m;
    m.edgeCount=0;
    //节点数转化
    m.nodeCount=dfa.S_size;
    m.transArray = new char*[m.nodeCount];
    m.vertex = new char[m.nodeCount];
    //转移数组转化、节点名称转化、边数转化
    for(int i=0;i<dfa.S_size;i++){
        m.vertex[i]=i + '0';
        m.transArray[i] = new char[dfa.ab_char_size];
        for(int j=0;j<dfa.ab_char_size;j++){
            if(dfa.Skips[i][j] == Maxnum) m.transArray[i][j]='-';
            else m.transArray[i][j]=dfa.Skips[i][j]+'0';
        }
    }
    //吸收字符转化
    for(int c=0;c<dfa.ab_char_size;c++){
            m.transChar.insert(dfa.absorb_char[c]);
    }
    //开始节点转化
    m.starNode=dfa.start_S+'0';
    //终止节点转化
    for(int i=0;i<dfa.end_set.length;i++){
        m.endNodeSet.insert(dfa.end_set.st_set[i] + '0');
    }
    return m;
}

Matrix nfa2Dfa(Skips_graph nfa)
{
    Skips_graph dfa;
    DFA_(nfa,dfa);
    vist_Graph(dfa,"dfa");
    return Skips_graph2Matrix(dfa);
}
//-------------------------------------------------LLC end---------------------------------------


//-------------------------------------------------CCB start--------------------------------------
//-------------------------------------------------最小化DFA--------------------------------------
// 状态组
struct Group
{
    int pk;
    set<char> stateSet;
    // 比较符重载
    bool operator <(const Group & g) const
	{
		return pk<g.pk;
	}

};

int countPk = 0;// Group主键自增依据

// 获取当前状态通过字符ch转换的下一个状态组
Group getNextStateGroup(Matrix dfa, char state, char ch, set<Group> res)
{
    char nextState;
    // 找到state对应的行
    int row;
    for(int i = 0; i < dfa.nodeCount; i++){
        if(dfa.vertex[i] == state){
            row = i;
            break;
        }
    }
    int col = 0;
    // 找到ch对应的列
    for(set<char>::iterator it=dfa.transChar.begin();it!=dfa.transChar.end();it++){
        if(*it == ch){
            break;
        }
        col++;
    }
    // 获取值
    nextState = dfa.transArray[row][col];
    // 找到对应的组
    for(set<Group>::iterator it=res.begin();it!=res.end();it++){
        Group t = *it;
        if(t.stateSet.find(nextState) != t.stateSet.end()){// 找到对应组，直接返回
            return t;
        }
    }
    // 如果没找到返回一个主键为-999的Group
    Group err;
    err.pk = -999;
    return err;
}

// 分割集合
void split(Matrix dfa, set<Group> & res, char ch)
{
    // 声明一个队列先放入初始集
    queue<Group> q;
    for(set<Group>::iterator it=res.begin();it!=res.end();it++){
        q.push(*it);
    }
    while(! q.empty()){
        Group curGroup = q.front();
        q.pop();
        map<Group, set<char> > sMap;
        for(set<char>::iterator it=curGroup.stateSet.begin();it!=curGroup.stateSet.end();it++){
            char curState = *it;
            // 找到当前状态通过字符ch转移的状态组
            Group nextGroup = getNextStateGroup(dfa, curState,ch, res);
            sMap[nextGroup].insert(curState);
        }
        // 如果转换后映射多个状态组，将其拆分
        if(sMap.size() != 1){
            res.erase(curGroup);
            map<Group, set<char> >::iterator iter;
            for(iter = sMap.begin(); iter != sMap.end(); iter++) {
                set<char> tSet = iter->second;
                Group t;
                t.pk = countPk++;
                t.stateSet = tSet;
                // res加入
                res.insert(t);
                // queue加入
                q.push(t);
            }
        }
    }


}

// 对res重命名,返回一个map映射
map<char, char> rename(set<Group> res)
{
    // 通过map key=原来状态 value=重命名的状态
    map<char, char> dic;
    for(set<Group>::iterator i = res.begin(); i != res.end(); i++){
        Group g = *i;
        set<char> set0 = g.stateSet;
        char value = *set0.begin();
        for(set<char>::iterator j = set0.begin(); j != set0.end(); j++){
            dic[*j] = value;
        }
    }
    return dic;
}

// 获取最小DFA
Matrix getMinDfa(Matrix dfa)
{
    // 构造所有终态集
    Group finalState;
    finalState.pk = countPk++;
    for(set<char>::iterator i = dfa.endNodeSet.begin(); i != dfa.endNodeSet.end(); i++){
        finalState.stateSet.insert(*i);
    }
    // 构造所有非终态集
    Group noFinalState;
    noFinalState.pk = countPk++;
    for(int i = 0; i < dfa.nodeCount; i++){
        if(dfa.endNodeSet.find(dfa.vertex[i]) == dfa.endNodeSet.end()){
            noFinalState.stateSet.insert(dfa.vertex[i]);
        }
    }
    set<Group> res;
    res.insert(finalState);
    res.insert(noFinalState);
    // 遍历每一个DFA的状态转移字符，进行分割
    for(set<char>::iterator i = dfa.transChar.begin(); i != dfa.transChar.end(); i++){
        char ch = *i;
        split(dfa, res, ch);
    }
    // 将分割好的res重命名
    map<char, char> dic = rename(res);
    dic['-'] = '-';
    // 生成minDfa
    Matrix minDfa;
    // 节点数统计-res的长度
    minDfa.nodeCount = res.size();
    // 构造节点名称-每个Group中的set的首元素
    minDfa.vertex = new char[minDfa.nodeCount];
    int cur = 0;
    for(set<Group>::iterator i = res.begin(); i != res.end(); i++){
        Group t = *i;
        minDfa.vertex[cur++] = *t.stateSet.begin();
    }
    // 状态转移字母表-不变
    minDfa.transChar = dfa.transChar;
    // 转移矩阵-遍历节点集，读取旧矩阵对应的节点名称的值，并重命名
    // 终止节点集-遍历节点集，判断节点名称是否在终态集里面，是的话加进去
    minDfa.transArray = new char*[Maxnum];
    for(int cur = 0; cur < minDfa.nodeCount; cur++){// 对每一个节点构造新矩阵
        minDfa.transArray[cur] = new char[Maxnum];
        char node = minDfa.vertex[cur];
        // 如果是终态集，则加进去
        if(dfa.endNodeSet.find(node) != dfa.endNodeSet.end()){
            minDfa.endNodeSet.insert(node);
        }
        // 找到对应node的原映射关系
        for(int i = 0; i < dfa.nodeCount; i++){
            if(dfa.vertex[i] == node){
                // 遍历列依次放入
                int col = 0;// 指向minDfa的列
                for(int col0 = 0; col0 < dfa.transChar.size(); col0++){
                    char v = dfa.transArray[i][col0];
                    map<char, char>::iterator iter = dic.find(v);
                    if(iter != dic.end()){
                        minDfa.transArray[cur][col++] = iter->second;
                    }
                }
                break;
            }
        }
    }
    // 边数统计-等于矩阵节点数*转移符数
    minDfa.edgeCount = minDfa.nodeCount * minDfa.transChar.size();
    // 开始节点-和dfa一样
    minDfa.starNode = dfa.starNode;
    return minDfa;
}

// 遍历最小DFA矩阵
void visitMinDfa(Matrix minDfa)
{
    // 输出字符
    cout<<"\t";
    for(set<char>::iterator i = minDfa.transChar.begin(); i != minDfa.transChar.end(); i++){
        cout<<*i<<"\t";
    }
    cout<<endl;
    // 输出矩阵
    for(int i = 0; i < minDfa.nodeCount; i++){
        cout<<minDfa.vertex[i]<<"\t";
        for(int j = 0; j < minDfa.transChar.size(); j++){
            cout<<minDfa.transArray[i][j]<<"\t";
        }
        cout<<endl;
    }
    // 输出起点
    cout<<"起点："<<endl;
    cout<<minDfa.starNode<<endl;
    // 输出终态
    cout<<"终态有："<<endl;
    for(set<char>::iterator i = minDfa.endNodeSet.begin(); i != minDfa.endNodeSet.end(); i++){
        cout<<*i<<"\t";
    }
}

void dfa2MinDfa(Matrix dfa)
{
    Matrix minDfa = getMinDfa(dfa);
    // 输出minDFA矩阵
    visitMinDfa(minDfa);
}

//-------------------------------------------------CCB end--------------------------------------

int main()
{
    // 以下测试用例通过：
    // (a|b)*b
    // (a|b)*abb
    // a(a|b)*
    // abb(a)*
    cout<<"------------------------表达式求NFA-------------------------------"<<endl;
    Skips_graph nfa = exp2Nfa();
    cout<<"------------------------NFA转DFA----------------------------------"<<endl;
    Matrix dfa = nfa2Dfa(nfa);
    cout<<"------------------------最小化DFA---------------------------------"<<endl;
    dfa2MinDfa(dfa);

}
