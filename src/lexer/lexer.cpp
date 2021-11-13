#include "./lexer.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <cctype>

Lexer::Lexer() {}
Lexer::~Lexer() {}

int getCurrentLine(std::istream& is)
{
    int lineCount = 1;   // need to clear error bits otherwise tellg returns -1.
    auto originalPos = is.tellg();
    if (originalPos < 0) {
		return -1;
	}
	is.seekg(0);
	char c;
    while ((is.tellg() < originalPos) && is.get(c)) {
        if (c == '\n') ++lineCount;
    }
    return lineCount;
}

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

vector<Pair> Lexer::readFile(string filename) {
	regex identifier("[a-zA-Z][a-zA-Z0-9_]*");
	regex keywords("int|char|if|elif|else|while|input|print|println");
	regex numeric("[0-9]+");

	ifstream filehandler;
	filehandler.open(filename);
	vector<Pair> TLpairs;
	
	if (filehandler.is_open()) {
		while (!filehandler.eof()) {
			try {

				string stream = getNext(filehandler);
				if (regex_match(stream, keywords)) {
					// now we know its a keyword
					Pair temp;
					
					if (stream == "int") { temp.token = Token::INT; }
					else if (stream =="char") { temp.token = Token::CHAR; }
					else if (stream =="if") { temp.token = Token::IF; }
					else if (stream =="elif") { temp.token = Token::ELIF; }
					else if (stream =="else") { temp.token = Token::ELSE; }
					else if (stream =="while") { temp.token = Token::WHILE; }
					else if (stream =="input") { temp.token = Token::INPUT; }
					else if (stream =="print") { temp.token = Token::PRINT; }
					else if (stream =="println") { temp.token = Token::PRINTLN; }
					
					TLpairs.push_back(temp);
				}

				else if (regex_match(stream, identifier)) {
					//identifier
					Pair temp;
					temp.token = Token::ID;
					temp.lexeme = stream;
					TLpairs.push_back(temp);
				}

				else if (regex_match(stream, numeric)) {
					//numeric literals
					Pair temp;
					temp.token = Token::NUM;
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

								bool loop; loop = true;
								while(loop) {
									if (filehandler.peek() == '*') {
										stream.push_back(filehandler.get());
										if (filehandler.peek() == '/') {
											stream.push_back(filehandler.get());
											loop = false;
										}
									}
									stream.push_back(filehandler.get());
									if (filehandler.eof()) {
										throw runtime_error("Missing closing block comment tag");
									}
								}
								temp.lexeme = stream.substr(2, stream.size() - 3);
								TLpairs.push_back(temp);
							}
							else if (filehandler.peek() == ' ' || isalpha(filehandler.peek()) || isdigit(filehandler.peek())) {
								temp.token = Token::DIV;
								TLpairs.push_back(temp);
							}
							break;
							
						case '*':
							temp.token = Token::MUL;
							TLpairs.push_back(temp);
							break;

						case '<':
							if (filehandler.peek() == '=') {
								temp.token = Token::LTE;
							}
							else {
								temp.token = Token::LT;
							}
							
							TLpairs.push_back(temp);
							break;

						case '=':
							if (filehandler.peek() == '=') {
								temp.token = Token::EQ;
								stream.push_back(filehandler.get());
							}
							else {
								temp.token = Token::AS;
							}
							
							TLpairs.push_back(temp);
							break;

						case '>':
							if (filehandler.peek() == '=') {
								temp.token = Token::GTE;
							}
							else {
								temp.token = Token::GT;
							}
							
							TLpairs.push_back(temp);
							break;

						case '~':
							if (filehandler.peek() == '=') {
								temp.token = Token::NEQ;
							}
							else {
								throw std::runtime_error("Missing = operator");
							}
							
							TLpairs.push_back(temp);
							break;
						
						case '\"':
							temp.token = Token::STR;
							bool loop; loop = true;
							while(loop) {
								if (filehandler.peek() == '\"') {
									loop = false;
								}
								stream.push_back(filehandler.get());
								if (filehandler.eof()) {
									throw std::runtime_error("Missing closing double brackets");
								}
							}
							temp.lexeme = stream.substr(1, stream.size() - 2 );
							TLpairs.push_back(temp);
							break;
						
						case '\'':
							temp.token = Token::LIT;
							temp.lexeme.push_back(filehandler.get());
							if (filehandler.peek() != '\'') {
								throw std::runtime_error("Missing closing single quotes");
							}
							stream.push_back(filehandler.get());
							TLpairs.push_back(temp);
							break;

						case '[':
							temp.token = Token::SBO;
							TLpairs.push_back(temp);
							break;

						case ']':
							temp.token = Token::SBC;
							TLpairs.push_back(temp);
							break;
						
						case '{':
							temp.token = Token::FBO;
							TLpairs.push_back(temp);
							break;
						
						case '}':
							temp.token = Token::FBC;
							TLpairs.push_back(temp);
							break;

						case '(':
							temp.token = Token::PO;
							TLpairs.push_back(temp);
							break;
						
						case ')':
							temp.token = Token::PC;
							TLpairs.push_back(temp);
							break;
						
						case ':':
							temp.token = Token::COL;
							TLpairs.push_back(temp);
							break;

						case ';':
							temp.token = Token::SCOL;
							TLpairs.push_back(temp);
							break;

						case ',':
							temp.token = Token::COM;
							TLpairs.push_back(temp);
							break;
					}
				}
			}
			catch(const char *error) {
				cerr<<"Error: "<<"at line: "<<getCurrentLine(filehandler)<<endl;
				cerr<<error<<endl;
			}
		}
		if (!filehandler.eof()) {
			cerr<<"the Lexer couldn't reach the end!"<<endl;	
			return vector<Pair>(); 							// an empty array
		}
		else {
			cout<<"file parsed successfully !!"<<endl;
			return TLpairs;
		}
	}
	else {
		cerr<<"Not File Found!"<<endl;
	}
}

