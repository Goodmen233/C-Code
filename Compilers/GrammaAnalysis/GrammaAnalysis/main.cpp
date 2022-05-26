#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cctype>
#include <algorithm>
#include <iomanip>
#include <stack>
#define MAXNUM 999
using namespace std;


// ������������������������ ע���� $ ��ʾ �� ������������������������
// �ķ��ı�ʾ----------------------------------------------------
struct Cfg{
    set<string> n;// ���ս������
    set<string> t;// �ս������
    map<string, set<string> > p;// ����ʽ����
    string s;// ��ʼ����
};
// ˵����
// ����ʽ����p��keyΪ����ʽ��
// ����ʽ����p��valueΪ����ʽ�Ҳ�(ʡ����|)

// first��follow���ı�ʾ----------------------------------------
map<string, set<string> > first;
map<string, set<string> > follow;
// Ԥ�������ı�ʾ---------------------------------------------
string m[MAXNUM][MAXNUM];


// --------------------------------------------------ZZC start--------------------------------------
list< list<string> > gramRule;           //���ڴ�����е��ķ�����
fstream infile;
fstream file;                          //��д�������Դ򿪵��ļ��ɽ��ж�д����
vector<string> gramBuf;                  //�ݴ���ļ��ж�ȡ���ķ�����
map<string, bool> nonSymExist;          //���ڱ�Ǹ��󲿷��ս���Ƿ��Ѵ���
int n = 0;
//������������
void to_LL1_rule();
void read_file();
void split_gram();
void remove_left_recursion();
void remove_left_common_factor();
void merge();
void output_all();
Cfg creatCfg(char* basePath);
void createTerminator();
vector<string> splitWithStl();

vector<string> splitWithStl(const string str, const string part)
{
	vector<string> resVec;
	if ("" == str)//��Ϊ�գ��򷵻�ԭ�ַ���
	{
		return resVec;
	}
	//�����ȡ���һ������
	string strs = str + part;
	size_t pos = strs.find(part);//find�����ķ���ֵ�����ҵ��ָ������طָ�����һ�γ��ֵ�λ�ã����򷵻�npos�˴���size_t������Ϊ�˷���λ��
	size_t size = strs.size();
	while (pos != string::npos)
	{
		string x = strs.substr(0, pos);//substr������������ַ���
		resVec.push_back(x);
		strs = strs.substr(pos + 1, size);
		pos = strs.find(part);
	}
	return resVec;
}

void to_LL1_rule(Cfg & cfg, char* basePath) {
    string tmp;
    infile.open(basePath);

    getline(infile, tmp);
    vector<string> terminates = splitWithStl(tmp, " ");
    for(int i = 0; i < terminates.size(); i++){
        cfg.t.insert(terminates[i]);
    }

    read_file();
    split_gram();
    remove_left_recursion();
    remove_left_common_factor();
}

void read_file() {
    string tempStr;
    while (getline(infile, tempStr)) {
        if (tempStr.length() > 0) {
            gramBuf.push_back(tempStr);
        }
    }
}

void split_gram() {
    int gramSum = gramBuf.size();
    int strLen = -1, start = -1;
    string tempStr = "";
    for (int i = 0;i < gramSum; i++) {
        tempStr = gramBuf[i];
        strLen = tempStr.size();
        //cout << "tempStr: " << tempStr << endl;
        list<string> tempList;                             //����һ����ʱ�����Դ��ÿ�����ս�����Ҳ�����ʽ

        //Ѱ���Ҳ�����ʽ�Ŀ�ʼ��־ ->
        for (int j = 0;j < strLen;j++) {
            if (tempStr[j] == '>') {
                tempList.push_back(tempStr.substr(0, j - 1));       //�����ķ��󲿵ķ��ս��
                nonSymExist[tempStr.substr(0, j - 1)] = true;
                start = j + 1;
                break;
            }
        }

        //����|���ÿ���Ҳ�����ʽ
        for (int j = start;j < strLen;j++) {
            if (tempStr[j] == '|') {
                tempList.push_back(tempStr.substr(start, j - start));
                start = j + 1;
            }
        }
        //�������һ���Ҳ�����ʽ
        //cout << "���һ���Ҳ�����ʽ: " << tempStr.substr(start) << endl;
        tempList.push_back(tempStr.substr(start));
        gramRule.push_back(tempList);                        //���²�ֺõ��ķ���������������
    }
    cout << "------------------------" << endl;
    cout << "��ֺõ��ķ�����Ϊ: " << endl;
    output_all();
    cout << "------------------------" << endl;
}

