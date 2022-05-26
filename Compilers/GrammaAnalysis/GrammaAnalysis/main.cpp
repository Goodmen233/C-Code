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


// ！！！！！！！！！！！！ 注：用 $ 表示 ε ！！！！！！！！！！！！
// 文法的表示----------------------------------------------------
struct Cfg{
    set<string> n;// 非终结符集合
    set<string> t;// 终结符集合
    map<string, set<string> > p;// 产生式集合
    string s;// 开始符号
};
// 说明：
// 产生式集合p的key为产生式左部
// 产生式集合p的value为产生式右部(省略了|)

// first和follow集的表示----------------------------------------
map<string, set<string> > first;
map<string, set<string> > follow;
// 预测分析表的表示---------------------------------------------
string m[MAXNUM][MAXNUM];


// --------------------------------------------------ZZC start--------------------------------------
list< list<string> > gramRule;           //用于存放所有的文法规则
fstream infile;
fstream file;                          //读写操作，对打开的文件可进行读写操作
vector<string> gramBuf;                  //暂存从文件中读取的文法规则
map<string, bool> nonSymExist;          //用于标记该左部非终结符是否已存在
int n = 0;
//函数声明部分
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
	if ("" == str)//若为空，则返回原字符串
	{
		return resVec;
	}
	//方便截取最后一段数据
	string strs = str + part;
	size_t pos = strs.find(part);//find函数的返回值，若找到分隔符返回分隔符第一次出现的位置，否则返回npos此处用size_t类型是为了返回位置
	size_t size = strs.size();
	while (pos != string::npos)
	{
		string x = strs.substr(0, pos);//substr函数，获得子字符串
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
        list<string> tempList;                             //声明一个临时链表以存放每个非终结符的右部产生式

        //寻找右部产生式的开始标志 ->
        for (int j = 0;j < strLen;j++) {
            if (tempStr[j] == '>') {
                tempList.push_back(tempStr.substr(0, j - 1));       //加入文法左部的非终结符
                nonSymExist[tempStr.substr(0, j - 1)] = true;
                start = j + 1;
                break;
            }
        }

        //根据|拆分每个右部产生式
        for (int j = start;j < strLen;j++) {
            if (tempStr[j] == '|') {
                tempList.push_back(tempStr.substr(start, j - start));
                start = j + 1;
            }
        }
        //加入最后一个右部产生式
        //cout << "最后一个右部产生式: " << tempStr.substr(start) << endl;
        tempList.push_back(tempStr.substr(start));
        gramRule.push_back(tempList);                        //将新拆分好的文法规则插至链表最后
    }
    cout << "------------------------" << endl;
    cout << "拆分好的文法规则为: " << endl;
    output_all();
    cout << "------------------------" << endl;
}

/*
 * 消除左递归
 */
