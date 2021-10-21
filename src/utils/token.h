#ifndef TOKEN_H
#define TOKEN_H

enum Token {
	INT,
	CHAR,
	WHILE,
	IF,
	ELIF,
	ELSE, 
	INPUT, 			// input
	PRINT, 			// print()
	PRINTLN,	 	// println()
	MUL, 			// multiply
	DIV, 			// division
	ADD, 			// addition
	SUB, 			// subtraction
	LT, 			// less than
	LTE, 			// less than equal
	GT, 			// greater than 
	GTE, 			// greater than equal
	EQ, 			// equal
	NEQ, 			// not equal
	SLC, 			// single line comments
	MLC, 			// multi line comments
	ID, 			// identifier
	NUM, 			// numeric literal
	LIT, 			// char literal
	STR, 			// string literal
	AS, 			// assignment
	IN, 			// input symbol
	SBO, 			// square bracket open
	SBC, 			// square bracket close
 	FBO, 			// flower bracket open
	FBC, 			// flower bracket close
	PO, 			// parenthesis open
	PC, 			// parenthesis close
	COL, 			// colon
	SCOL, 			// semi colon
	COM, 			// comma
	INC, 			// increment
	DEC, 			// decrement
};

#endif 