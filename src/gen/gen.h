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
	Gen(map<string, Token>, map<string, int>, vector<string>, int, int);
	Gen(Translator);
	void convert();
	void fillInitValTable();
	int newTemp(int);
	int newTemp(string);
	int newTemp(char);

	void statement();
	void assignment();
	void operators();
	void ifcmd();
	void compare();
	void out();
	void in();


	~Gen();
};


#endif