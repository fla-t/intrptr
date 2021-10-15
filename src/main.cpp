#include "./lexer/lexer.h"
#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

int main() {
	ifstream filehandler;
	filehandler.open("../testfiles/examplecode.txt");

	// token lexeme pairs
	vector<Pair> TLpairs;
	
	if (filehandler.is_open()) {
		// while (!filehandler.eof()) {
			char temp;
			cout<<"temp"<<temp<<endl;
			temp = filehandler.peek();
			Pair tempPair;
			switch(temp) {
				case 'i': 
					filehandler >> temp; 
					temp = filehandler.peek();
					switch(temp) {
						case 'f':
							filehandler >> temp;
							tempPair.token = Tokens::IF;
							tempPair.lexeme = '\0';
							TLpairs.push_back(tempPair);
							break;
						case 'n':
							filehandler >> temp;
							temp = filehandler.peek();
							switch(temp) {
								case 't':
									filehandler >> temp;
									tempPair.token = Tokens::INT;
									tempPair.lexeme = "\0";
									TLpairs.push_back(tempPair);
									break;
								case 'p':
									filehandler >> temp;
									temp = filehandler.peek();
									switch(temp) {
										case 'u':
											filehandler >> temp;
											temp = filehandler.peek();
												switch(temp) {
													case 't':
														filehandler >> temp;
														temp = filehandler.peek();
														tempPair.token = Tokens::INPUT;
														tempPair.lexeme = "\0";
														TLpairs.push_back(tempPair);
														break;
													default:
														break;
												}
										default:
												break;
									}
								default:
									break;
							}
						default:
							break;
					}



				case 'c':
				case 'e':
				case 'w':
				case 'p':	
				default:
					break;	
			}
		// }
	}
	else {
		cout<< "File not found!" <<endl;
	}
}