#include <iostream>
#include <set>
#include <fstream>
#include <cmath>
#include "TMparser.h"
#include "TMsimulator.h"
using namespace std;

TapeNode* pos;     // position in the tape
StateNode* cur;    //which state is in?
int step;          // count the steps

TapeNode* leftP = NULL;   //leftmost position on the tape that have been visited
TapeNode* rightP = NULL;  //rightmost position on the tape that have been visited

extern char blank;
extern StateNode* BeginState;

extern set<char> inputChar;
extern set<char> tapeChar;

extern StateNode* findState(string name);
extern TransNode* findRule(StateNode* p, char pre);

bool judgeInput(string input) {
	int len = input.length();
	for (int i = 0; i < len; i++) {
		if (inputChar.find(input[i]) == inputChar.end()) {
			return false;
		}
	}
	return true;
}

void printState(ostream &out) {
	out << "Step  : " << step << endl;
	out << "Index : ";
	TapeNode* p = leftP;
	while (p != pos && p->con == blank) {
		p = p->next;
	}
	TapeNode* q = rightP;
	while (q != pos && q->con == blank) {
		q = q->pre;
	}
	TapeNode* i = p;
	while (true) {
		out << abs(i->idx) << " ";
		if (i == q) break;
		i = i->next;
	}
	out << endl;
	out << "Tape  : ";
	i = p;
	while (true) {
		out << i->con;
		int temp = abs(i->idx);
		if (temp == 0) out << " ";
		else {
			while (temp > 0) {
				out << " ";
				temp = temp / 10;
			}
		}
		if (i == q) break;
		i = i->next;
	}
	out << endl;

	out << "Head  : ";
	i = p;
	while (true) {
		if (i == pos) {
			out << "^";
			break;
		}
		out << " ";
		int temp = abs(i->idx);
		if (temp == 0) out << " ";
		else {
			while (temp > 0) {
				out << " ";
				temp = temp / 10;
			}
		}
		i = i->next;
	}
	out << endl;

	out << "State : " << cur->name << endl;

	out << "---------------------------------------------" << endl;
}

bool moveState() {
	char ontape = pos->con;
	TransNode* tomove = findRule(cur, ontape);
	if (tomove == NULL)
		return false;
	//cout << "findRule result: " << tomove->ori << "->" << tomove->rwt << endl;
	if (tomove->rwt != '*')
		pos->con = tomove->rwt;
	if (tomove->dir == 'r') { //move right
		if (pos->next == NULL) {
			TapeNode* tn = new TapeNode;
			tn->pre = pos;
			pos->next = tn;
			tn->next = NULL;
			tn->idx = pos->idx + 1;
			tn->con = blank;
			rightP = tn;
		}
		pos = pos->next;
	}
	else if (tomove->dir == 'l') { //move left
		if (pos->pre == NULL) {
			TapeNode* tn = new TapeNode;
			tn->next = pos;
			pos->pre = tn;
			tn->pre = NULL;
			tn->idx = pos->idx - 1;
			tn->con = blank;
			leftP = tn;
		}
		pos = pos->pre;
	}
	cur = tomove->target;
	return true;
}

void generateTape(string input) {
	int len = input.length();
	for (int i = 0; i < len; i++) {
		TapeNode* tn = new TapeNode;
		tn->idx = i;
		tn->con = input[i];
		tn->pre = NULL;
		tn->next = NULL;
		if (leftP == NULL)
			leftP = rightP = tn;
		else {
			rightP->next = tn;
			tn->pre = rightP;
			rightP = tn;
		}
	}
	if (leftP == NULL) {
		leftP = new TapeNode;
		leftP->con = blank;
		leftP->idx = 0;
		leftP->pre = NULL;
		leftP->next = NULL;
		rightP = leftP;
	}
}

void TMsimulate(string input, ostream& out1, ostream& out2) {

	leftP = NULL;
	rightP = NULL;

	if (!judgeInput(input)) {
		out1 << "Input: " << input << endl;
		out1 << "==================== ERR ====================" << endl;
		out1 << "The input \"" << input << "\" is illegal" << endl;
		out1 << "==================== END ====================" << endl;
		out2 << "Error" << endl;
		return;
	}

	out1 << "Input: " << input << endl;
	out1 << "==================== RUN ====================" << endl;
	generateTape(input);

	cur = BeginState;
	pos = leftP;
	step = 0;
	printState(out1);
	while (true) {
		if (!moveState()) {
			out2 << "False" << endl;
			break;
		}
		printState(out1);
		if (cur->type == -1) {
			out2 << "True" << endl;
			break;
		}

		step++;
	}
	out1 << "Result: ";
	TapeNode* p = leftP;
	TapeNode* q = rightP;
	while (p != NULL && p->con == blank) p = p->next;
	while (q != NULL && q->con == blank) q = q->pre;
	if (p != NULL && q != NULL) {
		while (true) {
			out1 << p->con;
			if (p == q)
				break;
			p = p->next;
		}
	}
	out1 << endl;
	out1 << "==================== END ====================" << endl;
}

void delTape() {
	for (TapeNode* p = leftP; p != NULL; p = p->next) {
		delete(p);
	}
}