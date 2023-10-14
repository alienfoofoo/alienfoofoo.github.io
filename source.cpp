#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<map>
#include<algorithm>
#include<set>
#include<unordered_set>
#include "header.h"
using namespace std;

//判断操作数
bool isOperator(char ch) {
	return !(charSet.find(ch) == charSet.end());
}
//添加连接符
string addConnection(const string &str) {
	string res;
	for (int i = 0; i < str.size(); i++) {
		res += str[i];
		if (i + 1 >= str.size())break;
		if (!isOperator(str[i]) || str[i] == ')' || str[i] == '*') {
			if (!isOperator(str[i+1]) || str[i+1] == '(') {
				res += "&";
			}
		}
	}
	return res;
}
//定义优先级
int priority(char ch) {
	//闭包优先级最高
		if (ch == '*')
		{
			return 3;
		}
	//连接优先级
		if (ch == '&')
		{
			return 2;
		}
	//或操作优先级
		if (ch == '|')
		{
			return 1;
		}

		if (ch == '(')
		{
			return 0;
		}
}
//转化成后缀表达式
string topostprocess(const string &temp) {
	stack<char> cstk;
	string res;
	string str = addConnection(temp);
	cout << str << "\n";
	for (auto i : str) {
		if (!isOperator(i)) {
			res += i;
		}
		else {
			if (i == '(') {
				cstk.push(i);
			}
			else if (i==')') {
				while (!cstk.empty()&&cstk.top() != '(') {
					res+=cstk.top();
					cstk.pop();
				}
				//找不到左括号
				if (cstk.empty()) {
					cout << "no ( error!";
					return "";
				}
				cstk.pop();
			}
			else {
				if (cstk.empty()) {
					cstk.push(i);
				}else{
					while (!cstk.empty() && priority(cstk.top()) >= priority(i)) {
						res += cstk.top();
						cstk.pop();
					}
					cstk.push(i);
				}
				

			}
		}
	}
	while (!cstk.empty()) {
		res += cstk.top();
		cstk.pop();
	}
	return res;
}
/*
↑创建后缀表达式

↓nfa数据结构和实现
*/


NFA nfaBuilder(const string &ss) {
	//初始化NfaStates
	coun = 0;
	NfaStates = vector<NfaState>(1000);

	string str = topostprocess(ss);
	cout << str<<"\n";
	//NFA栈
	stack<NFA> nfastk;
	for (auto i : str) {
		if (!isOperator(i)) {
			//新建nfa
			NFA newone = NFA();
			//状态转换弧变成操作数
			newone.head->input = i;
			//入栈
			nfastk.push(newone);

			//记录出现的字符
			charRecord.insert(i);
		}
		else if (i=='*') {
			//新建一个NFA
			NFA newone = NFA();
			//取出栈顶nfa
			if (nfastk.empty()) {
				cout << "error!";
				return NFA(NULL, NULL);
			}
			NFA n1 = nfastk.top();
			nfastk.pop();
			//尾指针指向新nfa的头（返回
			n1.tail->epTrans.insert(n1.head->index);
			//尾指针指向新nfa的尾（跳出
			n1.tail->epTrans.insert(newone.tail->index);
			//新节点进入栈中的节点
			newone.head->epTrans.insert(n1.head->index);
			//新节点跳过栈中的节点
			newone.head->epTrans.insert(newone.tail->index);
			//新节点入栈
			nfastk.push(newone);
		}
		else if (i == '|') {
			NFA newone = NFA();
			//取出栈顶两个nfa n1,n2
			if (nfastk.empty()) {
				cout << "error!";
				return NFA(NULL, NULL);
			}
			NFA n1 = nfastk.top();
			nfastk.pop();
			if (nfastk.empty()) {
				cout << "error!";
				return NFA(NULL, NULL);
			}
			NFA n2 = nfastk.top();
			nfastk.pop();
			//新节点跳转到n1,n2
			newone.head->epTrans.insert(n1.head->index);
			newone.head->epTrans.insert(n2.head->index);
			//n1,n2跳转到新节点尾
			n1.tail->epTrans.insert(newone.tail->index);
			n2.tail->epTrans.insert(newone.tail->index);
			//新的nfa入栈
			nfastk.push(newone);
		}
		else if (i == '&') {
			//取出栈顶两个nfa n1,n2
			if (nfastk.empty()) {
				cout << "error!";
				return NFA(NULL, NULL);
			}
			NFA n1 = nfastk.top();
			nfastk.pop();
			if (nfastk.empty()) {
				cout << "error!";
				return NFA(NULL, NULL);
			}
			NFA n2 = nfastk.top();
			nfastk.pop();
			//n2指向n1
			n2.tail->epTrans.insert(n1.head->index);
			//n1,n2变成一个nfa
			NFA n(n2.head,n1.tail);
			nfastk.push(n);
		}
	}
	return nfastk.top();
}
void NFATablePrint(NFA temp) {
	//打印表头
	charRecord.insert('#');
	cout << "		";
	for (auto i : charRecord) {
		cout << i << "		";
	}
	cout << "\n";
	map<int, bool> help;
	int cnt = 1;
	vector<NfaState> nodeList;
	stack<NfaState> nfastk;
	nfastk.push(*temp.head);
	help[temp.head->index] = true;
	while (!nfastk.empty()) {
		NfaState cur = nfastk.top();
		nfastk.pop();
		nodeList.push_back(cur);
		//输出信息
		cout << cur.index << "		";
		for (auto i : charRecord) {
			if (i == '#') {
				for (auto j : cur.epTrans) {
					cout << j<<" ";
				}
			}
			else if (i == cur.input) {
				cout << cur.chTrans << " ";
			}
			else {
				cout << "		";
			}
		}
		//加入连接的节点
		if (cur.chTrans != -1) {
			if (!help[cur.chTrans]) {
				nfastk.push(NfaStates[cur.chTrans]);
				help[cur.chTrans] = true;
			}
		}
		for (auto j: cur.epTrans) {
			if (!help[j]) {
				nfastk.push(NfaStates[j]);
				help[j] = true;
			}
		}
		cout << "\n";
	}
	
}
/*
↑正则表达式转NFA

↓NFA转DFA
*/