/*
 * ������ݹ�
 */
void remove_left_recursion() {
    list< list<string> >::iterator it_i, it_j;                   //�ֱ�Ϊ���ѭ�����ڲ�ѭ������ʱ����
    list<string>::iterator it_k, it_temp;

    for (it_i = gramRule.begin(); it_i != gramRule.end(); it_i++) {
//        for (it_j = gramRule.begin(); it_j != it_i; it_j++) {
//            it_k = it_i -> begin();
//            /**
//             * �����������ѭ����Ӧ��������Ҳ�����ʽ�������Ƿ���Ai->Ajr��ʽ���Ҳ�����ʽ
//             */
//            for (it_k++; it_k != it_i -> end();) {
//                string searchStr = *it_k;
//                string srcStr = it_j -> front();
//                bool isfind = false;
//                if (isupper(searchStr[0])) {            //�Ҳ�����ʽ������ĸΪ���ս��ʱ,�ſ��ܳ�����ݹ�����
//                    for (int i = 0;i < searchStr.length();i++) {
//                        if (isupper(searchStr[i])) {
//                            if(searchStr.substr(i, srcStr.size()) == srcStr) {  //�ҵ���Ai->Ajr��ʽ���Ҳ�����ʽ,�����滻Ai���Ҳ�����ʽ
//                                isfind = true;
//                                it_temp = it_j -> begin();
//                                // ��Aj���Ҳ�����ʽ�滻Ai->Ajr��ʽ���Ҳ�����ʽ
//                                for (it_temp++; it_temp != it_j -> end(); it_temp++) {
//                                    // ǰ�巨
//                                    it_i -> insert(it_k, searchStr.substr(0, i) + *it_temp + searchStr.substr(i+1));
//                                }
//                                it_k = it_i -> erase(it_k);              //ɾ���滻ǰ�Ľ��,����ɾ�����ĺ�һ�����ĵ�����
//                            }
//                        }
//                    }
//                }
//                if (!isfind) {              //û���ҵ����滻�ķ��ս��,�����������,����������
//                    it_k++;
//                }
//            }
//        }
        /**
         * �ж��滻��Ĺ����Ƿ����ֱ����ݹ�,������,����ݹ�ת�����ҵݹ�
         */
        bool hasRecur = false;
        it_temp = it_i -> begin();
        vector<string> tempStrs;                     //���ڴ����ݹ鲿�ֵ��Ҳ�����ʽ,��E -> E + T,����+T
        for (it_temp++; it_temp != it_i -> end(); ) {       //�����Ƿ���E -> E...��ʽ����ݹ�
            if (it_temp -> substr(0, (it_i -> front()).size()) == it_i -> front()) {
                hasRecur = true;                                    //��ʾ�����д�����ݹ�
                tempStrs.push_back(it_temp -> substr((it_i -> front()).size()));
                it_temp = it_i -> erase(it_temp);               //ɾ��������ݹ�Ľ��
            } else {
                it_temp++;
            }
        }
        if (hasRecur) {             //���������ݹ�,����ת�����ҵݹ��ٴ����ķ�������

            it_temp = it_i -> begin();

            for (it_temp++; it_temp != it_i -> end(); it_temp++) {      //�����µķ��ս��
                if(*it_temp == "$"){
                    *it_temp = it_i -> front() + "'";
                }else{
                    *it_temp += it_i -> front() + "'";
                }
            }

            for (int i = 0; i < tempStrs.size(); i++) {
                tempStrs[i] += it_i -> front() + "'";
            }

            list<string> newGram;                                   //�����µ��ҵݹ����
            string tempnonsym = it_i -> front() + "'";
            while (nonSymExist[tempnonsym]) {
                tempnonsym += "'";                              //�÷��ս���Ѿ�����,���Ҫ���µ��ս��
            }
            newGram.push_back(tempnonsym);
            nonSymExist[tempnonsym] = true;
            for (int i = 0; i < tempStrs.size(); i++) {
                newGram.push_back(tempStrs[i]);
            }
            newGram.push_back("$");
            list< list<string> >::iterator insert_temp = it_i;
            gramRule.insert(++insert_temp, newGram);
        }
    }
    cout << "------------------------" << endl;
    cout << "������ݹ����ķ�����Ϊ: " << endl;
    output_all();
    cout << "------------------------" << endl;
}

