#include "./lexer.h"
#include <fstream>
#include <iostream>

Lexer::Lexer() {}
Lexer::~Lexer() {}

vector<Pair> Lexer::readFile(string filename) {
	ifstream filehandler;
	filehandler.open(filename);

	// token lexeme pairs
	vector<Pair> TLpairs;
	
	if (!filehandler.is_open()) {
		while (!filehandler.eof()) {
			string temp;

			filehandler >> temp;
			if (temp == "int") {
				Pair tempPair;

				tempPair.token = INT;
				tempPair.lexeme = "";
			}
		}
	}
	else {
		cout<< "File not found!" <<endl;
	}
}

