#include "./token.h"

#include <string>
using namespace std;

#ifndef PAIR_H
#define PAIR_H

struct Pair {
	Token token;
	string lexeme = "\0";
};


#endif 