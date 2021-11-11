#include "parser.h"

Parser::Parser(vector<Pair> _TokenStream) {
	this->tokenStream = _TokenStream;
	this->currentToken = this->tokenStream.begin();
}

void Parser::Parse() {
	if (this->tokenStream.empty()) { 
		cout << "The tokenStream is empty !" << endl;
		return;
	}
	else {
		statement();
	}
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
	else if (currentToken->token == Token::WHILE) {
		whilecmd(); statement();
	}
	else if (currentToken->token == Token::ID) {
		match(Token::ID);
		if (currentToken->token == Token::AS) {
			currentToken--;
			assignment(); 
			statement();
		}
		else {
			currentToken--;
			expr();
			match(Token::SCOL);
			statement();
		}
	}
	else {

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

	// charinit part
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

	// intinit part below
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

void Parser::docmd () {
	match(Token::COL);
	match(Token::FBO);
	statement();
	match(Token::FBC);
}

void Parser::branch() {
	if (currentToken->token == Token::ELSE) {
		match(Token::ELSE);
		docmd();
	}
	else if (currentToken->token == Token::ELIF) {
		match(Token::ELIF);
		comparison();
		docmd();
		branch();
	}
	else { }
}

void Parser::whilecmd() {
	match(Token::WHILE);
	comparison();
	docmd();
}

void Parser::expr() {
	T();
	R();
}

void Parser::T() {
	F(); 
	Rprime();
}

void Parser::R() {
	if (currentToken->token == Token::ADD) {
		match(Token::ADD);
		T(); 
		R();
	}
	else if (currentToken->token == Token::SUB) {
		match(Token::SUB);
		T();
		R();
	}
	else {

	}
}

void Parser::Rprime() {
	if (currentToken->token == Token::MUL) {
		match(Token::MUL);
		F();
		Rprime();
	}
	else if (currentToken->token == Token::DIV) {
		match(Token::DIV);
		F();
		Rprime();
	}
	else {}
}

void Parser::F() {
	if (currentToken->token == Token::ID) {
		match(Token::ID);
		if (currentToken->token == Token::INC) {
			match(Token::INC);
		}
		else if (currentToken->token == Token::DEC) {
			match(Token::DEC);
		}
	}
	else if (currentToken->token == Token::NUM) {
		match(Token::NUM);
	}
	else if (currentToken->token == Token::LIT) {
		match(Token::LIT);
	}
	else if (currentToken->token == Token::PO) {
		match(Token::PO);
		expr();
		match(Token::PC);
	}
}

void Parser::assignment() {
	match(Token::ID);
	match(Token::AS);
	expr();
	match(Token::SCOL);
}