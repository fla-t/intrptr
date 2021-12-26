#include "token.h"
#include <map>
#include <string>
using namespace std;

#ifndef OPCODE_H
#define OPCODE_H

enum class Opcode {
	ADD, 			// addition
	MUL, 			// multiply
	DIV, 			// division
	SUB, 			// subtraction
	
	AS,				// assignment
	
	LT, 			// less than
	LTE, 			// less than equal
	GT, 			// greater than 
	GTE, 			// greater than equal
	EQ, 			// equal
	NEQ, 			// not equal
	
	OUT, 			// print()
	OUTSTR,			// printing strings

	IN, 			// input symbol
	GOTO,			// goto 
};


#endif