#include "./lexer.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <cctype>

Lexer::Lexer() {}
Lexer::~Lexer() {}

string getNext(ifstream& filehandler) {
	if (!filehandler.is_open()) {
		while(!filehandler.eof()) {
			if (isdigit(filehandler.peek())) {
				string num;
				while(isdigit(filehandler.peek())) {
					num.push_back(filehandler.get());
				}
				return num;
			}
			
			else if (isalpha(filehandler.peek())) {
				//string
				string alphanum;
				while(isalpha(filehandler.peek()) || isdigit(filehandler.peek()) || filehandler.peek() == '_') {
					alphanum.push_back(filehandler.get());
				}
				return alphanum;
			}

			else {
				char special;
				filehandler >> special;
				string strspecial; 
				strspecial.push_back(special);
				return strspecial;
			}
		}
	}
}

vector<Pair> Lexer::readFile(string filename) {
	ifstream filehandler;
	filehandler.open(filename);

	// token lexeme pairs
	vector<Pair> TLpairs;
	
	if (!filehandler.is_open()) {
		while (!filehandler.eof()) {
			cout << getNext(filehandler) <<endl;
		}
	}
	else {
		cout<< "File not found!" <<endl;
	}
}

