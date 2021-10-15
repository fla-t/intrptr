#ifndef TOKEN_H
#define TOKEN_H

enum Tokens {
	INT,
	CHAR,
	WHILE,
	IF,
	ELIF,
	ElSE,
	INPUT,
	PRINT,
	PRINTLN,
	MUL,
	DIV,
	ADD,
	SUB,
	LT,
	LTE,
	GT,
	GTE,
	EQ,
	NEQ,
	SLC,
	MLC,
	ID,
	NUM,
	LIT,
	STR,
	AS,
	IN,
	SBO, //square bracket open
	SBC, //square bracket close
 	FBO, // flower bracket open
	FBC, // flower bracket close
	PO, // parenthesis open
	PC, // parenthesis close
	COL, // colon
	SCOL,
	COM,
	INC,
	DEC,
};

#endif 