/**
 * ��ȡ������
 */
void remove_left_common_factor() {
    list< list<string> >::iterator it_i, it_j;                   //�ֱ�Ϊ���ѭ�����ڲ�ѭ������ʱ����
    list<string>::iterator it_k, it_temp;
    bool isrecursion = false;
    /**
     * ��ȡ������
     */
    for (it_i = gramRule.begin(); it_i != gramRule.end();it_i++) {
        it_k = it_i -> begin();
        for (it_k++; it_k != it_i -> end(); it_k++) {
            vector<string> tempStrs;                     //���ڴ���������Ҳ�����ʽ�����������ʣ�ಿ��
            bool issamefind = false;
            map<string, bool> tempsign;
            it_temp = it_k;
            int commonlen = it_k -> size();
            for (it_temp++; it_temp != it_i -> end(); it_temp++) {
                if ((*it_temp)[0] == (*it_k)[0]) {
                    bool issubstr = true;
                    for (int i = 0; i < min(it_temp -> size(), it_k -> size()); i++) {           //�����ӳ��ȿ��ܴ���1
                        if ((*it_temp)[i] != (*it_k)[i]) {
                            issubstr = false;
                             //ȡ������������̵Ĳ�����ȡ
                            commonlen = min(commonlen, i);
                            break;
                        }
                    }

                    if (issubstr) {         //������������,˵������һ������ʽ������һ������ʽ���Ӵ�
                        int sublen = min(it_temp -> size(), it_k -> size());
                        commonlen = min(commonlen, sublen);
                    }
                    issamefind = true;
                    tempsign[*it_temp] = true;         //���ڱ��Ҫ��ȡ�����ӵ��Ҳ�����ʽ
                    //cout << "*it_temp = " << *it_temp << endl;
                }
            }
            if (issamefind) {
                //��ȡ�����Ӳ���
                for (list<string>::iterator iter = it_k; iter != it_i -> end();) {
                    if (tempsign[*iter]) {
                        tempStrs.push_back(iter -> substr(commonlen) == "" ? "$" : iter -> substr(commonlen));
                        iter = it_i -> erase(iter);               //ɾ���ý��,����������ָ�����
                    } else {
                        iter++;
                    }
                }

                list<string> newGram;                                               //�����µķ��ս������
                string tempnonsym = it_i -> front() + "'";
                while (nonSymExist[tempnonsym]) {
                    tempnonsym += "'";                              //�÷��ս���Ѿ�����,���Ҫ���µ��ս��
                }

                //���ɹ���Ĺ����Ӳ�����ȡ
                tempStrs.push_back(it_k -> substr(commonlen) == "" ? "$" : it_k -> substr(commonlen));
                //�ɵĹ��������µķ��ս��,ȡmax��ֹԽ����߾ɹ���ǹ����Ӳ�����ʣ��
                it_k -> replace(commonlen, max(tempnonsym.length(), it_k -> size() - commonlen), tempnonsym);
                //cout << "*it_k = " << *it_k << endl;
                newGram.push_back(tempnonsym);
                nonSymExist[tempnonsym] = true;
                for (int i = 0; i < tempStrs.size(); i++) {
                    newGram.push_back(tempStrs[i]);
                }
                list< list<string> >::iterator insert_temp = it_i;
                gramRule.insert(++insert_temp, newGram);                        //ǰ�巨,���ڵ�ǰ���ĺ�һλ
            }
        }
    }

    if (isrecursion) {
        remove_left_common_factor();        //�ݹ�ִ��
    } else {
        cout << "------------------------" << endl;
        cout << "��ȡ�����Ӻ���ķ�����Ϊ: " << endl;
        output_all();
        cout << "------------------------" << endl;
    }
}

