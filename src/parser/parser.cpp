#include "parser.h"
#include "../utils/tokenMap.h"
#include <string>

#define POP_PREFIX_8 prefix.erase(prefix.end()-9, prefix.end());
#define POP_PREFIX_3 prefix.erase(prefix.end()-4, prefix.end());

string CONT_BAR = "│  ";
string TAB_SPACE = "    ";
static string prefix;

void printTabs(bool continued = true) {
	cout<<prefix;
	cout << (continued ?  "├── " : "└── ");
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


void Parser::match(Token symbol, bool continued = true) {
	if (symbol == this->currentToken->token) {
		printTabs(continued);
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
		cout << "Statement" << endl;
		prefix.append(CONT_BAR);
		prefix.append(TAB_SPACE);

		if (currentToken->token == Token::INT || currentToken->token == Token::CHAR) {
			initialization();
			POP_PREFIX_8
			statement();
		}
		else if (currentToken->token == Token::INPUT) { 
			input(); 
			POP_PREFIX_8 
			statement();
		}
		else if (currentToken->token == Token::PRINT || currentToken->token == Token::PRINTLN) {
			funcs(); 
			POP_PREFIX_8 
			statement();
		}
		else if (currentToken->token == Token::IF) {
			ifcmd(); 
			POP_PREFIX_8 
			statement();
		}
		else if (currentToken->token == Token::WHILE) {
			whilecmd(); 
			POP_PREFIX_8 
			statement();
		}
		else if (currentToken->token == Token::ID) {
			currentToken++;
			if (currentToken->token == Token::AS) {
				currentToken--;
				assignment(); 
				POP_PREFIX_8
				statement();
			}
			else {
				currentToken--;
				expr();
				match(Token::SCOL, false);
				POP_PREFIX_8
				statement();
			}
		}
		else if (currentToken->token == Token::SLC || currentToken->token == Token::MLC) {
			currentToken++; 
			printTabs( false);
			cout<<"Comment Ignored"<<endl;
			POP_PREFIX_8
			statement();
		}
		else {
			//POP_PREFIX
		}
	}
	else {}
}

void Parser::initialization() {
	printTabs(false);
	cout << "Initialization" << endl;
	prefix.append(TAB_SPACE);
	if (currentToken->token == Token::INT) {
		intinit();
	}
	else if (currentToken->token == Token::CHAR) {
		charinit();
	}

	POP_PREFIX_3
}

void Parser::charid() {
	printTabs();
	cout << "Charid" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);

	match(Token::CHAR);
	match(Token::COL);
	match(Token::ID, false);

	POP_PREFIX_8
}


void Parser::charinit() {
	printTabs( false);
	cout << "Charinit" << endl;
	prefix.append(TAB_SPACE);

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
		match(Token::SCOL, false);
	}
	else {
		charinitlist();
		match(Token::SCOL, false);
	}	
	POP_PREFIX_3
}

void Parser::charinitlist() {
	if (currentToken->token == Token::COM || currentToken->token == Token::SCOL) {
		printTabs();
	}
	else {
		printTabs(false);
	}
	cout << "CharInitList" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);

// includes contidinit
	if (currentToken->token == Token::COM) {
		match(Token::COM);
		
		currentToken++;
		if (currentToken->token == Token::AS) {
			currentToken--;
			match(Token::ID);
		}
		else {
			currentToken--;
			match(Token::ID, false);
		}

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
	POP_PREFIX_8
}

void Parser::intid() {
	printTabs();
	cout << "Intid" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);
	// includes intid
	match(Token::INT); 
	match(Token::COL);
	match(Token::ID, false);
	
	POP_PREFIX_8
}


void Parser::intinit() {
	printTabs(false);
	cout << "Intinit" << endl;
	prefix.append(TAB_SPACE);

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
		match(Token::SCOL, false);
	}
	else {
		intinitlist();
		match(Token::SCOL, false);
	}	

	POP_PREFIX_3
}

void Parser::intinitlist() {
	if (currentToken->token == Token::COM || currentToken->token == Token::SCOL) {
		printTabs();
	}
	else {
		printTabs(false);
	}
	cout << "IntInitList" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);

	// includes contidinit
	if (currentToken->token == Token::COM) {
		match(Token::COM);
		currentToken++;
		if (currentToken->token == Token::AS) {
			currentToken--;
			match(Token::ID);
		}
		else {
			currentToken--;
			match(Token::ID, false);
		}

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

	POP_PREFIX_8
}

