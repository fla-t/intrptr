#include "parser.h"
#include "../utils/tokenMap.h"

int tabs = 0;

void printTabs(int tabs) {
	for (int i = 0; i < tabs; i++) { cout << "    "; }
}

Parser::Parser(vector<Pair> _TokenStream) {
	this->tokenStream = _TokenStream;
	this->currentToken = this->tokenStream.begin();
}

void Parser::PrintDataTypeTable() {
	cout << "----------------------------------" << endl;
	cout << "\tDatatype Table" << endl << endl;
	cout << "ID Name" << "\t\t" << "DataType" << endl << endl;
	for (auto entry : this->datatypeTable) {
		cout << entry.first<< "\t\t" << token_to_symbol[entry.second] <<endl;
	}
	cout << "----------------------------------" << endl;
}

void Parser::Parse() {
	if (this->tokenStream.empty()) { 
		cout << "The tokenStream is empty !" << endl;
		return;
	}
	else {
		statement();
		PrintDataTypeTable();
		if (currentToken == tokenStream.end()) {
			cout << "Everything parsed successfully!" << endl;
		}
	}
}


void Parser::match(Token symbol) {
	if (symbol == this->currentToken->token) {
		printTabs(tabs);
		if (currentToken->token == Token::ID) {
			cout << currentToken->lexeme << endl;
		}
		else 
			cout << token_to_symbol[currentToken->token] << endl;
		this->currentToken++;

	}
	else {
		throw std::runtime_error("Bad Token");
	}
}

void Parser::statement() {
	if (currentToken != tokenStream.end()) {
		
		printTabs(tabs++);
		cout << "Statement" << endl;

		if (currentToken->token == Token::INT || currentToken->token == Token::CHAR) {
			initialization(); statement();
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
		else if (currentToken->token == Token::SLC || currentToken->token == Token::MLC) {
			currentToken++; statement();
		}
		else {
			tabs--;
		}
	}
	else {}
}

void Parser::initialization() {
	printTabs(tabs++);
	cout << "Initialization" << endl;

	if (currentToken->token == Token::INT) {
		intinit();
	}
	else if (currentToken->token == Token::CHAR) {
		charinit();
	}

	tabs--;
}

void Parser::charinit() {
	printTabs(tabs++);
	cout << "Charinit" << endl;

	// includes charid
	match(Token::CHAR);
	match(Token::COL);
	match(Token::ID);
	currentToken--;
	if (datatypeTable.find(currentToken->lexeme) == datatypeTable.end()) {
		datatypeTable[currentToken->lexeme] = Token::CHAR;
	}
	currentToken++;

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
	tabs--;
}

void Parser::charinitlist() {
	printTabs(tabs++);
	cout << "CharInitList" << endl;

// includes contidinit
	if (currentToken->token == Token::COM) {
		match(Token::COM);
		match(Token::ID);
		
		currentToken--;
		if (datatypeTable.find(currentToken->lexeme) == datatypeTable.end()) {
			datatypeTable[currentToken->lexeme] = Token::CHAR;
		}
		currentToken++;

		if (currentToken->token == Token::AS) {
			match(Token::AS);
			expr();
			charinitlist();
		}
		else {
			charinitlist();
		}
	}

	tabs--;
}

void Parser::intinit() {
	printTabs(tabs++);
	cout << "Intinit" << endl;

	// includes intid
	match(Token::INT); 
	match(Token::COL);
	match(Token::ID);

	currentToken--;
	if (datatypeTable.find(currentToken->lexeme) == datatypeTable.end()) {
		datatypeTable[currentToken->lexeme] = Token::INT;
	}
	currentToken++;

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

	tabs--;
}

void Parser::intinitlist() {
	printTabs(tabs++);
	cout << "IntInitList" << endl;

	// includes contidinit
	if (currentToken->token == Token::COM) {
		match(Token::COM);
		match(Token::ID);

		currentToken--;
		if (datatypeTable.find(currentToken->lexeme) == datatypeTable.end()) {
			datatypeTable[currentToken->lexeme] = Token::INT;
		}
		currentToken++;

		if (currentToken->token == Token::AS) {
			match(Token::AS);
			expr();
			intinitlist();
		}
		else {
			intinitlist();
		}
	}

	tabs--;
}

void Parser::input() {
	printTabs(tabs++);
	cout << "Input" <<endl ;
	match(Token::INPUT);
	match(Token::IN);
	match(Token::ID);
	match(Token::SCOL);
	tabs--;
}

void Parser::funcs() {
	printTabs(tabs++);
	cout << "Funcs" << endl;

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

	tabs--;
}

void Parser::params() {
	printTabs(tabs++);
	cout << "Params" << endl;

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

	tabs--;
}

void Parser::ifcmd() {
	printTabs(tabs++);
	cout << "IF" << endl;

	match(Token::IF);
	comparison();
	docmd();
	branch();

	tabs--;
}

void Parser::comparison() {
	printTabs(tabs++);
	cout << "Comparison" << endl;

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

	tabs--;
}

void Parser::RO () {
	printTabs(tabs++);
	cout << "RO" << endl;

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

	tabs--;
}

void Parser::docmd () {
	printTabs(tabs++);
	cout << "DO" << endl;

	match(Token::COL);
	match(Token::FBO);
	statement();
	match(Token::FBC);

	tabs--;
}

void Parser::branch() {
	printTabs(tabs++);
	cout << "BRANCH" << endl;

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
	
	tabs--;
}

void Parser::whilecmd() {
	printTabs(tabs++);
	cout << "WHILE" << endl;

	match(Token::WHILE);
	comparison();
	docmd();

	tabs--;
}

void Parser::expr() {
	printTabs(tabs++);
	cout << "Expression" << endl;

	T();
	R();

	tabs--;
}

void Parser::T() {
	printTabs(tabs++);
	cout << "Term" << endl;

	F(); 
	Rprime();

	tabs--;
}

void Parser::R() {
	printTabs(tabs++);
	cout << "Residual Term" << endl;

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
	
	tabs--;
}

void Parser::Rprime() {
	printTabs(tabs++);
	cout << "Residual \'" << endl;

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
		
	tabs--;
}

void Parser::F() {
	printTabs(tabs++);
	cout << "Factor" << endl;

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

	tabs--;
}

void Parser::assignment() {
	printTabs(tabs++);
	cout << "Assignment" << endl;

	match(Token::ID);
	match(Token::AS);
	expr();
	match(Token::SCOL);

	tabs--;
}