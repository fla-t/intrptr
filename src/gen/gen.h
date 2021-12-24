#include "../utils/token.h"
#include "../translator/translator.h"
#include "../utils/opcode.h"
#include "../utils/quad.h"

#include <vector>
#include <iostream>
#include <map>
using namespace std;

#ifndef GEN_H
#define GEN_H

class Gen {
	map<string, Token> datatypeTable;
	map<string, int> addressTable;
	map<string, string> initialvalTable;
	vector<string> translation;

	vector<quad> converted;

	int currentAddr;
	int tempCount;

public:
	Gen();
	Gen(Translator);

	void fillInitValTable();
	
	void PrintQuadTable();
	int newTemp(int);
	int newTemp(string);
	int newTemp(char);

	void convert();
	void assignment();
	void operators();
	void ifcmd();
	void compare();
	void out();
	void in();


	~Gen();
};


#endif