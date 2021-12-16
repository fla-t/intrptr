#include "../utils/token.h"
#include "../utils/pair.h"
#include "../utils/varStruct.h"
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
	map<string, int> addressTable;
	
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
	
	void charid();
	void charinit();
	void charinitlist();

	void intid();
	void intinit();
	void intinitlist();

};


#endif //PARSER_H