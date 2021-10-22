#include "../utils/pair.h"
#include <regex>
#include <stdexcept>
#include <vector>
#include <fstream>
using namespace std;

#ifndef LEXER_H
#define LEXER_H

class Lexer {
	
public:
	Lexer();
	~Lexer();
	
	vector<Pair> readFile(string filename);
};


#endif