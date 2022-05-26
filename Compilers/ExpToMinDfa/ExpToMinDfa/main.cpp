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
//-------------------------------------------------���ʽתNFA--------------------------------------
typedef struct Node{//���ڵ�
    char type;
    char Char;
    Node *lc;
    Node *rc;
    int start_S;
    int end_S;
}*Tree_root;

void default_Node(Node* N){//��ʼ�����ڵ�
    N->lc = N->rc = NULL;
}

Node* default_L_Node(char c){//��װҶ�ӽڵ�
    Node* N = new Node;
    default_Node(N);
    N->Char = c;
    N->type = 'l';//�����ڵ����������Ϊl
    return N;
}

int Bvisit_Tree(Tree_root Tree){//������������
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

Node* default_R_Node(char type,Node* lchild,Node* rchild){//��װ���ڵ�
    Node* N = new Node;
    default_Node(N);
    N->type = type;
    N->lc = lchild;
    N->rc = rchild;
    return N;
}

typedef struct op_Stack{//���������ջ
    char* top;
    char* bottom;
    char St[Maxnum];//��������Ϊջ
    };

void Init_op_Stack(op_Stack &S){//�Է���ջ���г�ʼ��
    S.top = S.bottom = S.St;
}

void push_op(op_Stack &S,char a){//��ջ
    *(S.top)=a;
    S.top++;
}

char pop_op(op_Stack &S){//��ս
    S.top--;
    return *S.top;
}

char top_op(op_Stack &S){//����ջ��Ԫ��
    return *(S.top-1);
}

typedef struct yufa_Stack{//���������ջ
    int top;
    int bottom;
    Node* St[Maxnum];
    };

void Init_yufa_Stack(yufa_Stack &S){//��ʼ��������ջ
    S.top=S.bottom=0;
}

void push_yufa(yufa_Stack &S,Node* c){//��ջ
    S.St[S.top] = c;
    S.top++;
}

Node* pop_yufa(yufa_Stack &S){//��ս
    S.top--;
    return S.St[S.top];
}

int o_youxianji(char o){//�жϲ����������ȼ�
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
        return 0; //���ǲ�����
    }
}

