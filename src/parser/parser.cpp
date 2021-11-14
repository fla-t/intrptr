#include "parser.h"
#include "../utils/tokenMap.h"
#define POP_TABS tabs.pop_back(); tabs.pop_back(); tabs.pop_back(); tabs.pop_back(); tabs.pop_back(); tabs.pop_back();
//"└──" , "├──" , "│" tabs.append("│  ");

string tabs;

void printTabs(bool continued = true) {
	// for (int i = 0; i < tabs; i++) { cout << "    "; }
	cout<<tabs;
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
		tabs.append("│  ");
		tabs.append("   ");

		if (currentToken->token == Token::INT || currentToken->token == Token::CHAR) {
			initialization();
			POP_TABS
			statement();
		}
		else if (currentToken->token == Token::INPUT) { 
			input(); 
			POP_TABS 
			statement();
		}
		else if (currentToken->token == Token::PRINT || currentToken->token == Token::PRINTLN) {
			funcs(); 
			POP_TABS 
			statement();
		}
		else if (currentToken->token == Token::IF) {
			ifcmd(); 
			POP_TABS 
			statement();
		}
		else if (currentToken->token == Token::WHILE) {
			whilecmd(); 
			POP_TABS 
			statement();
		}
		else if (currentToken->token == Token::ID) {
			currentToken++;
			if (currentToken->token == Token::AS) {
				currentToken--;
				assignment(); 
				POP_TABS
				statement();
			}
			else {
				currentToken--;
				expr();
				match(Token::SCOL, false);
				POP_TABS
				statement();
			}
		}
		else if (currentToken->token == Token::SLC || currentToken->token == Token::MLC) {
			currentToken++; 
			printTabs( false);
			cout<<"Comment Ignored"<<endl;
			POP_TABS
			statement();
		}
		else {
			POP_TABS
		}
	}
	else {}
}

void Parser::initialization() {
	printTabs( false);
	cout << "Initialization" << endl;
	tabs.append("   ");
	if (currentToken->token == Token::INT) {
		intinit();
	}
	else if (currentToken->token == Token::CHAR) {
		charinit();
	}

	POP_TABS
}

void Parser::charid() {
	printTabs();
	cout << "Charid" << endl;
	tabs.append("│  ");
	tabs.append("   ");

	match(Token::CHAR);
	match(Token::COL);
	match(Token::ID);

	POP_TABS
}


void Parser::charinit() {
	printTabs( false);
	cout << "Charinit" << endl;
	tabs.append("   ");

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
	POP_TABS
}

void Parser::charinitlist() {
	printTabs();
	cout << "CharInitList" << endl;
	tabs.append("│  ");
	tabs.append("   ");

// includes contidinit
	if (currentToken->token == Token::COM) {
		match(Token::COM);
		match(Token::ID, false);
		
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

	POP_TABS
}

void Parser::intid() {
	printTabs();
	cout << "Intid" << endl;
	tabs.append("│  ");
	tabs.append("   ");
	// includes intid
	match(Token::INT); 
	match(Token::COL);
	match(Token::ID);
	
	POP_TABS
}


void Parser::intinit() {
	printTabs( false);
	cout << "Intinit" << endl;
	tabs.append("   ");

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

	POP_TABS
}

void Parser::intinitlist() {
	printTabs();
	cout << "IntInitList" << endl;
	tabs.append("│  ");
	tabs.append("   ");

	// includes contidinit
	if (currentToken->token == Token::COM) {
		match(Token::COM);
		match(Token::ID, false);

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

	POP_TABS
}

void Parser::input() {
	printTabs(false);
	cout << "Input" <<endl;
	tabs.append("   ");

	match(Token::INPUT);
	match(Token::IN);
	match(Token::ID);
	match(Token::SCOL, false);
	POP_TABS
}

void Parser::funcs() {
	printTabs(false);
	cout << "Funcs" << endl;
	tabs.append("   ");

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

	POP_TABS
}

void Parser::params() {
	printTabs();
	cout << "Params" << endl;
	tabs.append("│  ");
	tabs.append("   ");

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

	POP_TABS
}

void Parser::ifcmd() {
	printTabs();
	cout << "IF" << endl;
	tabs.append("│  ");
	tabs.append("   ");

	match(Token::IF);
	comparison();
	docmd();
	branch();

	POP_TABS
}

void Parser::comparison() {
	printTabs();
	cout << "Comparison" << endl;
	tabs.append("│  ");
	tabs.append("   ");

	expr();
	RO();
	expr();

	POP_TABS
}

void Parser::RO () {
	printTabs();
	cout << "RelationalOp" << endl;
	tabs.append("│  ");
	tabs.append("   ");

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

	POP_TABS
}

void Parser::docmd () {
	printTabs();
	cout << "DO" << endl;
	tabs.append("│  ");
	tabs.append("   ");

	match(Token::COL);
	match(Token::FBO);
	statement();
	match(Token::FBC, false);

	POP_TABS
}

void Parser::branch() {
	printTabs();
	cout << "BRANCH" << endl;
	tabs.append("│  ");
	tabs.append("   ");

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
	
	POP_TABS
}

void Parser::whilecmd() {
	printTabs();
	cout << "WHILE" << endl;
	tabs.append("│  ");
	tabs.append("   ");

	match(Token::WHILE);
	comparison();
	docmd();

	POP_TABS
}

void Parser::expr() {
	printTabs();
	cout << "Expression" << endl;
	tabs.append("│  ");
	tabs.append("   ");


	T();
	R();

	POP_TABS
}

void Parser::T() {
	printTabs();
	cout << "Term" << endl;
	tabs.append("│  ");
	tabs.append("   ");

	F(); 
	Rprime();

	POP_TABS
}

void Parser::R() {
	printTabs( false);
	cout << "Residual Term" << endl;
	tabs.append("   ");

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
	
	POP_TABS
}

void Parser::Rprime() {
	printTabs( false);
	cout << "Residual \'" << endl;
	tabs.append("   ");

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
		
	POP_TABS
}

void Parser::F() {
	printTabs();
	cout << "Factor" << endl;
	tabs.append("│  ");
	tabs.append("   ");

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

	POP_TABS
}

void Parser::assignment() {
	printTabs(false);
	cout << "Assignment" << endl;
	tabs.append("   ");

	match(Token::ID);
	match(Token::AS);
	expr();
	match(Token::SCOL, false);

	POP_TABS
}