void output_all() {
    list< list<string> >::iterator iter;
    for (iter = gramRule.begin(); iter != gramRule.end(); iter++) {
        list<string> tempList = *iter;
        cout << tempList.front() << "->";
        tempList.pop_front();
        while (!tempList.empty()) {
            cout << tempList.front();
            tempList.pop_front();
            if (!tempList.empty()) {
                cout << "|";
            }
        }
        cout << endl;
    }
}

Cfg creatCfg(char* basePath){
    Cfg cfg;
    to_LL1_rule(cfg, basePath);
    list< list<string> >::iterator iter;
    set<string>::iterator it1;
    map<string,set<string> >::iterator it;
    string x;
    iter = gramRule.begin();
    list<string> beginf = *iter;
    cfg.s = beginf.front();
    for (iter = gramRule.begin(); iter != gramRule.end(); iter++) {
        list<string> tempList = *iter;
        cfg.n.insert(tempList.front());
        tempList.pop_front();
    }
    for (iter = gramRule.begin(); iter != gramRule.end(); iter++) {   //������ʽ�洢Ϊmap��ʽ
        list<string> tempList = *iter;
        x = tempList.front();
        tempList.pop_front();
        while (!tempList.empty()) {
            cfg.p[x].insert(tempList.front());
            tempList.pop_front();
        }
    }
   return cfg;
}

// --------------------------------------------------ZZC end--------------------------------------

