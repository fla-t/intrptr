#include "../utils/token.h"
#include "../utils/pair.h"
#include <vector>
#include <iostream>
using namespace std;

#ifndef PARSER_H
#define PARSER_H


class Parser {
	vector<Pair> tokenStream;
	vector<Pair>::iterator currentToken;

public:
	Parser(vector<Pair>) {}
	void Parse() {}
	void PrintParseTree() {}
	void match(Token) {}

	void statement() {}
	void expr() {}
	void input() {}
	void funcs() {}
	void ifcmd() {}

	void docmd() {}
	void branch() {}
	void comparison() {}
	void RO() {}

	void params() {}

	void initialization() {}
	
	void charinit() {}
	void charinitlist() {}

	void intinit() {}
	void intinitlist() {}

};


#endif //PARSER_H