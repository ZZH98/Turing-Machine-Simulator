#ifndef _TMSIMULATOR_H_
#define _TMSIMULATOR_H_
#include <string>
using namespace std;

/*use the 2-direction linked list to describe a tape. */

struct TapeNode {
	int idx;
	char con;
	TapeNode* pre;
	TapeNode* next;
};

/*called by main, simulate a single input*/
void TMsimulate(string input, ostream& out1, ostream& out2);

void delTape();

#endif