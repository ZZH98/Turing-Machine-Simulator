#include "TMParser.h"
#include <fstream>
#include <string>
#include <sstream>
#include <set>

using namespace std;

/*These are head and tail node of the linked list that stores all the states. */
StateNode* head = NULL;
StateNode* tail = NULL;

char blank;
StateNode* BeginState;

set<char> inputChar;
set<char> tapeChar;

string removeUseless(string line1) {
	//cout << line1 << endl;
	int len0 = line1.length();
	int i = 0;
	while (line1[i] != '{' && i < len0) i++;
	int j = i;
	while (line1[j] != '}' && j < len0) j++;
	string line = line1.substr(i + 1, j - i - 1);

	int len = line.length();
	for (int i = 0; i < len; i++) {
		if (line[i] == ';') {
			line = line.substr(0, i);
			break;
		}
		if (line[i] == ',')
			line[i] = ' ';
	}
	//cout << line << endl;
	return line;
}

StateNode* findState(string name) {
	//cout << "findState:" << name << endl;
	if (head == NULL) return NULL;
	for (StateNode* p = head; p != NULL; p = p->next) {
		if (p->name == name)
			return p;
	}
	//cout << "not found" << endl;
	return NULL;
}

TransNode* findRule(StateNode* p, char pre) {
	TransNode *ret1 = NULL, *ret2 = NULL;
	for (TransNode* h = p->rules; h != NULL; h = h->next) {
		if (h->ori == pre)
			ret1 = h;
		else if (h->ori == '*')
			ret2 = h;
	}
	if (ret1 != NULL) {
		//cout << p->name << ", find precise\n";
		return ret1;
	}
	else if (ret2 != NULL) {
		//cout << p->name << ", use * for " << pre << endl;
		return ret2;
	}
	//cout << "cannot find\n";
	return NULL;
}

void TMparse_Q(string line) {
	line = removeUseless(line);
	stringstream ss(line);
	string Sname;
	while (ss >> Sname) {
		//cout << Sname << endl;
		StateNode* sn = new StateNode;
		sn->name = Sname;
		sn->rules = NULL;
		sn->type = 0;
		sn->next = NULL;
		if (head == NULL)
			head = sn;
		else
			tail->next = sn;
		tail = sn;
	}
}

void TMparse_S(string line) {
	line = removeUseless(line);
	stringstream ss(line);
	char c;
	while (ss >> c) {
		inputChar.insert(c);
	}
}

void TMparse_T(string line) {
	line = removeUseless(line);
	stringstream ss(line);
	char c;
	while (ss >> c) {
		tapeChar.insert(c);
	}
}

void TMparse_q0(string line) {
	line = line.substr(6);
	StateNode* start = findState(line);
	start->type = 1;
	BeginState = start;
}

void TMparse_B(string line) {
	blank = line[5];
}

void TMparse_F(string line) {
	line = removeUseless(line);
	stringstream ss(line);
	string Fstate;
	while (ss >> Fstate) {
		StateNode* start = findState(line);
		//cout << "F: " << start->name << endl;
		start->type = -1;
	}
}

void TMparse(string line) {
	if (line.length() == 0)
		return;
	if (line[0] == '#') {
		switch (line[1]) {
		case 'Q': TMparse_Q(line); break;
		case 'S': TMparse_S(line); break;
		case 'T': TMparse_T(line); break;
		case 'B': TMparse_B(line); break;
		default: break;
		}
	}
	else if (line[0] == ';')
		return;
}

void addRules(string line) {
	//moves
	if (line.length() == 0)
		return;
	else if (line[0] == '#') {
		switch (line[1]) {
		case 'q': TMparse_q0(line); break;
		case 'F': TMparse_F(line); break;
		default: break;
		}
	}
	else if (line[0] == ';')
		return;
	else {
		stringstream ss(line);
		TransNode* tn = new TransNode;
		string oldstate;
		ss >> oldstate;
		StateNode* p = findState(oldstate);
		ss >> tn->ori;
		ss >> tn->rwt;
		ss >> tn->dir;
		string newstate;
		ss >> newstate;
		tn->target = findState(newstate);

		if (p->rules == NULL) {
			p->rules = tn;
			tn->next = NULL;
		}
		else {
			tn->next = p->rules;
			p->rules = tn;
		}
	}
}

void examineTMparser() {
	for (StateNode* p = head; p != NULL; p = p->next) {
		cout << p->name << ", {" << p->type << "}, ";
		for (TransNode* q = p->rules; q != NULL; q = q->next) {
			cout << "on this state: " << q->ori << "->" << q->rwt << ", ";
		}
		cout << endl;
	}
}

void delNodes() {
	StateNode* p = head;
	while (p != NULL) {
		StateNode* temp = p->next;
		TransNode* q = p->rules;
		while (q != NULL) {
			TransNode* temp2 = q->next;
			delete(q);
			q = temp2;
		}
		delete(p);
		p = temp;
	}
}