// --------------------------------------------------LLC start--------------------------------------
// ��ȡ����ʽ�Ҳ���һ��ʽ�ӵĵ�start���ַ��Ľ�ֹλ��
int getStr(Cfg cfg, string str, int start)
{
    if(start >= str.length()){
        return -1;
    }
    // �����ƥ��ԭ��ƥ��
    bool isFirstMatch = false;
    int pos = start;
    string cur = str.substr(start, 1);
    while(pos < str.length()){
        // �����ǰ���ս�� ���� ���ս������������ƥ��
        if(cfg.n.find(cur) != cfg.n.end() || cfg.t.find(cur) != cfg.t.end()){
            isFirstMatch = true;
            cur = cur + str[++pos];
        // ����ǵ�һ�λ�ûƥ���ϣ�����ƥ��
        }else if(!isFirstMatch && (cfg.n.find(cur) == cfg.n.end() && cfg.t.find(cur) == cfg.t.end())){
            cur = cur + str[++pos];
        }else{
            // ��ǰλ��ƥ�䲻�ϣ�������һ��λ��
            return pos - 1;
        }
    }
    return str.length() - 1;
}
// ��ȡ����ʽ�Ҳ���һ��ʽ�ӵĵ�ending���ַ��Ľ�ֹλ���淽��
int NgetStr(Cfg cfg, string str, int ending)
{
    if(ending >= str.length()){
        return -1;
    }
    // �����ƥ��ԭ��ƥ��
    bool isFirstMatch = false;
    int pos = ending;
    string cur = str.substr(ending, 1);
    while(pos > str.length()){
        // �����ǰ���ս�� ���� ���ս������������ƥ��
        if(cfg.n.find(cur) != cfg.n.end() || cfg.t.find(cur) != cfg.t.end()){
            isFirstMatch = true;
            cur = str[pos--] + cur;
        // ����ǵ�һ�λ�ûƥ���ϣ�����ƥ��
        }else if(!isFirstMatch && (cfg.n.find(cur) == cfg.n.end() && cfg.t.find(cur) == cfg.t.end())){
            cur = str[pos--] + cur;
        }else{
            // ��ǰλ��ƥ�䲻�ϣ�������һ��λ��
            return pos + 1;
        }
    }
    return str.length() - 1;
}
//����ս����First��
void Get_First(Cfg cfg,map< string, set<string> > &first,string S){
    //�ҵ����ŵĲ���ʽ����
    set<string> temp = cfg.p.find(S)->second;
    for(set<string>::iterator it=temp.begin();it!=temp.end();it++){
        //�õ�����ʽ
        string Head = *it;
        int start = 0;
        int ending = getStr(cfg,Head,start);
        string head = Head.substr(start, ending - start + 1);
        bool exit=true;
        while(start<Head.length()&&exit){
            //1.�������ʽ�ĵ�һ���ַ�Ϊ�ս����$ ��S->a...
            if(cfg.t.find(head)!=cfg.t.end()||head=="$"){
                //���ս�����뵽���ս��A��First����
                first[S].insert(head);
                exit=false;
            }
            //2.�������ʽ�ĵ�һ���ַ�Ϊ���ս��
            else if(cfg.n.find(head)!=cfg.t.end()){
                //��A��Fisrt��
                Get_First(cfg,first,head);
                //���A��First���п� ��A��First��ȥ���պ��ټ���S��First����
                if(first[head].find("$")!=first[head].end()){
                    for(set<string>::iterator ia=first[head].begin();ia!=first[head].end();ia++){
                        if(*ia!="$"){
                            first[S].insert(*ia);
                        }
                    }
                }
                //��A��First������S��First����
                else{
                    for(set<string>::iterator ia=first[head].begin();ia!=first[head].end();ia++){
                        first[S].insert(*ia);
                    }
                    exit=false;
                }
            }
            //3.���ÿ���ַ���Ϊ���ս���Ҷ�������
            if(ending==Head.length()-1){
                if(first[head].find("$")!=first[head].end()){
                    first[S].insert("$");
                }
            }
            //��ǰ��λ
            start=ending+1;
            ending=getStr(cfg,Head,start);
            head = Head.substr(start, ending - start + 1);
        }
    }
}
//���ķ���First��
void First(Cfg cfg){
    for(set<string>::iterator Q=cfg.n.begin(); Q!=cfg.n.end();Q++){
        Get_First(cfg,first,*Q);
    }
}
//����ս����Follow��
void Get_Follow(Cfg cfg,map< string, set<string> > first,map< string, set<string> > &follow,string S){
    set<string> temp=cfg.p.find(S)->second;
    for(set<string>::iterator it=temp.begin();it!=temp.end();it++){
        //�õ�����ʽ
        string Head = *it;
        int start = 0;
        int ending = getStr(cfg,Head,start);
        string head = Head.substr(start, ending - start + 1);
        start=ending+1;
        ending=getStr(cfg,Head,start);
        string head1 = Head.substr(start, ending - start + 1);
        bool exit = true;
        while(start<Head.length()&&exit){
            //�жϸ��ַ��Ƿ�Ϊ���ս��
            if(cfg.n.find(head)!=cfg.n.end()){
                //�����һλ�ַ����ս��
                if(cfg.t.find(head1)!=cfg.t.end()){
                    //����һλ�ַ����뵽���ַ���Follow����
                    follow[head].insert(head1);
                }
                //�����һλ�ַ��Ƿ��ս��
                else if(cfg.n.find(head1)!=cfg.n.end()){
                    //����һλ�ַ�First��ȥ��$����뵽���ַ���Follow����
                    for(set<string>::iterator ia=first[head1].begin();ia!=first[head1].end();ia++){
                        if(*ia!="$"){
                            follow[head].insert(*ia);
                        }
                    }
                }
            }
            //����S->...A AΪ���ս��
            if(ending==Head.length()-1&&cfg.n.find(head1)!=cfg.n.end()){
                //��S��Follow�����뵽A��Follow����
                for(set<string>::iterator ib=follow[S].begin();ib!=follow[S].end();ib++){
                        follow[head1].insert(*ib);
                }
                //��A��First������$
                while(first[head1].find("$")!=first[head1].end()){
                    //��S��Follow�����뵽A��ǰһλFollow����
                    for(set<string>::iterator ib=follow[S].begin();ib!=follow[S].end();ib++){
                        follow[head].insert(*ib);
                    }
                    //����
                    head1=head;
                    ending = start-1;
                    start = NgetStr(cfg,Head,ending);
                    head=Head.substr(start, ending - start + 1);
                }
                exit=false;
            }
            //��ǰ��λ
            start=ending+1;
            ending=getStr(cfg,Head,start);
            head=head1;
            head1=Head.substr(start, ending - start + 1);
        }

    }
}
//���ķ���Follow��
void Follow(Cfg cfg){
    follow[cfg.s].insert("#");
    for(set<string>::iterator Q=cfg.n.begin(); Q!=cfg.n.end();Q++){
        Get_Follow(cfg,first,follow,*Q);
    }
    //�ٴζ����з��ս����Follow��
    for(set<string>::iterator Q=cfg.n.begin(); Q!=cfg.n.end();Q++){
        Get_Follow(cfg,first,follow,*Q);
    }
    for(set<string>::iterator Q=cfg.n.begin(); Q!=cfg.n.end();Q++){
        Get_Follow(cfg,first,follow,*Q);
    }
}
//��ӡ
void print(Cfg cfg){
    cout<<"-----------First---------Follow---------"<<endl;
    for(set<string>::iterator it=cfg.n.begin();it!=cfg.n.end();it++){
        cout<<setw(2)<<*it<<":"<<setw(8);
        for(set<string>::iterator ifirst=first[*it].begin();ifirst!=first[*it].end();ifirst++){
            cout<<*ifirst<<" ";
        }
        cout<<setw(12);
        for(set<string>::iterator ifollow=follow[*it].begin();ifollow!=follow[*it].end();ifollow++){
            cout<<*ifollow<<" ";
        }
        cout<<endl;
    }
}
// --------------------------------------------------LLC end--------------------------------------

