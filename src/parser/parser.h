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
};


#endif //PARSER_H