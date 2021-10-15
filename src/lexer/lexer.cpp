#include "./lexer.h"
#include <fstream>
#include <iostream>
#include <regex>

Lexer::Lexer() {}
Lexer::~Lexer() {}

// vector<Pair> Lexer::readFile(string filename) {
// 	ifstream filehandler;
// 	filehandler.open(filename);

// 	// token lexeme pairs
// 	vector<Pair> TLpairs;
	
// 	if (!filehandler.is_open()) {
// 		while (!filehandler.eof()) {
// 			char

// 		}
// 	}
// 	else {
// 		cout<< "File not found!" <<endl;
// 	}
// }
