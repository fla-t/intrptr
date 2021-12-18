#include "../utils/token.h"
#include "../utils/pair.h"
#include "../utils/varStruct.h"
#include <vector>
#include <iostream>
#include <map>
using namespace std;

#ifndef TRANSLATOR_H
#define TRANSLATOR_H


class Translator {
	vector<Pair> tokenStream;
	vector<Pair>::iterator currentToken;
	map<string, Token> datatypeTable;
	map<string, int> addressTable;

	vector<string> translation;
	int currentAddr;
	int tempCount;

public:
	Translator(vector<Pair>);
	void PrintDataTypeTable();
	void Parse();
	void match(Token, bool);

	void statement();
	void input();
	void assignment();
	void funcs();
	void ifcmd();
	void whilecmd();

	Pair expr();
	Pair T();
	Pair R();
	Pair F();
	Pair Rprime();


	void docmd();
	void branch();
	void comparison();
	void RO();

	void params();

	void initialization();
	
	Pair charid();
	Pair charinit();
	Pair charinitlist();

	Pair intid();
	Pair intinit();
	Pair intinitlist();

};


#endif //TRANSLATOR_H