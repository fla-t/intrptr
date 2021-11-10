#include "parser.h"

void match(Token x, Token y) {
	if (x == y) {}
}

Parser::Parser(vector<Pair> _TokenStream) {
	this->tokenStream = _TokenStream;
	this->currentToken = this->tokenStream.begin();
}

void Parser::Parse() {
	if (this->tokenStream.empty()) { cout << "The tokenStream is empty !" << endl; return; }
 


}