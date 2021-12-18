#include "translator.h"
#include "../utils/tokenMap.h"
#include <string>
#include <sstream>

int bytesNeeded(Token x) {
	if (x == Token::INT) {return 4;}
	else if (x == Token::CHAR) {return 1;}
}

Pair newTemp(Token x) {
	Pair newpair;
	newpair.lexeme = "t"+this->tempCount;
	newpair.token = x;

	datatypeTable[temp] = x;
	addressTable[temp] = this->currentAddr;
	this->currentAddr += bytesNeeded(x);

	return newpair;
}

Translator::Translator(vector<Pair> _TokenStream) {
	this->tokenStream = _TokenStream;
	this->currentToken = this->tokenStream.begin();

	this->currentAddr = 0;
}

void Translator::PrintDataTypeTable() {
	cout << "----------------------------------" << endl;
	cout << "\tDatatype Table" << endl << endl;
	cout << "ID Name" << "\t\t" << "DataType" << endl << endl;
	for (auto entry : this->datatypeTable) {
		cout << entry.first<< "\t\t" << token_to_symbol[entry.second] <<endl;
	}
	cout << "----------------------------------" << endl;
}

void Translator::Parse() {
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


void Translator::match(Token symbol, bool continued = true) {
	if (symbol == this->currentToken->token) {
		this->currentToken++;
	}
	else {
		stringstream error;
		error << "Bad token, Unexpected: " << token_to_symbol[currentToken->token] << endl; 
		throw std::runtime_error(error.str().c_str());
	}
}

void Translator::statement() {
	if (currentToken != tokenStream.end()) {
		
		if (currentToken->token == Token::INT || currentToken->token == Token::CHAR) {
			initialization();
			statement();
		}
		else if (currentToken->token == Token::INPUT) { 
			input(); 
			statement();
		}
		else if (currentToken->token == Token::PRINT || currentToken->token == Token::PRINTLN) {
			funcs(); 
			statement();
		}
		else if (currentToken->token == Token::IF) {
			ifcmd(); 
			statement();
		}
		else if (currentToken->token == Token::WHILE) {
			whilecmd(); 
			statement();
		}
		else if (currentToken->token == Token::ID) {
			currentToken++;
			if (currentToken->token == Token::AS) {
				currentToken--;
				assignment(); 
				statement();
			}
			else {
				currentToken--;
				expr();
				match(Token::SCOL, false);
				statement();
			}
		}
		else if (currentToken->token == Token::SLC || currentToken->token == Token::MLC) {
			currentToken++; 
			statement();
		}
		else {
			//POP_PREFIX
		}
	}
	else {}
}

void Translator::initialization() {
	if (currentToken->token == Token::INT) {
		intinit();
	}
	else if (currentToken->token == Token::CHAR) {
		charinit();
	}
}

Pair Translator::charid() {
	match(Token::CHAR);
	match(Token::COL);
	match(Token::ID, false);
	
	//sdt things
	currentToken--;

	if (datatypeTable.find(currentToken->lexeme) == datatypeTable.end()) {
		datatypeTable[currentToken->lexeme] = Token::CHAR;
		addressTable[currentToken->lexeme] = this->currentAddr;
		this->currentAddr += bytesNeeded(currentToken->token);
	}

	Pair newpair = *currentToken;
	currentToken++;

	return newpair;
}


Pair Translator::charinit() {
	Pair charid_n = charid();

	// charinit part
	if (currentToken->token == Token::AS) {
		match(Token::AS);

		//translation
		Pair expr_n = expr();
		stringstream ss;
		ss<<charid_n.lexeme<<" = "<<expr_n.lexeme;
		this->translation.push_back(ss.str());
		//end

		charinitlist();
		match(Token::SCOL, false);
	}
	else {

		charinitlist();
		match(Token::SCOL, false);
	}	
}

Pair Translator::charinitlist() {

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
			addressTable[currentToken->lexeme] = this->currentAddr;
			this->currentAddr += bytesNeeded(currentToken->token);
		}
		Pair id = *currentToken;
		currentToken++;

		if (currentToken->token == Token::AS) {
			match(Token::AS);
			Pair expr_n = expr();
			
			//sdt
			stringstream ss;
			ss<<id.lexeme<<" = "<<expr_n.lexeme;
			this->translation.push_back(ss.str());

			charinitlist();
		}
		else {
			charinitlist();
		}
	}
}

