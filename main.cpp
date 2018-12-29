#include "TMParser.h"
#include "TMsimulator.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
extern void examineTMparser();

int main(int argc, char* argv[]) {
	if (argc != 2)
		return -1;
	string dir = argv[1];
	fstream infile1(dir + "/test.tm", ios::in); //xxxx.tm
	if (!infile1) return -1;

	//parse the .tm file, please view TMparser.h and TMparser.cpp
	while (!infile1.eof()) {
		string line;
		getline(infile1, line);
		TMparse(line);
	}
	infile1.close();

	fstream infile2(dir + "/test.tm", ios::in); //xxxx.tm
	if (!infile2) return -1;

	while (!infile2.eof()) {
		string line;
		getline(infile2, line);
		addRules(line);
	}
	infile2.close();

	//examineTMparser();
	fstream input(dir + "/input.txt", ios::in);
	if (!input) return -1;

	fstream outfile1(dir + "/console.txt", ios::out);
	fstream outfile2(dir + "/result.txt", ios::out);

	while (!input.eof()) {
		string in;
		getline(input, in);
		cout << in << endl;
		TMsimulate(in, outfile1, outfile2);
		delTape();
	}
	input.close();

	outfile1.close();
	outfile2.close();

	delNodes();
}