#include "../utils/token.h"
#include "../utils/pair.h"
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
	void PrintTranslation();
	void PrintAddressTable();
	
	void Parse();
	void match(Token, bool);
	Pair newTemp(Token);

	void statement();
	void input();
	void assignment();
	void funcs();
	void ifcmd();
	void whilecmd();

	Pair expr();
	Pair T();
	Pair R(Pair);
	Pair F();
	Pair Rprime(Pair);


	void docmd();
	void branch();
	string comparison();
	Pair RO();

	Pair params();

	void initialization();
	
	Pair charid();
	Pair charinit();
	Pair charinitlist();

	Pair intid();
	Pair intinit();
	void intinitlist();

};


#endif //TRANSLATOR_H