void dfaBuilder(NfaState start,NfaState end) {
	Dstats = vector<DfaState>();
	//加入nfa开头节点的空边闭包
	Dstats.emplace_back(e_closure(start.index));
	set<DfaState> vis;
	vis.insert(Dstats[0]);
	for (int i = 0; i < Dstats.size(); i++) {
		for (auto& j : charRecord) {
			if (j == '#')continue;
			//取得该状态下该字符的闭包
			DfaState U = e_closure(move(Dstats[i], j));
			if (U.closure.empty())continue;
			//状态未在集合中
			if (vis.find(U) == vis.end()) {
				Dstats.emplace_back(U);
				//如果没有到结束节点
				if (U.closure.find(end.index) != U.closure.end()) {
					//end集合中添加U的下标
					dfaend.insert(std::find(Dstats.begin(), Dstats.end(), U) - Dstats.begin());//U在DStat中的下标
					TerminateChars.insert(j);//终结字符添加
				}
				//状态集中加入该集合
				vis.insert(U);
			}
			//加入边集,该dfa状态通过字符所指向的dfa状态
			G[i][j] = std::find(Dstats.begin(), Dstats.end(), U) - Dstats.begin();//U在DStat中的下标
		}
	}
	dfaPrinter(Dstats,start.index);

}
void dfaPrinter(const vector<DfaState>& Dstats,int start) {
	for (int i = 0; i < Dstats.size(); ++i)
	{
		std::cout << i << ": {";
		for (auto& j : Dstats[i].closure)
			std::cout << j << ' ';
		std::cout << "}\n";
	}
	stack<int> stk;
	map<int, bool> help;
	stk.push(start);
	while (!stk.empty()) {
		int temp = stk.top();
		stk.pop();
		for (auto& i : G[temp]) {
			cout << temp << " " << i.first << " " << i.second;
			if (!help[i.second]) {
				help[i.second] = true;
				stk.push(i.second);
			}
			cout << "\n";
		}
	}

}
//状态s通过空边能到达的状态集合
DfaState e_closure(int s) {
	DfaState stat;
	stat.closure.insert(s);
	stack<int> stk;
	stk.emplace(s);
	while (!stk.empty()) {
		int i = stk.top();
		stk.pop();
		for (auto& v : NfaStates[i].epTrans) {
			if (stat.closure.find(v) != stat.closure.end()) {
				continue;
			}
			stk.emplace(v);
			stat.closure.insert(v);
		}
	}
	return stat;
}
//move(T, a) : NFA中，一个状态集合T中通过一个字符a可以转移到的状态集合为
DfaState move(const DfaState &T,char a) {
	DfaState stat;
	for (auto& i : T.closure) {
		//如果字符边等于该字符，闭包中添加字符边所指向的状态
		if (NfaStates[i].input == a) {
			stat.closure.insert(NfaStates[i].chTrans);
		}
	}
	return stat;
}
//ϵ−closure(T): 一个状态集合T中的所有状态通过任意数量的空边能转移到的状态集合为
DfaState e_closure(DfaState T) {
	DfaState stat;
	for (auto& i : T.closure)
	{
		auto tmp = e_closure(i);
		for (auto& v : tmp.closure)
			stat.closure.insert(v);
	}
	return stat;

}