// --------------------------------------------------CCB start--------------------------------------
// ��Ԥ�������
bool getM(Cfg cfg)
{
    // ��ʼ��Ϊ" "
    for(int i = 0; i < cfg.n.size(); ++i){
        for(int j = 0; j < cfg.t.size(); ++j){
            m[i][j] = " ";
        }
    }
    // ���ά�����±��Ӧ
    // ����ÿһ���ս��
    int row = 0;
    for(set<string>::iterator it = cfg.n.begin(); it != cfg.n.end(); it++, row++){
        map<string, string> resultMap;
        string str = *it;// ���ս��
        set<string> rightSet;
        // ��ȡ���ս����Ӧ�Ĳ���ʽ�Ҳ�
        map<string, set<string> >::iterator iter = cfg.p.find(str);
        if(iter != cfg.p.end()){
            rightSet = iter->second;
        }else{
            // ������ʾ
            cout<<str<<"�Ҳ�����Ӧ�Ĳ���ʽ�Ҳ�"<<endl;
            return false;
        }
        // ��������ʽ�Ҳ�
        for(set<string>::iterator it0 = rightSet.begin(); it0 != rightSet.end(); it0++){
            string right = *it0;
            if(right == "$"){
                // ������ת���뵽follow��Ԫ����
                set<string> followChar;
                map<string, set<string> >::iterator iter0 = follow.find(str);
                if(iter0 != follow.end()){
                    followChar = iter0->second;
                }else{
                    // ������ʾ
                    cout<<str<<"�Ҳ�����Ӧ��follow��"<<endl;
                    return false;
                }
                for(set<string>::iterator it1 = followChar.begin(); it1 != followChar.end(); it1++){
                    // ���resultMap�����Ѿ���*it1����ʾ���ڶ����ԣ�������LL(1)�ķ�
                    if(resultMap.find(*it1) != resultMap.end()){
                        cout<<"���ķ����ڶ����ԣ�����LL(1)�ķ�!"<<endl;
                        return false;
                    }
                    resultMap[*it1] = "$";
                }
            }else{
                // ���Ҳ���Ӧ��first����Ӧ��Ԫ�ؼ���
                bool exit$ = true;
                int start = 0;
                int curEnd = 0;
                string cur;
                while(exit$){
                    curEnd = getStr(cfg, right, start);
                    if(curEnd == -1){// ���������һ��Ԫ���ˣ���ʾ�����󲿵�first����
                        // ������ת���뵽follow��Ԫ����
                        set<string> followChar;
                        map<string, set<string> >::iterator iter0 = follow.find(str);
                        if(iter0 != follow.end()){
                            followChar = iter0->second;
                        }else{
                            // ������ʾ
                            cout<<str<<"�Ҳ�����Ӧ��follow��"<<endl;
                            return false;
                        }
                        for(set<string>::iterator it1 = followChar.begin(); it1 != followChar.end(); it1++){
                            // ���resultMap�����Ѿ���*it1����ʾ���ڶ����ԣ�������LL(1)�ķ�
                            if(resultMap.find(*it1) != resultMap.end()){
                                cout<<"���ķ����ڶ����ԣ�����LL(1)�ķ�!"<<endl;
                                return false;
                            }
                            resultMap[*it1] = "$";
                        }
                        break;
                    }
                    cur = right.substr(start, curEnd - start + 1);
                    // ����Ҳ���һ���ַ�Ϊ�ս��,ֱ�ӽ����Ҳ�����
                    if(cfg.t.find(cur) != cfg.t.end()){
                        if(resultMap.find(cur) != resultMap.end()){
                            cout<<"���ķ����ڶ����ԣ�����LL(1)�ķ�!"<<endl;
                            return false;
                        }
                        resultMap[cur] = right;
                        exit$ = false;
                    }else{// �����ǰ���ս����first���пգ����������һ���ַ���first��
                        set<string> firstChar;
                        map<string, set<string> >::iterator it2 = first.find(cur);
                        if(it2 != first.end()){
                            firstChar = it2->second;
                        }else{
                            // ������ʾ
                            cout<<cur<<"�Ҳ�����Ӧ��first��"<<endl;
                            return false;
                        }
                        exit$ = false;
                        // ��first���е�Ԫ�ؼ���
                        for(set<string>::iterator firstIt = firstChar.begin(); firstIt != firstChar.end(); firstIt++){
                            if(*firstIt == "$"){
                                exit$ = true;
                            }else{
                                if(resultMap.find(*firstIt) != resultMap.end()){
                                    cout<<"���ķ����ڶ����ԣ�����LL(1)�ķ�!"<<endl;
                                    return false;
                                }
                                resultMap[*firstIt] = right;
                            }
                        }
                    }
                    start = curEnd + 1;// ������һ���ַ���ʼλ��
                }

            }
        }
        // ͨ��resultMap�����
        int col = 0;
        for(set<string>::iterator it0 = cfg.t.begin(); it0 != cfg.t.end(); it0++, col++){
            str = *it0;
            map<string, string >::iterator iter0 = resultMap.find(str);
            if(iter0 != resultMap.end()){
                m[row][col] = iter0->second;
            }
        }
        // map���
        resultMap.clear();
    }
    return true;
}

