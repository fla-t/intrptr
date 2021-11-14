#include "../utils/token.h"
#include "../utils/pair.h"
#include <vector>
#include <iostream>
#include <map>
using namespace std;

#ifndef PARSER_H
#define PARSER_H


class Parser {
	vector<Pair> tokenStream;
	vector<Pair>::iterator currentToken;
	map<string, Token> datatypeTable;

public:
	Parser(vector<Pair>);
	void PrintDataTypeTable();
	void Parse();
	void match(Token, bool);

	void statement();
	void input();
	void assignment();
	void funcs();
	void ifcmd();
	void whilecmd();

	void expr();
	void T();
	void R();
	void F();
	void Rprime();


	void docmd();
	void branch();
	void comparison();
	void RO();

	void params();

	void initialization();
	
	void charinit();
	void charid();
	void charinitlist();

	void intinit();
	void intid();
	void intinitlist();
};


#endif //PARSER_H