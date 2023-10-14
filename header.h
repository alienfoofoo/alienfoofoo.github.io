#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<map>
#include<algorithm>
#include<set>
#include<unordered_set>
using namespace std;
/* 连接、选择（|）、闭包（*）、括号（）、可选（?  ）*/
const unordered_set<char> charSet = { '*','&','|','(',')' };
bool isOperator(char ch);
string addConnection(const string& str);
int priority(char ch);
string topostprocess(const string& temp);

int coun = 0;
struct NfaState {
	int index;/*nfa状态的状态号*/
	char input;/*nfa状态弧上的值，默认为#*/
	int chTrans;/*nfa状态弧转移到的状态号，默认为-1*/
	unordered_set<int> epTrans; /*当前状态通过#转移的状态号集合*/
	NfaState() {
		index = coun++;
		input = '#';
		chTrans = -1;
	}
};

int nfaStateNum = 0;
vector<NfaState> NfaStates;
unordered_set<char> charRecord;
struct NFA {
	NfaState* head;			/*NFA的头指针*/
	NfaState* tail;			/*NFA的尾指针*/
	NFA() {
		/*每次需要创建一个NFA时就通过NfaStates[nfaStateNum]和NfaStates[nfaStateNum + 1]
		从数组中取出两个状态，nfaStateNum加2，
		再更新NFA的头尾指针即可。*/
		head = &NfaStates[nfaStateNum];
		tail = &NfaStates[nfaStateNum + 1];
		head->chTrans = tail->index;
		nfaStateNum += 2;
	}
	NFA(NfaState* h, NfaState* t) {
		head = h;
		tail = t;
	}
};

NFA nfaBuilder(const string& ss);

void NFATablePrint(NFA temp);
/*
nfa to dfa
*/
const int MAX = 500;
struct Edge			/*定义DFA的转换弧*/
{

	char input;			/*弧上的值*/
	int Trans;			/*弧所指向的状态号*/
};
//记录dfa的边
map<int, map<char, int>> G;
//
unordered_set<int> dfaend;
int dfacnt = 0;
struct DfaState		/*定义DFA状态*/
{

	bool isEnd;			/*是否为终态，是为true，不是为false*/

	int index;			/*DFA状态的状态号*/
	set<int> closure;		/*NFA的ε-move()闭包*/

	int edgeNum;		/*DFA状态上的射出弧数*/
	Edge Edges[10];		/*DFA状态上的射出弧*/
	bool operator<(const DfaState& temp)const {
		return closure < temp.closure;
	}
	bool operator==(const DfaState& temp)const {
		return closure == temp.closure;
	}
	DfaState() {
		index = dfacnt++;
	}
};

struct DFA			/*定义DFA结构*/
{

	int startState;				/*DFA的初态*/

	set<int> endStates;			/*DFA的终态集*/
	set<char> terminator;		/*DFA的终结符集*/

	int trans[MAX][26];		/*DFA的转移矩阵*/
};

vector<DfaState> Dstats;

set<char> TerminateChars;

void dfaBuilder(NfaState start, NfaState end);

void dfaPrinter(const vector<DfaState>& Dstats,int);

DfaState e_closure(int s);

DfaState move(const DfaState& T, char a);

DfaState e_closure(DfaState T);
/*
最小化dfa
*/

void mindfaBuilder(DfaState end);

int setFind(const vector<set<DfaState>> &temp,int statNum,int setSize);

int BeginStat;

void mindfaPrinter(const vector<set<DfaState>>& temp,int cnt);

void programGenerator();

string cTos(char c);