// ��ӡԤ�������
void printM(Cfg cfg)
{
    cout<<"----------------------------------Ԥ�������----------------------------------------"<<endl;
    cout<<"\t";
    int skip = -1;
    int count = 0;
    for(set<string>::iterator it = cfg.t.begin(); it != cfg.t.end(); it++){
        if(*it == "$"){
            skip = count;
            continue;
        }
        cout<<*it<<"  \t";
        count++;
    }
    cout<<endl;
    int row = 0;
    for(set<string>::iterator it = cfg.n.begin(); it != cfg.n.end(); it++){
        cout<<*it<<"\t";
        for(int col = 0; col < cfg.t.size(); col++){
            if(col == skip){
                continue;
            }
            cout<<m[row][col];
            if(m[row][col].size() < 8)
                cout<<"\t";
        }
        cout<<endl;
        row++;
    }
}

// ��ȡ��ӦԤ���������ָ���ս��ַ��ͷ��ս��ַ���Ӧ���ַ�����Vector����
vector<string> getVector(Cfg cfg, string n, string t)// n���ս� t�ս�
{
    vector<string> res;
    int row = 0;
    int col = 0;
    for(set<string>::iterator it = cfg.n.begin(); it != cfg.n.end(); it++){
        if(*it == n){// �ҵ����ս��
            for(set<string>::iterator it0 = cfg.t.begin(); it0 != cfg.t.end(); it0++){
                if(*it0 == t){// �ҵ��ս��
                    string s = m[row][col];
                    if(s == " "){
                        return res;
                    }else{
                        if(s != "$"){
                            cout<<"push("<<s<<")"<<endl;
                        }else{
                            cout<<endl;
                        }
                        // ��sתΪvector
                        int start = 0;
                        int nextEnd;
                        string next;
                        nextEnd = getStr(cfg, s, start);
                        next = s.substr(start, nextEnd - start + 1);// ָ����һ��ƥ��������ַ�
                        while(start < s.length()){
                            res.push_back(next);
                            start = nextEnd + 1;
                            nextEnd = getStr(cfg, s, start);
                            next = s.substr(start, nextEnd - start + 1);// ָ����һ��ƥ��������ַ�
                        }
                        return res;
                    }
                }
                col++;
            }
        }
        row++;
    }
    return res;
}
// ���ջ����
void printStack(stack<string> s)
{
    while(!s.empty()){
        cout<<s.top();
        s.pop();
    }

}
// �������㷨
bool isMatch(Cfg cfg, string exp)
{
    bool isMatch = true;
    cout<<"----------------------------------������----------------------------------------"<<endl;
    // �����һ��
    cout<<"ջ����\t\t\t��ǰ����\t\t\t����"<<endl;
    stack<string> gramStack;// �ķ�ջ
    int start = 0;
    int nextEnd;
    string next;
    nextEnd = getStr(cfg, exp, start);
    next = exp.substr(start, nextEnd - start + 1);// ָ����һ��ƥ��������ַ�
	// ��ʼ���ķ�ջ����Ϊ#S
	gramStack.push("#");
	gramStack.push(cfg.s);
	// �� �ķ�ջ��ջ����Ϊ# �� next��Ϊ#
	while(true){
        // �����ǰջ���ݼ���ǰ����
        printStack(gramStack);
        cout<<"\t\t\t";
        cout<<exp.substr(start, exp.length() - start);
        cout<<"\t\t\t";
	    string stackTop = gramStack.top();
	    if(stackTop != "#" || next != "#"){
            // ����ķ�ջ��ջ���Ƿ��ս��
            if(cfg.n.find(stackTop) != cfg.n.end()){
                // �ķ�ջ��ջ��
                gramStack.pop();
                cout<<"pop("<<stackTop<<"), ";
                // �ķ�ջѹ��Ԥ��������Ӧ��ʽ�ӣ�
                vector<string> v = getVector(cfg, stackTop, next);
                if(v.empty()){
                    cout<<"��Ԥ���������δ�ҵ���Ӧ����ֵ"<<endl;
                    return false;
                }
                for(int i = v.size() - 1; i >= 0; i--){
                    if(v[i] == "$"){
                        break;
                    }
                    gramStack.push(v[i]);
                }
            }else if(stackTop == next){
                // �ķ�ջ��ջ��
                gramStack.pop();
                cout<<"pop("<<stackTop<<"), ";
                // nextָ����һ��ƥ���ַ���
                start = nextEnd + 1;
                nextEnd = getStr(cfg, exp, start);
                next = exp.substr(start, nextEnd - start + 1);// ָ����һ��ƥ��������ַ�
                cout<<"next:"<<next<<endl;
            }else{
                cout<<"ջ���뵱ǰ�ַ���ƥ��"<<endl;
                return false;
            }
	    }else{
            // ����ƥ�����
            cout<<"��ȷ����"<<endl;
            return true;
	    }
    }

}