void remove_left_recursion() {
    list< list<string> >::iterator it_i, it_j;                   //分别为外层循环和内层循环的临时变量
    list<string>::iterator it_k, it_temp;

    for (it_i = gramRule.begin(); it_i != gramRule.end(); it_i++) {
//        for (it_j = gramRule.begin(); it_j != it_i; it_j++) {
//            it_k = it_i -> begin();
//            /**
//             * 遍历查找外层循环对应的链表的右部产生式，查找是否有Ai->Ajr形式的右部产生式
//             */
//            for (it_k++; it_k != it_i -> end();) {
//                string searchStr = *it_k;
//                string srcStr = it_j -> front();
//                bool isfind = false;
//                if (isupper(searchStr[0])) {            //右部产生式的首字母为非终结符时,才可能出现左递归的情况
//                    for (int i = 0;i < searchStr.length();i++) {
//                        if (isupper(searchStr[i])) {
//                            if(searchStr.substr(i, srcStr.size()) == srcStr) {  //找到了Ai->Ajr形式的右部产生式,则将其替换Ai的右部产生式
//                                isfind = true;
//                                it_temp = it_j -> begin();
//                                // 用Aj的右部产生式替换Ai->Ajr形式的右部产生式
//                                for (it_temp++; it_temp != it_j -> end(); it_temp++) {
//                                    // 前插法
//                                    it_i -> insert(it_k, searchStr.substr(0, i) + *it_temp + searchStr.substr(i+1));
//                                }
//                                it_k = it_i -> erase(it_k);              //删除替换前的结点,返回删除结点的后一个结点的迭代器
//                            }
//                        }
//                    }
//                }
//                if (!isfind) {              //没有找到可替换的非终结符,则迭代器自增,继续向后查找
//                    it_k++;
//                }
//            }
//        }
        /**
         * 判断替换后的规则是否存在直接左递归,若存在,则将左递归转换成右递归
         */
        bool hasRecur = false;
        it_temp = it_i -> begin();
        vector<string> tempStrs;                     //用于存放左递归部分的右部产生式,即E -> E + T,则存放+T
        for (it_temp++; it_temp != it_i -> end(); ) {       //查找是否有E -> E...形式的左递归
            if (it_temp -> substr(0, (it_i -> front()).size()) == it_i -> front()) {
                hasRecur = true;                                    //表示规则中存在左递归
                tempStrs.push_back(it_temp -> substr((it_i -> front()).size()));
                it_temp = it_i -> erase(it_temp);               //删除含有左递归的结点
            } else {
                it_temp++;
            }
        }
        if (hasRecur) {             //如果含有左递归,则将其转化成右递归再存入文法规则中

            it_temp = it_i -> begin();

            for (it_temp++; it_temp != it_i -> end(); it_temp++) {      //引入新的非终结符
                if(*it_temp == "$"){
                    *it_temp = it_i -> front() + "'";
                }else{
                    *it_temp += it_i -> front() + "'";
                }
            }

            for (int i = 0; i < tempStrs.size(); i++) {
                tempStrs[i] += it_i -> front() + "'";
            }

            list<string> newGram;                                   //引入新的右递归规则
            string tempnonsym = it_i -> front() + "'";
            while (nonSymExist[tempnonsym]) {
                tempnonsym += "'";                              //该非终结符已经存在,因此要用新的终结符
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
    cout << "消除左递归后的文法规则为: " << endl;
    output_all();
    cout << "------------------------" << endl;
}

/**
 * 提取左公因子
 */
void remove_left_common_factor() {
    list< list<string> >::iterator it_i, it_j;                   //分别为外层循环和内层循环的临时变量
    list<string>::iterator it_k, it_temp;
    bool isrecursion = false;
    /**
     * 提取左公因子
     */
    for (it_i = gramRule.begin(); it_i != gramRule.end();it_i++) {
        it_k = it_i -> begin();
        for (it_k++; it_k != it_i -> end(); it_k++) {
            vector<string> tempStrs;                     //用于存放左公因子右部产生式除公因子外的剩余部分
            bool issamefind = false;
            map<string, bool> tempsign;
            it_temp = it_k;
            int commonlen = it_k -> size();
            for (it_temp++; it_temp != it_i -> end(); it_temp++) {
                if ((*it_temp)[0] == (*it_k)[0]) {
                    bool issubstr = true;
                    for (int i = 0; i < min(it_temp -> size(), it_k -> size()); i++) {           //左公因子长度可能大于1
                        if ((*it_temp)[i] != (*it_k)[i]) {
                            issubstr = false;
                             //取所有左公因子最短的部分提取
                            commonlen = min(commonlen, i);
                            break;
                        }
                    }

                    if (issubstr) {         //正常遍历结束,说明其中一个产生式是另外一个产生式的子串
                        int sublen = min(it_temp -> size(), it_k -> size());
                        commonlen = min(commonlen, sublen);
                    }
                    issamefind = true;
                    tempsign[*it_temp] = true;         //用于标记要提取左公因子的右部产生式
                    //cout << "*it_temp = " << *it_temp << endl;
                }
            }
            if (issamefind) {
                //提取左公因子操作
                for (list<string>::iterator iter = it_k; iter != it_i -> end();) {
                    if (tempsign[*iter]) {
                        tempStrs.push_back(iter -> substr(commonlen) == "" ? "$" : iter -> substr(commonlen));
                        iter = it_i -> erase(iter);               //删除该结点,并将迭代器指针后移
                    } else {
                        iter++;
                    }
                }

                list<string> newGram;                                               //引入新的非终结符规则
                string tempnonsym = it_i -> front() + "'";
                while (nonSymExist[tempnonsym]) {
                    tempnonsym += "'";                              //该非终结符已经存在,因此要用新的终结符
                }

                //将旧规则的公因子部分提取
                tempStrs.push_back(it_k -> substr(commonlen) == "" ? "$" : it_k -> substr(commonlen));
                //旧的规则引入新的非终结符,取max防止越界或者旧规则非公因子部分有剩余
                it_k -> replace(commonlen, max(tempnonsym.length(), it_k -> size() - commonlen), tempnonsym);
                //cout << "*it_k = " << *it_k << endl;
                newGram.push_back(tempnonsym);
                nonSymExist[tempnonsym] = true;
                for (int i = 0; i < tempStrs.size(); i++) {
                    newGram.push_back(tempStrs[i]);
                }
                list< list<string> >::iterator insert_temp = it_i;
                gramRule.insert(++insert_temp, newGram);                        //前插法,插在当前结点的后一位
            }
        }
    }

    if (isrecursion) {
        remove_left_common_factor();        //递归执行
    } else {
        cout << "------------------------" << endl;
        cout << "提取左公因子后的文法规则为: " << endl;
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
    for (iter = gramRule.begin(); iter != gramRule.end(); iter++) {   //将产生式存储为map形式
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
// 获取产生式右部的一个式子的第start个字符的截止位置
int getStr(Cfg cfg, string str, int start)
{
    if(start >= str.length()){
        return -1;
    }
    // 根据最长匹配原则匹配
    bool isFirstMatch = false;
    int pos = start;
    string cur = str.substr(start, 1);
    while(pos < str.length()){
        // 如果当前是终结符 或者 非终结符，继续往下匹配
        if(cfg.n.find(cur) != cfg.n.end() || cfg.t.find(cur) != cfg.t.end()){
            isFirstMatch = true;
            cur = cur + str[++pos];
        // 如果是第一次还没匹配上，继续匹配
        }else if(!isFirstMatch && (cfg.n.find(cur) == cfg.n.end() && cfg.t.find(cur) == cfg.t.end())){
            cur = cur + str[++pos];
        }else{
            // 当前位置匹配不上，返回上一个位置
            return pos - 1;
        }
    }
    return str.length() - 1;
}
// 获取产生式右部的一个式子的第ending个字符的截止位置逆方法
int NgetStr(Cfg cfg, string str, int ending)
{
    if(ending >= str.length()){
        return -1;
    }
    // 根据最长匹配原则匹配
    bool isFirstMatch = false;
    int pos = ending;
    string cur = str.substr(ending, 1);
    while(pos > str.length()){
        // 如果当前是终结符 或者 非终结符，继续往下匹配
        if(cfg.n.find(cur) != cfg.n.end() || cfg.t.find(cur) != cfg.t.end()){
            isFirstMatch = true;
            cur = str[pos--] + cur;
        // 如果是第一次还没匹配上，继续匹配
        }else if(!isFirstMatch && (cfg.n.find(cur) == cfg.n.end() && cfg.t.find(cur) == cfg.t.end())){
            cur = str[pos--] + cur;
        }else{
            // 当前位置匹配不上，返回上一个位置
            return pos + 1;
        }
    }
    return str.length() - 1;
}
//求非终结符的First集
void Get_First(Cfg cfg,map< string, set<string> > &first,string S){
    //找到符号的产生式集合
    set<string> temp = cfg.p.find(S)->second;
    for(set<string>::iterator it=temp.begin();it!=temp.end();it++){
        //得到产生式
        string Head = *it;
        int start = 0;
        int ending = getStr(cfg,Head,start);
        string head = Head.substr(start, ending - start + 1);
        bool exit=true;
        while(start<Head.length()&&exit){
            //1.如果产生式的第一个字符为终结符或$ 即S->a...
            if(cfg.t.find(head)!=cfg.t.end()||head=="$"){
                //将终结符加入到非终结符A的First集中
                first[S].insert(head);
                exit=false;
            }
            //2.如果产生式的第一个字符为非终结符
            else if(cfg.n.find(head)!=cfg.t.end()){
                //求A的Fisrt集
                Get_First(cfg,first,head);
                //如果A的First集有空 把A的First集去掉空后再加入S的First集中
                if(first[head].find("$")!=first[head].end()){
                    for(set<string>::iterator ia=first[head].begin();ia!=first[head].end();ia++){
                        if(*ia!="$"){
                            first[S].insert(*ia);
                        }
                    }
                }
                //把A的First集加入S的First集中
                else{
                    for(set<string>::iterator ia=first[head].begin();ia!=first[head].end();ia++){
                        first[S].insert(*ia);
                    }
                    exit=false;
                }
            }
            //3.如果每个字符都为非终结符且都包含空
            if(ending==Head.length()-1){
                if(first[head].find("$")!=first[head].end()){
                    first[S].insert("$");
                }
            }
            //向前移位
            start=ending+1;
            ending=getStr(cfg,Head,start);
            head = Head.substr(start, ending - start + 1);
        }
    }
}
//求文法的First集
void First(Cfg cfg){
    for(set<string>::iterator Q=cfg.n.begin(); Q!=cfg.n.end();Q++){
        Get_First(cfg,first,*Q);
    }
}
//求非终结符的Follow集
void Get_Follow(Cfg cfg,map< string, set<string> > first,map< string, set<string> > &follow,string S){
    set<string> temp=cfg.p.find(S)->second;
    for(set<string>::iterator it=temp.begin();it!=temp.end();it++){
        //得到产生式
        string Head = *it;
        int start = 0;
        int ending = getStr(cfg,Head,start);
        string head = Head.substr(start, ending - start + 1);
        start=ending+1;
        ending=getStr(cfg,Head,start);
        string head1 = Head.substr(start, ending - start + 1);
        bool exit = true;
        while(start<Head.length()&&exit){
            //判断该字符是否为非终结符
            if(cfg.n.find(head)!=cfg.n.end()){
                //如果下一位字符是终结符
                if(cfg.t.find(head1)!=cfg.t.end()){
                    //将下一位字符加入到该字符的Follow集中
                    follow[head].insert(head1);
                }
                //如果下一位字符是非终结符
                else if(cfg.n.find(head1)!=cfg.n.end()){
                    //将下一位字符First集去掉$后加入到该字符的Follow集中
                    for(set<string>::iterator ia=first[head1].begin();ia!=first[head1].end();ia++){
                        if(*ia!="$"){
                            follow[head].insert(*ia);
                        }
                    }
                }
            }
            //若有S->...A A为非终结符
            if(ending==Head.length()-1&&cfg.n.find(head1)!=cfg.n.end()){
                //将S的Follow集加入到A的Follow集中
                for(set<string>::iterator ib=follow[S].begin();ib!=follow[S].end();ib++){
                        follow[head1].insert(*ib);
                }
                //若A的First集包含$
                while(first[head1].find("$")!=first[head1].end()){
                    //将S的Follow集加入到A的前一位Follow集中
                    for(set<string>::iterator ib=follow[S].begin();ib!=follow[S].end();ib++){
                        follow[head].insert(*ib);
                    }
                    //回退
                    head1=head;
                    ending = start-1;
                    start = NgetStr(cfg,Head,ending);
                    head=Head.substr(start, ending - start + 1);
                }
                exit=false;
            }
            //向前移位
            start=ending+1;
            ending=getStr(cfg,Head,start);
            head=head1;
            head1=Head.substr(start, ending - start + 1);
        }

    }
}
//求文法的Follow集
void Follow(Cfg cfg){
    follow[cfg.s].insert("#");
    for(set<string>::iterator Q=cfg.n.begin(); Q!=cfg.n.end();Q++){
        Get_Follow(cfg,first,follow,*Q);
    }
    //再次对所有非终结符求Follow集
    for(set<string>::iterator Q=cfg.n.begin(); Q!=cfg.n.end();Q++){
        Get_Follow(cfg,first,follow,*Q);
    }
    for(set<string>::iterator Q=cfg.n.begin(); Q!=cfg.n.end();Q++){
        Get_Follow(cfg,first,follow,*Q);
    }
}
//打印
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
// 求预测分析表
bool getM(Cfg cfg)
{
    // 初始化为" "
    for(int i = 0; i < cfg.n.size(); ++i){
        for(int j = 0; j < cfg.t.size(); ++j){
            m[i][j] = " ";
        }
    }
    // 与二维数组下标对应
    // 遍历每一个终结符
    int row = 0;
    for(set<string>::iterator it = cfg.n.begin(); it != cfg.n.end(); it++, row++){
        map<string, string> resultMap;
        string str = *it;// 非终结符
        set<string> rightSet;
        // 获取非终结符对应的产生式右部
        map<string, set<string> >::iterator iter = cfg.p.find(str);
        if(iter != cfg.p.end()){
            rightSet = iter->second;
        }else{
            // 出错提示
            cout<<str<<"找不到对应的产生式右部"<<endl;
            return false;
        }
        // 遍历产生式右部
        for(set<string>::iterator it0 = rightSet.begin(); it0 != rightSet.end(); it0++){
            string right = *it0;
            if(right == "$"){
                // 将空跳转加入到follow集元素中
                set<string> followChar;
                map<string, set<string> >::iterator iter0 = follow.find(str);
                if(iter0 != follow.end()){
                    followChar = iter0->second;
                }else{
                    // 出错提示
                    cout<<str<<"找不到对应的follow集"<<endl;
                    return false;
                }
                for(set<string>::iterator it1 = followChar.begin(); it1 != followChar.end(); it1++){
                    // 如果resultMap里面已经有*it1，表示存在二义性，不符合LL(1)文法
                    if(resultMap.find(*it1) != resultMap.end()){
                        cout<<"该文法存在二义性，不是LL(1)文法!"<<endl;
                        return false;
                    }
                    resultMap[*it1] = "$";
                }
            }else{
                // 将右部对应的first集对应的元素加入
                bool exit$ = true;
                int start = 0;
                int curEnd = 0;
                string cur;
                while(exit$){
                    curEnd = getStr(cfg, right, start);
                    if(curEnd == -1){// 遍历到最后一个元素了，表示空在左部的first集中
                        // 将空跳转加入到follow集元素中
                        set<string> followChar;
                        map<string, set<string> >::iterator iter0 = follow.find(str);
                        if(iter0 != follow.end()){
                            followChar = iter0->second;
                        }else{
                            // 出错提示
                            cout<<str<<"找不到对应的follow集"<<endl;
                            return false;
                        }
                        for(set<string>::iterator it1 = followChar.begin(); it1 != followChar.end(); it1++){
                            // 如果resultMap里面已经有*it1，表示存在二义性，不符合LL(1)文法
                            if(resultMap.find(*it1) != resultMap.end()){
                                cout<<"该文法存在二义性，不是LL(1)文法!"<<endl;
                                return false;
                            }
                            resultMap[*it1] = "$";
                        }
                        break;
                    }
                    cur = right.substr(start, curEnd - start + 1);
                    // 如果右部第一个字符为终结符,直接将该右部加入
                    if(cfg.t.find(cur) != cfg.t.end()){
                        if(resultMap.find(cur) != resultMap.end()){
                            cout<<"该文法存在二义性，不是LL(1)文法!"<<endl;
                            return false;
                        }
                        resultMap[cur] = right;
                        exit$ = false;
                    }else{// 如果当前非终结符的first集有空，则继续求下一个字符的first集
                        set<string> firstChar;
                        map<string, set<string> >::iterator it2 = first.find(cur);
                        if(it2 != first.end()){
                            firstChar = it2->second;
                        }else{
                            // 出错提示
                            cout<<cur<<"找不到对应的first集"<<endl;
                            return false;
                        }
                        exit$ = false;
                        // 将first集中的元素加入
                        for(set<string>::iterator firstIt = firstChar.begin(); firstIt != firstChar.end(); firstIt++){
                            if(*firstIt == "$"){
                                exit$ = true;
                            }else{
                                if(resultMap.find(*firstIt) != resultMap.end()){
                                    cout<<"该文法存在二义性，不是LL(1)文法!"<<endl;
                                    return false;
                                }
                                resultMap[*firstIt] = right;
                            }
                        }
                    }
                    start = curEnd + 1;// 重置下一个字符起始位置
                }

            }
        }
        // 通过resultMap填充表格
        int col = 0;
        for(set<string>::iterator it0 = cfg.t.begin(); it0 != cfg.t.end(); it0++, col++){
            str = *it0;
            map<string, string >::iterator iter0 = resultMap.find(str);
            if(iter0 != resultMap.end()){
                m[row][col] = iter0->second;
            }
        }
        // map清空
        resultMap.clear();
    }
    return true;
}

// 打印预测分析表
void printM(Cfg cfg)
{
    cout<<"----------------------------------预测分析表----------------------------------------"<<endl;
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

// 获取对应预测分析表中指定终结字符和非终结字符对应的字符串的Vector集合
vector<string> getVector(Cfg cfg, string n, string t)// n非终结 t终结
{
    vector<string> res;
    int row = 0;
    int col = 0;
    for(set<string>::iterator it = cfg.n.begin(); it != cfg.n.end(); it++){
        if(*it == n){// 找到非终结符
            for(set<string>::iterator it0 = cfg.t.begin(); it0 != cfg.t.end(); it0++){
                if(*it0 == t){// 找到终结符
                    string s = m[row][col];
                    if(s == " "){
                        return res;
                    }else{
                        if(s != "$"){
                            cout<<"push("<<s<<")"<<endl;
                        }else{
                            cout<<endl;
                        }
                        // 将s转为vector
                        int start = 0;
                        int nextEnd;
                        string next;
                        nextEnd = getStr(cfg, s, start);
                        next = s.substr(start, nextEnd - start + 1);// 指向下一个匹配的输入字符
                        while(start < s.length()){
                            res.push_back(next);
                            start = nextEnd + 1;
                            nextEnd = getStr(cfg, s, start);
                            next = s.substr(start, nextEnd - start + 1);// 指向下一个匹配的输入字符
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
// 输出栈内容
void printStack(stack<string> s)
{
    while(!s.empty()){
        cout<<s.top();
        s.pop();
    }

}
// 驱动器算法
bool isMatch(Cfg cfg, string exp)
{
    bool isMatch = true;
    cout<<"----------------------------------驱动器----------------------------------------"<<endl;
    // 输出第一行
    cout<<"栈内容\t\t\t当前输入\t\t\t动作"<<endl;
    stack<string> gramStack;// 文法栈
    int start = 0;
    int nextEnd;
    string next;
    nextEnd = getStr(cfg, exp, start);
    next = exp.substr(start, nextEnd - start + 1);// 指向下一个匹配的输入字符
	// 初始化文法栈里面为#S
	gramStack.push("#");
	gramStack.push(cfg.s);
	// 当 文法栈的栈顶不为# 且 next不为#
	while(true){
        // 输出当前栈内容及当前输入
        printStack(gramStack);
        cout<<"\t\t\t";
        cout<<exp.substr(start, exp.length() - start);
        cout<<"\t\t\t";
	    string stackTop = gramStack.top();
	    if(stackTop != "#" || next != "#"){
            // 如果文法栈的栈顶是非终结符
            if(cfg.n.find(stackTop) != cfg.n.end()){
                // 文法栈弹栈；
                gramStack.pop();
                cout<<"pop("<<stackTop<<"), ";
                // 文法栈压入预测分析表对应的式子；
                vector<string> v = getVector(cfg, stackTop, next);
                if(v.empty()){
                    cout<<"在预测分析表中未找到对应的数值"<<endl;
                    return false;
                }
                for(int i = v.size() - 1; i >= 0; i--){
                    if(v[i] == "$"){
                        break;
                    }
                    gramStack.push(v[i]);
                }
            }else if(stackTop == next){
                // 文法栈弹栈；
                gramStack.pop();
                cout<<"pop("<<stackTop<<"), ";
                // next指向下一个匹配字符；
                start = nextEnd + 1;
                nextEnd = getStr(cfg, exp, start);
                next = exp.substr(start, nextEnd - start + 1);// 指向下一个匹配的输入字符
                cout<<"next:"<<next<<endl;
            }else{
                cout<<"栈顶与当前字符不匹配"<<endl;
                return false;
            }
	    }else{
            // 正常匹配结束
            cout<<"正确结束"<<endl;
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
        cout<<"测试用例:\n"<<exp<<endl;
        // 如果用户没有加#，自己加
        if(exp[exp.size() - 1] != '#'){
            exp = exp + "#";
        }
        if(isMatch(cfg, exp)){
            cout<<"匹配成功！"<<endl<<endl;
        }else{
            cout<<"匹配失败！"<<endl<<endl;
        }
    }
    in.close();
}

// --------------------------------------------------CCB end--------------------------------------

int main()
{
    Cfg cfg;
    // test1
    // test2  体现消除左递归
    // test3  体现消除左因子
    // test4  不是LL(1)文法
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