void Parser::input() {
	printTabs(false);
	cout << "Input" <<endl;
	prefix.append(TAB_SPACE);

	match(Token::INPUT);
	match(Token::IN);
	match(Token::ID);
	match(Token::SCOL, false);

	POP_PREFIX_3
}

void Parser::funcs() {
	printTabs(false);
	cout << "Funcs" << endl;
	prefix.append(TAB_SPACE);

	if (currentToken->token == Token::PRINT) {
		match(Token::PRINT);
		match(Token::PO);
		params();
		match(Token::PC);
		match(Token::SCOL, false);
	}
	else if (currentToken->token == Token::PRINTLN) {
		match(Token::PRINTLN);
		match(Token::PO);
		params();
		match(Token::PC);
		match(Token::SCOL, false);
	}

	POP_PREFIX_3
}

void Parser::params() {
	printTabs();
	cout << "Params" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);

	if (currentToken->token == Token::ID) {
		match(Token::ID, false);
	}
	else if (currentToken->token == Token::STR) {
		match(Token::STR, false);
	}
	else if (currentToken->token == Token::LIT) {
		match(Token::LIT, false);
	}
	else if (currentToken->token == Token::NUM) {
		match(Token::NUM, false);
	}
	else {
		expr();
	}

	POP_PREFIX_8
}

void Parser::ifcmd() {
	printTabs();
	cout << "IF" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);

	match(Token::IF);
	comparison();
	docmd();
	branch();

	POP_PREFIX_8
}

void Parser::comparison() {
	printTabs();
	cout << "Comparison" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);

	expr();
	RO();
	expr();

	POP_PREFIX_8
}

void Parser::RO () {
	printTabs();
	cout << "RelationalOp" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);

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

	POP_PREFIX_8
}

void Parser::docmd () {
	printTabs(false);
	cout << "DO" << endl;
	prefix.append(TAB_SPACE);

	match(Token::COL);
	match(Token::FBO);
	statement();
	match(Token::FBC, false);

	POP_PREFIX_3
}

void Parser::branch() {
	printTabs();
	cout << "BRANCH" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);

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
	
	POP_PREFIX_8
}

void Parser::whilecmd() {
	printTabs(false);
	cout << "WHILE" << endl;
	prefix.append(TAB_SPACE);

	match(Token::WHILE);
	comparison();
	docmd();

	POP_PREFIX_3
}

void Parser::expr() {
	printTabs();
	cout << "Expression" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);


	T();
	R();

	POP_PREFIX_8
}

void Parser::T() {
	printTabs();
	cout << "Term" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);

	F(); 
	Rprime();

	POP_PREFIX_8
}

void Parser::R() {
	printTabs( false);
	cout << "Residual Term" << endl;
	prefix.append(TAB_SPACE);

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
	
	POP_PREFIX_3
}

void Parser::Rprime() {
	printTabs( false);
	cout << "Residual \'" << endl;
	prefix.append(TAB_SPACE);

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
		
	POP_PREFIX_3
}

void Parser::F() {
	printTabs();
	cout << "Factor" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);

	if (currentToken->token == Token::ID) {
		currentToken++;
		if (currentToken->token == Token::INC) {
			currentToken--;
			match(Token::ID);
			match(Token::INC, false);
		}
		else if (currentToken->token == Token::DEC) {
			currentToken--;
			match(Token::ID);
			match(Token::DEC, false);
		}
		else {
			currentToken--;
			match(Token::ID, false);
		}
	}
	else if (currentToken->token == Token::NUM) {
		match(Token::NUM, false);
	}
	else if (currentToken->token == Token::LIT) {
		match(Token::LIT, false);
	}
	else if (currentToken->token == Token::PO) {
		match(Token::PO);
		expr();
		match(Token::PC, false);
	}

	POP_PREFIX_8
}

void Parser::assignment() {
	printTabs(false);
	cout << "Assignment" << endl;
	prefix.append(TAB_SPACE);

	match(Token::ID);
	match(Token::AS);
	expr();
	match(Token::SCOL, false);

	POP_PREFIX_3
}