void judgeExp(string basePath, string testN, Cfg cfg)
{
    fstream in;
    string testStr = (basePath + testN + "/test.txt");
    char* test = const_cast<char*>(testStr.data());
    in.open(test);
    string exp;
    while (getline(in, exp)) {
        cout<<"��������:\n"<<exp<<endl;
        // ����û�û�м�#���Լ���
        if(exp[exp.size() - 1] != '#'){
            exp = exp + "#";
        }
        if(isMatch(cfg, exp)){
            cout<<"ƥ��ɹ���"<<endl<<endl;
        }else{
            cout<<"ƥ��ʧ�ܣ�"<<endl<<endl;
        }
    }
    in.close();
}

// --------------------------------------------------CCB end--------------------------------------

int main()
{
    Cfg cfg;
    // test1
    // test2  ����������ݹ�
    // test3  ��������������
    // test4  ����LL(1)�ķ�
    string basePath = "C:/Users/CCB/Documents/TempProject/GrammaAnalysis/GrammaAnalysis/";
    string testN = "test3";
    string gramStr = (basePath + testN + "/gram.txt");
    char* gram = const_cast<char*>(gramStr.data());

    cfg = creatCfg(gram);

    First(cfg);
    Follow(cfg);
    print(cfg);

    if(getM(cfg)){
        printM(cfg);
        cout<<endl;
        judgeExp(basePath, testN, cfg);
    }

    return 0;
}
