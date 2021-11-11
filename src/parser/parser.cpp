#include "parser.h"

Parser::Parser(vector<Pair> _TokenStream) {
	this->tokenStream = _TokenStream;
	this->currentToken = this->tokenStream.begin();
}

void Parser::Parse() {
	if (this->tokenStream.empty()) { cout << "The tokenStream is empty !" << endl; return; }

}


void Parser::match(Token symbol) {
	if (symbol == this->currentToken->token) {
		this->currentToken++;
	}
	else {
		throw std::runtime_error("Bad Token");
	}
}

void Parser::statement() {
	if (currentToken->token == Token::INT || currentToken->token == Token::CHAR) {
		initialization();
	}
	else if (currentToken->token == Token::INPUT) { 
		input(); statement();
	}
	else if (currentToken->token == Token::PRINT || currentToken->token == Token::PRINTLN) {
		funcs(); statement();
	}
	else if (currentToken->token == Token::IF) {
		ifcmd(); statement();
	}
}

void Parser::initialization() {
	if (currentToken->token == Token::INT) {
		intinit();
	}
	else if (currentToken->token == Token::CHAR) {
		charinit();
	}
}

void Parser::charinit() {
// includes charid
	match(Token::CHAR); 
	match(Token::COL);
	match(Token::ID);

	if (currentToken->token == Token::AS) {
		match(Token::AS);
		expr();
		charinitlist();
		match(Token::SCOL);
	}
	else {
		charinitlist();
		match(Token::SCOL);
	}	
}

void Parser::charinitlist() {
// includes contidinit
	if (currentToken->token == Token::COM) {
		match(Token::COM);
		match(Token::ID);

		if (currentToken->token == Token::AS) {
			match(Token::AS);
			expr();
			charinitlist();
		}
		else {
			charinitlist();
		}
	}
	else {

	}
}

void Parser::intinit() {
// includes intid
	match(Token::INT); 
	match(Token::COL);
	match(Token::ID);

//intinit part below
	if (currentToken->token == Token::AS) {
		match(Token::AS);
		expr();
		intinitlist();
		match(Token::SCOL);
	}
	else {
		intinitlist();
		match(Token::SCOL);
	}	
}

void Parser::intinitlist() {
// includes contidinit
	if (currentToken->token == Token::COM) {
		match(Token::COM);
		match(Token::ID);

		if (currentToken->token == Token::AS) {
			match(Token::AS);
			expr();
			intinitlist();
		}
		else {
			intinitlist();
		}
	}
	else {

	}
}

void Parser::input() {
	match(Token::INPUT);
	match(Token::IN);
	match(Token::ID);
	match(Token::SCOL);
}

void Parser::funcs() {
	if (currentToken->token == Token::PRINT) {
		match(Token::PRINT);
		match(Token::PO);
		params();
		match(Token::PC);
		match(Token::SCOL);
	}
	else if (currentToken->token == Token::PRINTLN) {
		match(Token::PRINTLN);
		match(Token::PO);
		params();
		match(Token::PC);
		match(Token::SCOL);
	}
}

void Parser::params() {
	if (currentToken->token == Token::ID) {
		match(Token::ID);
	}
	else if (currentToken->token == Token::STR) {
		match(Token::STR);
	}
	else if (currentToken->token == Token::LIT) {
		match(Token::LIT);
	}
	else if (currentToken->token == Token::NUM) {
		match(Token::NUM);
	}
	else {
		expr();
	}
}

void Parser::ifcmd() {
	match(Token::IF);
	comparison();
	docmd();
	branch();
}

void Parser::comparison() {
	if (currentToken->token == Token::PO) {
		match(Token::PO);
		expr();
		match(Token::PC);
		RO();
		match(Token::PO);
		expr();
		match(Token::PC);
	}
	else {
		match(Token::ID);
		RO();
		match(Token::ID);
	}
}

void Parser::RO () {
	if (currentToken->token == Token::LT) {
		match(Token::LT);
	}
	else if (currentToken->token == Token::LTE) {
		match(Token::LTE);
	}
	else if (currentToken->token == Token::GT) {
		match(Token::GT);
	}
	else if (currentToken->token == Token::GTE) {
		match(Token::GTE);
	}
	else if (currentToken->token == Token::NEQ) {
		match(Token::NEQ);
	}
	else if (currentToken->token == Token::EQ) {
		match(Token::EQ);
	}
}