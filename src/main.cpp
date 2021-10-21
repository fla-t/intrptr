#include "./lexer/lexer.h"
#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

string getNext(ifstream& filehandler) {
	if (filehandler.is_open()) {
		while(!filehandler.eof()) {
			char testchar;
			filehandler >> testchar;

			if (isdigit(testchar)) {
				string num;
				num.push_back(testchar);
				while(isdigit(filehandler.peek())) {
					num.push_back(filehandler.get());
				}
				return num;
			}
			
			else if (isalpha(testchar)) {
				//string
				string alphanum;
				alphanum.push_back(testchar);
				while(isalpha(filehandler.peek()) || isdigit(filehandler.peek()) || filehandler.peek() == '_') {
					alphanum.push_back(filehandler.get());
				}
				return alphanum;
			}

			else {
				string strspecial; 
				strspecial.push_back(testchar);
				return strspecial;
			}
		}
	}
	return "";
}

int main() {

	regex identifier("[a-zA-Z][a-zA-Z0-9_]*");
	regex keywords("int|char|if|elif|else|while|input|print|println");

	ifstream filehandler;
	filehandler.open("../testfiles/examplecode.txt");
	vector<Pair> TLpairs;
	
	if (filehandler.is_open()) {
		while(!filehandler.eof()) {
			string stream = getNext(filehandler);

			if (regex_match(stream, keywords)) {
				// now we know its a keyword

				if (stream == "int") {
					Pair temp;
					temp.token = Token::INT;
					temp.lexeme = "\0";
					TLpairs.push_back(temp);
				}
				
				else if (stream =="char") {
					Pair temp;
					temp.token = Token::CHAR;
					temp.lexeme = "\0";
					TLpairs.push_back(temp);
				}
				
				else if (stream =="if") {
					Pair temp;
					temp.token = Token::IF;
					temp.lexeme = "\0";
					TLpairs.push_back(temp);
				}

				else if (stream =="elif") {
					Pair temp;
					temp.token = Token::ELIF;
					temp.lexeme = "\0";
					TLpairs.push_back(temp);
				}

				else if (stream =="else") {
					Pair temp;
					temp.token = Token::ELSE;
					temp.lexeme = "\0";
					TLpairs.push_back(temp);

				}

				else if (stream =="while") {
					Pair temp;
					temp.token = Token::WHILE;
					temp.lexeme = "\0";
					TLpairs.push_back(temp);

				}

				else if (stream =="input") {
					Pair temp;
					temp.token = Token::INPUT;
					temp.lexeme = "\0";
					TLpairs.push_back(temp);

				}

				else if (stream =="print") {
					Pair temp;
					temp.token = Token::PRINT;
					temp.lexeme = "\0";
					TLpairs.push_back(temp);

				}

				else if (stream =="println") {
					Pair temp;
					temp.token = Token::PRINTLN;
					temp.lexeme = "\0";
					TLpairs.push_back(temp);

				}
			}

			else if (regex_match(stream, identifier)) {
				//identifier
				Pair temp;
				temp.token = Token::ID;
				temp.lexeme = stream;
				TLpairs.push_back(temp);
			}

			else if (stream.size() == 1) {
				Pair temp;
				switch(stream[0]) {
					case '+':
						if (filehandler.peek() == '+') {
							temp.token = Token::INC;
							stream.push_back(filehandler.get());
						}
						else {
							temp.token = Token::ADD;
						}
						temp.lexeme = "\0";
						TLpairs.push_back(temp);
						break;

					case '-':
						if (filehandler.peek() == '-') {
							temp.token = Token::DEC;
							stream.push_back(filehandler.get());
						}
						else if (filehandler.peek() == '>') { 
							temp.token = Token::IN;
							stream.push_back(filehandler.get());
						}
						else { temp.token = Token::SUB; }
						
						temp.lexeme = "\0";
						TLpairs.push_back(temp);
						break;

					case '/':
						if (filehandler.peek() == '/') {
							temp.token = Token::SLC;
							stream.push_back(filehandler.get());
							
							getline(filehandler, stream, '\n');
							temp.lexeme = stream;
							TLpairs.push_back(temp);
						}
						else if (filehandler.peek() == '*') {
							temp.token = Token::MLC;
							stream.push_back(filehandler.get());

							bool loop = true;
							while(loop) {
								if (filehandler.peek() == '*') {
									stream.push_back(filehandler.get());
									if (filehandler.peek() == '/') {
										stream.push_back(filehandler.get());
										loop = false;
									}
								}
								stream.push_back(filehandler.get());
							}
							temp.lexeme = stream;
							TLpairs.push_back(temp);
						}
						else if (filehandler.peek() == ' ' || isalpha(filehandler.peek()) || isdigit(filehandler.peek())) {
							temp.token = Token::DIV;
							temp.lexeme = "\0";
							TLpairs.push_back(temp);
						}
						break;
						
					case '*':
						temp.token = Token::MUL;
						temp.lexeme = "\0";
						TLpairs.push_back(temp);
						break;

					case '<':
						if (filehandler.peek() == '=') {
							temp.token = Token::LTE;
						}
						else {
							temp.token = Token::LT;
						}
						
						temp.lexeme = "\0";
						TLpairs.push_back(temp);
						break;

					case '=':
						if (filehandler.peek() == '=') {
							temp.token = Token::EQ;
						}
						else {
							temp.token = Token::AS;
						}
						
						temp.lexeme = "\0";
						TLpairs.push_back(temp);
						break;

					case '>':
						if (filehandler.peek() == '=') {
							temp.token = Token::GTE;
						}
						else {
							temp.token = Token::GT;
						}
						
						temp.lexeme = "\0";
						TLpairs.push_back(temp);
						break;

					case '~':
						if (filehandler.peek() == '=') {
							temp.token = Token::NEQ;
						}
						else {
							temp.token = Token::EQ;
						}
						
						temp.lexeme = "\0";
						TLpairs.push_back(temp);
						break;
					
					case '\"':
						temp.token = Token::STR;
						getline(filehandler, stream, '\"');


				}
			}

			else {
				cout<<stream<<endl;
			}
		}
	}
}