#include "../utils/pair.h"
#include <vector>
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