bool if_cat(char l,char r){//�ж��Ƿ�������������

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

string cin_Regular_Exp(){//����������ʽ
    cout<<"������������ʽ��";
    string R_exp;
    cin>>R_exp;
    R_exp.insert(0,"#");
    R_exp.append("#");  //���������
    int l=0,r=1;
    while(R_exp[r]!='#'){    //��������ʽ����cat���ӷ�
        if(if_cat(R_exp[l],R_exp[r]))
            R_exp.insert(l+1,"+");
        l++;r++;}
    return R_exp;
}

bool if_high(char odd_op,char new_op){//�Ƚ���������ȼ�
    int odd_=o_youxianji(odd_op);
    int new_=o_youxianji(new_op);
    if(new_>odd_) return 1;
    return 0;
}

Node* Connect_Nodes(char op,yufa_Stack& char_st){//������������ӽڵ�
    switch ( op ){
    case  '|': //˫Ŀ�����
        return default_R_Node(op,pop_yufa(char_st),pop_yufa(char_st));
    case  '+': //˫Ŀ�����
        return default_R_Node(op,pop_yufa(char_st),pop_yufa(char_st));;
    case  '*': //��Ŀ�����
        return default_R_Node(op,pop_yufa(char_st),NULL);
    }
}

Tree_root Yufa_Tree(string exp){//�����﷨��
    op_Stack op_st;//����ջ
    yufa_Stack char_st;//�﷨ջ
    Init_op_Stack(op_st);
    Init_yufa_Stack(char_st);//��ʼ���﷨ջ�ͷ���ջ
    push_op(op_st,exp[0]);
    for(int i = 1; i < exp.size()-1; i++){
        char c = exp[i];
        if(!o_youxianji(c)){
            push_yufa(char_st,default_L_Node(c));
        }
        else{
            if(c == '('){
                push_op(op_st,'(');//����ǣ���ֱ����ջ
               }
            else if(c == ')'){//������������֮��ջ���������
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
                while(!if_high(top_op(op_st),c)){//ѭ���Ƚ�
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


int add_find_char(char *A,char a,int& size_){//���ַ������м�����Ԫ��,���Ѿ����ڣ��򷵻�������
    if(size_==0) {A[0]=a;size_++;return Maxnum;}
    else{
        int i;
        for(i=0;i<size_;i++)
            if(*(A+i)==a) return i;  //�ҵ��� �����
        A[i]=a;size_++;
        return Maxnum; //û�ҵ������
    } }
//�õ�������ʽ�����շ���
int Get_absorb_char(string exp,char *ab_chars){
    int char_size = 0;
    for(int i=0;i<exp.size();i++){
        if(!o_youxianji(exp[i])) //���ǲ�����
            add_find_char(ab_chars,exp[i],char_size);}
    return char_size;
}
//״̬��
typedef struct states_set{
    int st_set[Maxnum];
    int length=0;};
//ͼ�ľ����ʾ
typedef struct Skips_graph{
    int S_size;//״̬��
    int ab_char_size; //�����ַ�������
    char absorb_char[Maxnum]; //�����ַ�
    int Skips[Maxnum][Maxnum]; //״̬��ת
    int empty_Skips[Maxnum][Maxnum]; //��״̬��ת ÿ�е�һ�зŸ�״̬�Ŀ���ת��
    int start_S; //���
    int end_S; //�յ�(��nfa���ԣ�ֻ��һ���յ�)
    states_set end_set; //�յ㼯(����dfa���ԣ��ж���յ�)
};
//��ʼ��״̬ת�ƾ���
void default_graph(Skips_graph &S,char *ab_char,int ab_char_size){
    S.S_size=0;
    S.ab_char_size=ab_char_size;
    for(int i=0;i<ab_char_size;i++)
        S.absorb_char[i]=ab_char[i];
    for(int i=0;i<Maxnum;i++)
        for(int j=0;j<Maxnum;j++)
            S.Skips[i][j]=Maxnum;   //��ʾ����ת
    for(int i=0;i<Maxnum;i++)
        S.empty_Skips[i][0]=0;   //����ת����Ϊ0
    }
//����µ�״̬��ת ͼ ��� �յ� �����ַ�
void add_Skips(Skips_graph& S,int B,int E,char c){
    int ab_char=add_find_char(S.absorb_char,c,S.ab_char_size);
    S.Skips[B][ab_char]=E;
    }
void add_empty_Skips(Skips_graph& S,int B,int E){
    S.empty_Skips[B][0]=S.empty_Skips[B][0]+1; //����ת����һ
    int size_=S.empty_Skips[B][0];
    S.empty_Skips[B][size_]=E;}
//����Ҷ�ӽڵ�״̬ת��
void Leaf_Skip(Skips_graph& S,Node& N){
    N.start_S=S.S_size;S.S_size++;
    N.end_S=S.S_size;S.S_size++;
    add_Skips(S,N.start_S,N.end_S,N.Char); //���ʼĩ״̬ת��
    }
//�����״̬ת��
void root_Skip(Skips_graph& S,Node& N){
    //�������
    if(N.type=='|'){
        N.start_S=S.S_size;S.S_size++;
        N.end_S=S.S_size;S.S_size++;
        add_empty_Skips(S,N.start_S,N.lc->start_S);
        add_empty_Skips(S,N.start_S,N.rc->start_S);
        add_empty_Skips(S,N.lc->end_S,N.end_S);
        add_empty_Skips(S,N.rc->end_S,N.end_S);}
    //�հ�����
    else if(N.type=='*'){
        N.start_S=S.S_size;S.S_size++;
        N.end_S=S.S_size;S.S_size++;
        add_empty_Skips(S,N.start_S,N.lc->start_S);
        add_empty_Skips(S,N.start_S,N.end_S);
        add_empty_Skips(S,N.lc->end_S,N.lc->start_S);
        add_empty_Skips(S,N.lc->end_S,N.end_S);}
    //���ӹ���
    else if(N.type=='+'){
        N.start_S=N.lc->start_S;
        N.end_S=N.rc->end_S;
        add_empty_Skips(S,N.lc->end_S,N.rc->start_S);}
    else {cout<<"fault!";}
}

//�ݹ鹹��NFA
void NFA(Tree_root Tree, Skips_graph &S, string exp){
    //����״̬ת�ƾ���
    if(Tree->type=='l') //Ҷ��״̬ת��
        Leaf_Skip(S,*Tree);
    else
    {
        NFA(Tree->lc,S,exp); //����������״̬ת��
        if(Tree->rc!=NULL) NFA(Tree->rc,S,exp); //����������״̬ת��
        root_Skip(S,*Tree);//�����״̬ת��}
}}

void NFA_(Tree_root Tree, Skips_graph &S, string exp){
    //�õ����շ���
    char ab_chars[Maxnum]; //������ʽ�е����շ���
    int ab_chars_size=Get_absorb_char(exp,ab_chars);
    //��ʼ��״̬ת��(NFA)
    default_graph(S,ab_chars,ab_chars_size);
    //����
    NFA(Tree,S,exp);
    S.start_S=Tree->start_S;S.end_S=Tree->end_S;}
//��NFA
void vist_Graph(Skips_graph &S,string type){
    cout<<"״̬��ת����:"<<endl;
    cout<<"״̬";
    for(int i=0;i<S.ab_char_size;i++)
        cout<<setw(6)<<S.absorb_char[i];
    cout<<setw(8)<<"����ת";
    cout<<endl;
    for(int i=0;i<S.S_size;i++)
        {cout<<setw(3)<<i;
        for(int j=0;j<S.ab_char_size;j++)
            if(S.Skips[i][j]==Maxnum) cout<<setw(6)<<"-";
            else cout<<setw(6)<<S.Skips[i][j];
        //�������ת
        cout<<setw(6)<<" ";
        for(int j=0;j<S.empty_Skips[i][0];j++)
            cout<<S.empty_Skips[i][j+1]<<" ";
        cout<<endl;}
    cout<<"���:"<<S.start_S<<" ";
    if(type=="nfa") cout<<"�յ�:"<<S.end_S<<" ";
    else{
        cout<<"�յ�:";
        for(int i=0;i<S.end_set.length;i++)
            cout<<S.end_set.st_set[i]<<" ";
    }
    cout<<endl;}

Skips_graph exp2Nfa()
{
  string exp=cin_Regular_Exp();
  cout<<exp<<endl;
  //�����﷨��
  Node* Tree=Yufa_Tree(exp);
  //�����﷨��
  cout<<"��������﷨��: ";
  Bvisit_Tree(Tree);
  cout<<endl;
  //����NFA
  cout<<endl<<"********************Thompson�㷨����NFA***********************"<<endl;
  Skips_graph nfa;
  NFA_(Tree,nfa,exp);
  vist_Graph(nfa,"nfa");
  return nfa;
}
//-------------------------------------------------ZZC end---------------------------------------


//-------------------------------------------------LLC start-------------------------------------
//-------------------------------------------------NFA->DFA--------------------------------------
// �洢DFA�ľ���ṹ
struct Matrix
{
    char *vertex;           //�ڵ�����
    set<char>  transChar;    //״̬ת����ĸ��
    char** transArray;            //ת�ƾ���
    int  edgeCount;           //����ͳ��
    int  nodeCount;           //�ڵ���ͳ��
    char  starNode;         //��ʼ�ڵ�
    set<char>  endNodeSet;   //��ֹ�ڵ㼯
};
//��״̬�������Ԫ��
bool add_s_to_set(states_set &Set,int s){
    for(int i=0;i<Set.length;i++){
        if(Set.st_set[i]==s) return 0;
    }
    Set.st_set[Set.length]=s;
    Set.length++;
    return 1;
}
//�󼯺ϵĿձհ� ����ת
void empty_closure(states_set &Set,int empty_Skips[][Maxnum]){
    for(int i=0;i<Set.length;i++){
        int p=Set.st_set[i];
        for(int j=0;j<empty_Skips[p][0];j++){
            add_s_to_set(Set,empty_Skips[p][j+1]);}
        }
    cout<<"�հ���"<<endl;
    for(int i=0;i<Set.length;i++) cout<<Set.st_set[i]<<" ";cout<<endl;
}

//����ӵ�ǰ״̬������������a�ַ����ܵ�����һ��״̬��
void Smove(states_set Set,states_set &next_Set,int absorb_char,Skips_graph &nfa){
    //��ÿ��״̬�����㾭��a�õ�����״̬
    for(int i=0;i<Set.length;i++)
    {   int nfa_i=Set.st_set[i];  //��nfa�е�״̬
        if(nfa.Skips[nfa_i][absorb_char]!=Maxnum) //����ת
            add_s_to_set(next_Set,nfa.Skips[nfa_i][absorb_char]);}
}

//�жϵ�ǰ״̬���Ƿ���ڣ��������򷵻���״̬�����������򷵻�0
int if_exist(states_set *Sets,states_set Set,int sets_length){
    //�ҵ�������ȵĽ��бȶ�
    int T; //�ж��Ƿ�����ͬ��״̬��
    for(int i=0;i<sets_length;i++){
        if(Sets[i].length==Set.length){
            T=1; //�ȼ���ü�����
            for(int j=0;j<Set.length;j++){
                int t=0; //�ж��Ƿ��ҵ���ͬԪ��
                for(int k=0;k<Sets[i].length;k++)
                    if(Set.st_set[j]==Sets[i].st_set[k]){t=1;break;}
                if(t==0) {T=0;break;} //�Ҳ������ü��ϲ���
                }
            if(T==1) return i;
        }}
    return 0;
}

//NFAתΪDFA
void DFA(Skips_graph &nfa,Skips_graph &dfa,states_set *sets){
    for(int i=0;i<dfa.S_size;i++){
        //���㵱ǰ״̬����ת״̬�� cΪ�����ַ�
        for(int c=0;c<dfa.ab_char_size;c++){
            states_set next_set;
            Smove(sets[i],next_set,c,nfa);//����Smove(j,char)
            if(next_set.length==0) continue; //��״̬�����ո��ַ�
            empty_closure(next_set,nfa.empty_Skips);//����Smove�Ŀձհ�
            //�жϸ�״̬���Ƿ��Ѿ�����,�������������Ϊ��״̬������dfa
            int S=if_exist(sets,next_set,dfa.S_size);
            if(!S){
                S=dfa.S_size;
                sets[S]=next_set;
                dfa.S_size++;
            }
            //��dfa����ӵ�ǰ״̬�����ַ�����ת
            dfa.Skips[i][c]=S;
        }
    }
}

//�жϼ������Ƿ���ĳ��Ԫ��
bool find_s(states_set set_,int s){
    for(int i=0;i<set_.length;i++)
        if(set_.st_set[i]==s) return 1;
    return 0;
}

void DFA_(Skips_graph &nfa,Skips_graph &dfa){
    //��ʼ��dfa
    default_graph(dfa,nfa.absorb_char,nfa.ab_char_size);
    states_set sets[Maxnum];//�¾�״̬��
    //������ʼ״̬�Ŀձհ�
    states_set begin_set;
    add_s_to_set(begin_set,nfa.start_S);
    empty_closure(begin_set,nfa.empty_Skips);
    //����ʼ״̬����ΪDFA����ʼ״̬����
    dfa.start_S=0;
    //����ʼ״̬������״̬������״̬�Ķ��ձ���
    sets[dfa.S_size]=begin_set;
    dfa.S_size++;
    DFA(nfa,dfa,sets);
    //�ҵ�dfa�������յ�״̬
    int end_s=nfa.end_S;
    for(int i=0;i<dfa.S_size;i++){
        if(find_s(sets[i],end_s)){
            add_s_to_set(dfa.end_set,i);
        }}
}
Matrix Skips_graph2Matrix(Skips_graph dfa){
    Matrix m;
    m.edgeCount=0;
    //�ڵ���ת��
    m.nodeCount=dfa.S_size;
    m.transArray = new char*[m.nodeCount];
    m.vertex = new char[m.nodeCount];
    //ת������ת�����ڵ�����ת��������ת��
    for(int i=0;i<dfa.S_size;i++){
        m.vertex[i]=i + '0';
        m.transArray[i] = new char[dfa.ab_char_size];
        for(int j=0;j<dfa.ab_char_size;j++){
            if(dfa.Skips[i][j] == Maxnum) m.transArray[i][j]='-';
            else m.transArray[i][j]=dfa.Skips[i][j]+'0';
        }
    }
    //�����ַ�ת��
    for(int c=0;c<dfa.ab_char_size;c++){
            m.transChar.insert(dfa.absorb_char[c]);
    }
    //��ʼ�ڵ�ת��
    m.starNode=dfa.start_S+'0';
    //��ֹ�ڵ�ת��
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
//-------------------------------------------------��С��DFA--------------------------------------
// ״̬��
struct Group
{
    int pk;
    set<char> stateSet;
    // �ȽϷ�����
    bool operator <(const Group & g) const
	{
		return pk<g.pk;
	}

};

int countPk = 0;// Group������������

// ��ȡ��ǰ״̬ͨ���ַ�chת������һ��״̬��
Group getNextStateGroup(Matrix dfa, char state, char ch, set<Group> res)
{
    char nextState;
    // �ҵ�state��Ӧ����
    int row;
    for(int i = 0; i < dfa.nodeCount; i++){
        if(dfa.vertex[i] == state){
            row = i;
            break;
        }
    }
    int col = 0;
    // �ҵ�ch��Ӧ����
    for(set<char>::iterator it=dfa.transChar.begin();it!=dfa.transChar.end();it++){
        if(*it == ch){
            break;
        }
        col++;
    }
    // ��ȡֵ
    nextState = dfa.transArray[row][col];
    // �ҵ���Ӧ����
    for(set<Group>::iterator it=res.begin();it!=res.end();it++){
        Group t = *it;
        if(t.stateSet.find(nextState) != t.stateSet.end()){// �ҵ���Ӧ�飬ֱ�ӷ���
            return t;
        }
    }
    // ���û�ҵ�����һ������Ϊ-999��Group
    Group err;
    err.pk = -999;
    return err;
}

// �ָ��
void split(Matrix dfa, set<Group> & res, char ch)
{
    // ����һ�������ȷ����ʼ��
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
            // �ҵ���ǰ״̬ͨ���ַ�chת�Ƶ�״̬��
            Group nextGroup = getNextStateGroup(dfa, curState,ch, res);
            sMap[nextGroup].insert(curState);
        }
        // ���ת����ӳ����״̬�飬������
        if(sMap.size() != 1){
            res.erase(curGroup);
            map<Group, set<char> >::iterator iter;
            for(iter = sMap.begin(); iter != sMap.end(); iter++) {
                set<char> tSet = iter->second;
                Group t;
                t.pk = countPk++;
                t.stateSet = tSet;
                // res����
                res.insert(t);
                // queue����
                q.push(t);
            }
        }
    }


}

// ��res������,����һ��mapӳ��
map<char, char> rename(set<Group> res)
{
    // ͨ��map key=ԭ��״̬ value=��������״̬
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

// ��ȡ��СDFA
Matrix getMinDfa(Matrix dfa)
{
    // ����������̬��
    Group finalState;
    finalState.pk = countPk++;
    for(set<char>::iterator i = dfa.endNodeSet.begin(); i != dfa.endNodeSet.end(); i++){
        finalState.stateSet.insert(*i);
    }
    // �������з���̬��
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
    // ����ÿһ��DFA��״̬ת���ַ������зָ�
    for(set<char>::iterator i = dfa.transChar.begin(); i != dfa.transChar.end(); i++){
        char ch = *i;
        split(dfa, res, ch);
    }
    // ���ָ�õ�res������
    map<char, char> dic = rename(res);
    dic['-'] = '-';
    // ����minDfa
    Matrix minDfa;
    // �ڵ���ͳ��-res�ĳ���
    minDfa.nodeCount = res.size();
    // ����ڵ�����-ÿ��Group�е�set����Ԫ��
    minDfa.vertex = new char[minDfa.nodeCount];
    int cur = 0;
    for(set<Group>::iterator i = res.begin(); i != res.end(); i++){
        Group t = *i;
        minDfa.vertex[cur++] = *t.stateSet.begin();
    }
    // ״̬ת����ĸ��-����
    minDfa.transChar = dfa.transChar;
    // ת�ƾ���-�����ڵ㼯����ȡ�ɾ����Ӧ�Ľڵ����Ƶ�ֵ����������
    // ��ֹ�ڵ㼯-�����ڵ㼯���жϽڵ������Ƿ�����̬�����棬�ǵĻ��ӽ�ȥ
    minDfa.transArray = new char*[Maxnum];
    for(int cur = 0; cur < minDfa.nodeCount; cur++){// ��ÿһ���ڵ㹹���¾���
        minDfa.transArray[cur] = new char[Maxnum];
        char node = minDfa.vertex[cur];
        // �������̬������ӽ�ȥ
        if(dfa.endNodeSet.find(node) != dfa.endNodeSet.end()){
            minDfa.endNodeSet.insert(node);
        }
        // �ҵ���Ӧnode��ԭӳ���ϵ
        for(int i = 0; i < dfa.nodeCount; i++){
            if(dfa.vertex[i] == node){
                // ���������η���
                int col = 0;// ָ��minDfa����
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
    // ����ͳ��-���ھ���ڵ���*ת�Ʒ���
    minDfa.edgeCount = minDfa.nodeCount * minDfa.transChar.size();
    // ��ʼ�ڵ�-��dfaһ��
    minDfa.starNode = dfa.starNode;
    return minDfa;
}

// ������СDFA����
void visitMinDfa(Matrix minDfa)
{
    // ����ַ�
    cout<<"\t";
    for(set<char>::iterator i = minDfa.transChar.begin(); i != minDfa.transChar.end(); i++){
        cout<<*i<<"\t";
    }
    cout<<endl;
    // �������
    for(int i = 0; i < minDfa.nodeCount; i++){
        cout<<minDfa.vertex[i]<<"\t";
        for(int j = 0; j < minDfa.transChar.size(); j++){
            cout<<minDfa.transArray[i][j]<<"\t";
        }
        cout<<endl;
    }
    // ������
    cout<<"��㣺"<<endl;
    cout<<minDfa.starNode<<endl;
    // �����̬
    cout<<"��̬�У�"<<endl;
    for(set<char>::iterator i = minDfa.endNodeSet.begin(); i != minDfa.endNodeSet.end(); i++){
        cout<<*i<<"\t";
    }
}

void dfa2MinDfa(Matrix dfa)
{
    Matrix minDfa = getMinDfa(dfa);
    // ���minDFA����
    visitMinDfa(minDfa);
}

//-------------------------------------------------CCB end--------------------------------------

int main()
{
    // ���²�������ͨ����
    // (a|b)*b
    // (a|b)*abb
    // a(a|b)*
    // abb(a)*
    cout<<"------------------------���ʽ��NFA-------------------------------"<<endl;
    Skips_graph nfa = exp2Nfa();
    cout<<"------------------------NFAתDFA----------------------------------"<<endl;
    Matrix dfa = nfa2Dfa(nfa);
    cout<<"------------------------��С��DFA---------------------------------"<<endl;
    dfa2MinDfa(dfa);

}
