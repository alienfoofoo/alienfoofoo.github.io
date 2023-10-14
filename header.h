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
/* ���ӡ�ѡ��|�����հ���*�������ţ�������ѡ��?  ��*/
const unordered_set<char> charSet = { '*','&','|','(',')' };
bool isOperator(char ch);
string addConnection(const string& str);
int priority(char ch);
string topostprocess(const string& temp);

int coun = 0;
struct NfaState {
	int index;/*nfa״̬��״̬��*/
	char input;/*nfa״̬���ϵ�ֵ��Ĭ��Ϊ#*/
	int chTrans;/*nfa״̬��ת�Ƶ���״̬�ţ�Ĭ��Ϊ-1*/
	unordered_set<int> epTrans; /*��ǰ״̬ͨ��#ת�Ƶ�״̬�ż���*/
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
	NfaState* head;			/*NFA��ͷָ��*/
	NfaState* tail;			/*NFA��βָ��*/
	NFA() {
		/*ÿ����Ҫ����һ��NFAʱ��ͨ��NfaStates[nfaStateNum]��NfaStates[nfaStateNum + 1]
		��������ȡ������״̬��nfaStateNum��2��
		�ٸ���NFA��ͷβָ�뼴�ɡ�*/
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
struct Edge			/*����DFA��ת����*/
{

	char input;			/*���ϵ�ֵ*/
	int Trans;			/*����ָ���״̬��*/
};
//��¼dfa�ı�
map<int, map<char, int>> G;
//
unordered_set<int> dfaend;
int dfacnt = 0;
struct DfaState		/*����DFA״̬*/
{

	bool isEnd;			/*�Ƿ�Ϊ��̬����Ϊtrue������Ϊfalse*/

	int index;			/*DFA״̬��״̬��*/
	set<int> closure;		/*NFA�Ħ�-move()�հ�*/

	int edgeNum;		/*DFA״̬�ϵ��������*/
	Edge Edges[10];		/*DFA״̬�ϵ������*/
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

struct DFA			/*����DFA�ṹ*/
{

	int startState;				/*DFA�ĳ�̬*/

	set<int> endStates;			/*DFA����̬��*/
	set<char> terminator;		/*DFA���ս����*/

	int trans[MAX][26];		/*DFA��ת�ƾ���*/
};

vector<DfaState> Dstats;

set<char> TerminateChars;

void dfaBuilder(NfaState start, NfaState end);

void dfaPrinter(const vector<DfaState>& Dstats,int);

DfaState e_closure(int s);

DfaState move(const DfaState& T, char a);

DfaState e_closure(DfaState T);
/*
��С��dfa
*/

void mindfaBuilder(DfaState end);

int setFind(const vector<set<DfaState>> &temp,int statNum,int setSize);

int BeginStat;

void mindfaPrinter(const vector<set<DfaState>>& temp,int cnt);

void programGenerator();

string cTos(char c);