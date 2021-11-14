#include "parser.h"
#include "../utils/tokenSymbolMap.h"
#define TabSpace "    "

int tabs;

void printTabs() {
	for (int i = 0; i < tabs; i++) {cout<<TabSpace;}
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
		printTabs();
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
		
		printTabs();
		cout << "STATEMENT" << endl;
		tabs++;

		if (currentToken->token == Token::INT || currentToken->token == Token::CHAR) {
			initialization();
			tabs--;
			statement();
		}
		else if (currentToken->token == Token::INPUT) { 
			input(); 
			tabs--; 
			statement();
		}
		else if (currentToken->token == Token::PRINT || currentToken->token == Token::PRINTLN) {
			funcs(); 
			tabs--; 
			statement();
		}
		else if (currentToken->token == Token::IF) {
			ifcmd(); 
			tabs--; 
			statement();
		}
		else if (currentToken->token == Token::WHILE) {
			whilecmd(); 
			tabs--; 
			statement();
		}
		else if (currentToken->token == Token::ID) {
			currentToken++;
			if (currentToken->token == Token::AS) {
				currentToken--;
				assignment(); 
				tabs--;
				statement();
			}
			else {
				currentToken--;
				expr();
				match(Token::SCOL);
				tabs--;
				statement();
			}
		}
		else if (currentToken->token == Token::SLC || currentToken->token == Token::MLC) {
			currentToken++; 
			printTabs();
			cout<<"Comment Ignored"<<endl;
			tabs--;
			statement();
		}
		else {
			//tabs--;
		}
	}
	else {}
}

void Parser::initialization() {
	printTabs();
	cout << "INITIALIZATION" << endl;
	tabs++;
	if (currentToken->token == Token::INT) {
		intinit();
	}
	else if (currentToken->token == Token::CHAR) {
		charinit();
	}

	tabs--;
}

void Parser::charid() {
	printTabs();
	cout << "CHARID" << endl;
	tabs++;

	match(Token::CHAR);
	match(Token::COL);
	match(Token::ID);

	tabs--;
}


void Parser::charinit() {
	printTabs();
	cout << "CHARINIT" << endl;
	tabs++;

	charid();
	
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
	printTabs();
	cout << "CHARINITLIST" << endl;
	tabs++;

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

void Parser::intid() {
	printTabs();
	cout << "INTID" << endl;
	tabs++;
	// includes intid
	match(Token::INT); 
	match(Token::COL);
	match(Token::ID);
	
	tabs--;
}


void Parser::intinit() {
	printTabs();
	cout << "INTINIT" << endl;
	tabs++;

	intid();

	currentToken--;
	if (datatypeTable.find(currentToken->lexeme) == datatypeTable.end()) {
		datatypeTable[currentToken->lexeme] = Token::INT;
	}
	currentToken++;

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
	printTabs();
	cout << "INTINITLIST" << endl;
	tabs++;

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
	printTabs();
	cout << "INPUT" <<endl;
	tabs++;

	match(Token::INPUT);
	match(Token::IN);
	match(Token::ID);
	match(Token::SCOL);
	tabs--;
}

void Parser::funcs() {
	printTabs();
	cout << "FUNCS" << endl;
	tabs++;

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
	printTabs();
	cout << "PARAMS" << endl;
	tabs++;

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
	printTabs();
	cout << "IF" << endl;
	tabs++;

	match(Token::IF);
	comparison();
	docmd();
	branch();

	tabs--;
}

void Parser::comparison() {
	printTabs();
	cout << "COMPARISON" << endl;
	tabs++;

	expr();
	RO();
	expr();

	tabs--;
}

void Parser::RO () {
	printTabs();
	cout << "RELATIONAL OP" << endl;
	tabs++;

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
	printTabs();
	cout << "DO" << endl;
	tabs++;

	match(Token::COL);
	match(Token::FBO);
	statement();
	match(Token::FBC);

	tabs--;
}

void Parser::branch() {
	printTabs();
	cout << "BRANCH" << endl;
	tabs++;

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
	printTabs();
	cout << "WHILE" << endl;
	tabs++;

	match(Token::WHILE);
	comparison();
	docmd();

	tabs--;
}

void Parser::expr() {
	printTabs();
	cout << "EXPRESSION" << endl;
	tabs++;


	T();
	R();

	tabs--;
}

void Parser::T() {
	printTabs();
	cout << "TERM" << endl;
	tabs++;

	F(); 
	Rprime();

	tabs--;
}

void Parser::R() {
	printTabs();
	cout << "RESIDUAL TERM" << endl;
	tabs++;

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
	printTabs();
	cout << "RESIDUAL \'" << endl;
	tabs++;

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
	printTabs();
	cout << "FACTOR" << endl;
	tabs++;

	if (currentToken->token == Token::ID) {
		currentToken++;
		if (currentToken->token == Token::INC) {
			currentToken--;
			match(Token::ID);
			match(Token::INC);
		}
		else if (currentToken->token == Token::DEC) {
			currentToken--;
			match(Token::ID);
			match(Token::DEC);
		}
		else {
			currentToken--;
			match(Token::ID);
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
	printTabs();
	cout << "ASSIGNMENT" << endl;
	tabs++;

	match(Token::ID);
	match(Token::AS);
	expr();
	match(Token::SCOL);

	tabs--;
}