void mindfaBuilder() {
	/*给dfa状态标记*/
	for (int i = 0; i < Dstats.size(); i++) {
		Dstats[i].index = i;
	}
	/*dfa集合的总的划分集合*/
	vector<set<DfaState>> PartSets(128);
	int totCount = 2;
	/*划分终态和非终态*/
	for (int i = 0; i < Dstats.size();i++) {
		if (dfaend.count(i) != 0) {
			PartSets[0].insert(Dstats[i]);
		}
		else {
			PartSets[1].insert(Dstats[i]);
		}
	}
	bool flag = true;

	while (flag) {
		int cutCount = 0;//划分次数
		flag = false;
		//取出每个划分集合set
		for (auto &Set: PartSets) {
		
			//取出终结字符,每一个终结字符划分一次
			for (auto ch : TerminateChars) {
				//缓冲区存取划分 转移到的集合号->划分集合中的元素
				map<int, set<DfaState>> keeper;
				//记录某个DFA状态是否已经加入过划分
				//map<int, bool> help;
				//取出集合中的dfa状态
				for (auto state : Set) {
					//判断该dfa状态是否存在与该终结符对应的弧
					bool havaEdge = false;
					for (auto edge : G[state.index]) {
						if (edge.first == ch) {
							//找到该弧转换到的状态所属的集合划分号
							int setNum = setFind(PartSets, edge.second, totCount);
							//将该state加入到缓冲区能转换到setNum的状态集中;
							keeper[setNum].insert(state);
							//help[state.index] = true;
							havaEdge = true;
							break;
						}
					}
					if (!havaEdge) {
						//如果不存在通过该终结符能连接到的DFA状态,也没加入过划分集，记录下来
						keeper[-1].insert(state);
					}
				}
				if (keeper.size() > 1) { //缓冲区中元素的个数大于1则需要划分
					cutCount++;
					//替换掉原集合
					Set = (keeper.begin()->second);
					//多出来的集合加上
					for (auto it = (++keeper.begin()); it != keeper.end(); it++) {
						PartSets[totCount++] = it->second;
					}
				}
			}
			
		}
		if (cutCount > 0) {
			flag = true;
		}
	}
	//打印化简后的集合
	mindfaPrinter(PartSets, totCount);
	//DFA重新编号，合并边

	//新建边集
	map<int, map<char, int>> Gnew;
	int num = 0;
	for (auto &Set : PartSets) {
		for (auto &Dstat : Set) {
			for (auto& edge : G[Dstat.index]) {
				Gnew[num][edge.first] = setFind(PartSets, edge.second, totCount);
			}
		}
		num++;
	}
	G = Gnew;

	//新建终结集
	unordered_set<int> newend;
	for (auto& i : dfaend) {
		newend.insert(setFind(PartSets, i, totCount));
	}
	dfaend = newend;

	BeginStat = setFind(PartSets, Dstats[0].index, totCount);
	dfaPrinter(Dstats,BeginStat);


}
int setFind(const vector<set<DfaState>> &temp,int statNum,int setSize) {
	for (int i = 0; i < setSize; i++) {
		for (auto& Dstat : temp[i]) {
			if (Dstat.index == statNum) {
				return i;
			}
		}
	}
	return -1;
}
void mindfaPrinter(const vector<set<DfaState>>& temp,int cnt) {
	int num = 0;
	for (auto &i : temp) {
		cout << "{";
		for (auto& j : i) {
			cout << j.index<<" ";
		}
		cout << "}\n";
		num++;
		if (num >= cnt)break;
	}
}
/*
进行c++程序生成，写一个图的搜索算法
有边集G，终态dfaend，开始状态
*/

void programGenerator() {
	string Cpp;
	//头文件
	Cpp += "#include<iostream>\n#include<string>\nusing namespace std;\n";
	//定义主函数
	Cpp += "int main(){\n";
	//定义变量，输入字符串
	Cpp += "int state = " + cTos(BeginStat+'0') + ";\n";
	Cpp += "string str;\n";
	Cpp += "cin>>str;\n";
	Cpp += "bool canbreak = false;\n";
	Cpp += "bool canTransist = true;\n";
	//开始遍历字符串循环
	Cpp += "for(int i = 0;i<str.size();i++){\n";
	Cpp += "switch(state){\n";
	/*把边集转换成switch case循环，由于有state和char两个参数因此switch有两层*/
	for (auto &edge : G) {
		Cpp += "case " + cTos(edge.first + '0') + ": switch(str[i]){\n";
		for (auto trans : edge.second) {
			/*判断是否终结状态，做上标记*/
			if (dfaend.count(trans.second) != 0) {
				Cpp += "case '" + cTos(trans.first) + "': state = " + cTos(trans.second+'0') + +";\ncanbreak=true;\nbreak;\n";
			}
			else {
				Cpp += "case '" + cTos(trans.first) + "': state = " + cTos(trans.second+'0') + ";\nbreak;\n";
			}
		
		}
		Cpp += "default: \ncout<<\"can not be transisted\";\ncanTransist = false;\nbreak;\n}\nbreak;\n";
	}
	/*结束判断，是否成功*/
	Cpp += "if(canbreak&&i==str.size()-1){\ncout<<\"success\";\nbreak;\n}\n";
	Cpp += "if(!canTransist){\nbreak;\n}\n";
	Cpp += "}\n}\n}\n";
	std::cout << Cpp;
}

string cTos(char c) {

	return string(1,c);
}
int main() {
	NFA res = nfaBuilder("(a|b)*(aa|bb)(a|b)*1159");
	if (res.head == NULL) {
		return -1;
	}
	NFATablePrint(res);
	dfaBuilder(*res.head, *res.tail);
	mindfaBuilder();
	programGenerator();
}