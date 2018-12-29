#pragma once
#include <iostream>
using namespace std;

struct TransNode;

/*State is stored in struct StateNode. The data structure of states is linked list. */
struct StateNode {
	string name; //name of the state
	TransNode* rules; //all the rules that associated with this state, the data structure is also linked list. 
	int type; // 1: start -1: final 0: others
	StateNode* next; //point to next state in the linked list
};

/*Move is stored in struct TransNode. */
struct TransNode {
	char ori;
	char rwt;
	char dir;
	StateNode* target;
	TransNode* next;
};

/*called by main */
void TMparse(string line);
void addRules(string line);

void delNodes();