Pair Translator::intid() {
	// includes intid
	match(Token::INT); 
	match(Token::COL);
	match(Token::ID, false);
	
	currentToken--;
	if (datatypeTable.find(currentToken->lexeme) == datatypeTable.end()) {
		datatypeTable[currentToken->lexeme] = Token::INT;
		addressTable[currentToken->lexeme] = this->currentAddr;
		this->currentAddr += bytesNeeded(currentToken->token);
	}

	Pair newPair = *currentToken;
	currentToken++;
	
	return newPair;
}


Pair Translator::intinit() {
	Pair intid_n = intid();

	if (currentToken->token == Token::AS) {
		match(Token::AS);
		Pair expr_n = expr();
		
		//sdt
		stringstream ss;
		ss << expr_n.lexeme <<" = "<< expr_n.lexeme;
		this->translation.push_back(ss.str());

		intinitlist();
		match(Token::SCOL, false);
	}
	else {
		intinitlist();
		match(Token::SCOL, false);
	}	
}

void Translator::intinitlist() {
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
		Pair newPair = *currentToken;
		if (datatypeTable.find(currentToken->lexeme) == datatypeTable.end()) {
			datatypeTable[currentToken->lexeme] = Token::INT;
			addressTable[currentToken->lexeme] = this->currentAddr;
			this->currentAddr += bytesNeeded(currentToken->token);
		}
		currentToken++;

		if (currentToken->token == Token::AS) {
			match(Token::AS);
			int expr_n = expr();

			//sdt
			stringstream ss;
			ss << newPair.lexeme << " = " <<expr_n.lexeme; 
			this->tanslation.push_back(ss.str());

			intinitlist();
		}
		else {
			intinitlist();
		}
	}
}

void Translator::input() {
	match(Token::INPUT);
	match(Token::IN);
	Pair newpair = *currentToken;
	match(Token::ID);
	match(Token::SCOL, false);

	//sdt
	stringstream ss;
	ss << "IN" <<" "<< newpair.lexeme;
	this->translation.push_back(ss.str());
}

void Translator::funcs() {
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
}

void Translator::params() {
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
}

void Translator::ifcmd() {
	match(Token::IF);
	comparison();
	docmd();
	branch();
}

void Translator::comparison() {
	expr();
	RO();
	expr();
}

void Translator::RO () {
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

void Translator::docmd () {
	match(Token::COL);
	match(Token::FBO);
	statement();
	match(Token::FBC, false);
}

void Translator::branch() {
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
}

void Translator::whilecmd() {
	match(Token::WHILE);
	comparison();
	docmd();
}

Pair Translator::expr() {
	Pair T_n = T();
	Pair expr_n = R(T_n);
	return expr_n;
}

Pair Translator::T() {
	Pair F_n = F(); 
	Pair T_n = Rprime(F_n);
	return T_n;
}

Pair Translator::R(Pair R_n) {
	if (currentToken->token == Token::ADD) {
		match(Token::ADD);
		Pair T_n = T();
		
		//sdt	
		stringstream ss;
		Pair temp = newtemp(Token::INT);
		ss << temp << " = " << R_n.lexeme << " + " << T_n.lexeme;
		this->translation.push_back(ss.str());

		R(temp);
		return temp;
	}
	else if (currentToken->token == Token::SUB) {
		match(Token::SUB);
		Pair T_n = T();

		stringstream ss;
		Pair temp = newtemp(Token::INT);
		ss << temp << " = " << R_n.lexeme << " - " << T_n.lexeme;
		this->translation.push_back(ss.str());

		R(temp);
		return temp;
	}
}

Pair Translator::Rprime() {
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
}

void Translator::F() {
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
}

void Translator::assignment() {
	Pair newpair = *currentToken;
	match(Token::ID);
	match(Token::AS);
	Pair expr_n = expr();
	match(Token::SCOL, false);
	
	//sdt
	stringstream ss;
	ss << newpair.lexeme <<" = " <<expr_n.lexeme;
	this->translation(ss.str());
}