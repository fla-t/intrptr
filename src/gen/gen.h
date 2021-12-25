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

	int currentAddr;
	int tempCount;

public:
	vector<quad> converted;

	Gen();
	Gen(Translator);

	void fillInitValTable();
	void PrintQuadTable();
	unsigned char * GenerateData();
	void PrintAddressTable();
	void PrintInitValTable();

	int newTemp(int);
	int newTemp(string);
	int newTemp(char);

	void convert();



	~Gen();
};


#endif