#include "translator.h"
#include "../utils/tokenMap.h"
#include <string>
#include <sstream>

int bytesNeeded(Token x) {
	if (x == Token::INT) {return 4;}
	else if (x == Token::CHAR) {return 1;}
}

Pair Translator::newTemp(Token x) {
	Pair newpair;
	newpair.lexeme = "t"+this->tempCount;
	newpair.token = x;

	this->datatypeTable[newpair.lexeme] = x;
	this->addressTable[newpair.lexeme] = this->currentAddr;
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
			Pair expr_n = expr();

			//sdt
			stringstream ss;
			ss << newPair.lexeme << " = " <<expr_n.lexeme; 
			this->translation.push_back(ss.str());

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
		

		Pair param_n = params();
		
		//sdt
		stringstream ss;
		ss << "OUT " << param_n.lexeme ;
		this->translation.push_back(ss.str());
		
		match(Token::PC);
		match(Token::SCOL, false);
	}
	else if (currentToken->token == Token::PRINTLN) {
		match(Token::PRINTLN);
		match(Token::PO);
		
		Pair param_n = params();
		
		//sdt
		stringstream ss;
		ss << "OUT " << param_n.lexeme;
		this->translation.push_back(ss.str());
		
		ss << "OUT \"\\n\"";
		this->translation.push_back(ss.str());
		
		match(Token::PC);
		match(Token::SCOL, false);
	}
}

Pair Translator::params() {
	if (currentToken->token == Token::ID) {
		Pair newpair = *currentToken;	
		match(Token::ID, false);
		return newpair;
	}
	else if (currentToken->token == Token::STR) {
		Pair newpair = *currentToken;
		match(Token::STR, false);
		return newpair;
	}
	else if (currentToken->token == Token::LIT) {
		Pair newpair = *currentToken;	
		match(Token::LIT, false);
		return newpair;	
	}
	else if (currentToken->token == Token::NUM) {
		Pair newpair = *currentToken;
		match(Token::NUM, false);
		return newpair;
	}
	else {
		Pair expr_n = expr();
		return expr_n;
	}
}

void Translator::ifcmd() {
	match(Token::IF);
	string comp_n = comparison();
	
	stringstream ss;
	
	ss << "if "<< comp_n << " GOTO " << this->translation.size() + 2;
	this->translation.push_back(ss.str());

	ss.str("");

	ss << "GOTO ";
	this->translation.push_back(ss.str());

	int nextbranchgoto = this->translation.size() - 1;

	docmd();

	ss.str("");
	ss << "GOTO " <<;
	this->translation.push_back(ss.str());

	int afterlastbranchgoto = this->translation.size() - 1;
	this->translation[nextbranchgoto] += this->translation.size();

	branch();
	
	this->translation[afterlastbranchgoto] += this->translation.size(); 

}

string Translator::comparison() {
	Pair expr1_n = expr();
	Pair ro = RO();
	Pair expr2_n = expr();

	string temp = expr1_n.lexeme + " " + ro.lexeme + " " + expr2_n.lexeme;
	return temp;
}

Pair Translator::RO () {
	if (currentToken->token == Token::LT) {
		Pair newpair = *currentToken;
		match(Token::LT);
		return newpair;
	}
	else if (currentToken->token == Token::LTE) {
		Pair newpair = *currentToken;
		match(Token::LTE);
		return newpair;
	}
	else if (currentToken->token == Token::GT) {
		Pair newpair = *currentToken;
		match(Token::GT);
		return newpair;
	}
	else if (currentToken->token == Token::GTE) {
		Pair newpair = *currentToken;
		match(Token::GTE);
		return newpair;
	}
	else if (currentToken->token == Token::NEQ) {
		Pair newpair = *currentToken;
		match(Token::NEQ);
		return newpair;
	}
	else if (currentToken->token == Token::EQ) {
		Pair newpair = *currentToken;
		match(Token::EQ);
		return newpair;
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
		string comp_n = comparison();
	
		stringstream ss;
		
		ss << "if "<< comp_n << " GOTO " << this->translation.size() + 2;
		this->translation.push_back(ss.str());
		ss.str("");
		ss << "GOTO ";
		this->translation.push_back(ss.str());
		int nextbranchgoto = this->translation.size() - 1;

		docmd();

		ss.str("");
		ss << "GOTO " <<;
		this->translation.push_back(ss.str());
		int afterlastbranchgoto = this->translation.size() - 1;
		this->translation[nextbranchgoto] += this->translation.size();

		branch();

		this->translation[afterlastbranchgoto] += this->translation.size(); 		
	}
}

void Translator::whilecmd() {
	match(Token::WHILE);
	string comp_n = comparison();
	
	stringstream ss;

	ss << "if " << comp_n << "GOTO ";
	this->translation.push_back(ss.str());
	int getoutofloopgoto = this->translation.size();
	ss.str("");
	
	ss << "GOTO ";
	this->translation.push_back(ss.str());

	docmd();

	this->translation[getoutofloopgoto] += this->translation.size();
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
		ss << temp.lexeme << " = " << R_n.lexeme << " + " << T_n.lexeme;
		this->translation.push_back(ss.str());

		return R(temp);
		
	}
	else if (currentToken->token == Token::SUB) {
		match(Token::SUB);
		Pair T_n = T();

		stringstream ss;
		Pair temp = newtemp(Token::INT);
		ss << temp.lexeme << " = " << R_n.lexeme << " - " << T_n.lexeme;
		this->translation.push_back(ss.str());

		return R(temp);
	}
	else {
		return R_n;
	}
}

Pair Translator::Rprime(Pair RR_n) {
	if (currentToken->token == Token::MUL) {
		match(Token::MUL);
		Pair F_n = F();

		//sdt	
		stringstream ss;
		Pair temp = newtemp(Token::INT);
		ss << temp.lexeme << " = " << R_n.lexeme << " * " << T_n.lexeme;
		this->translation.push_back(ss.str());

		return Rprime(temp);
	}
	else if (currentToken->token == Token::DIV) {
		match(Token::DIV);
		F();

		//sdt	
		stringstream ss;
		Pair temp = newtemp(Token::INT);
		ss << temp.lexeme << " = " << R_n.lexeme << " / " << T_n.lexeme;
		this->translation.push_back(ss.str());

		return Rprime(temp);
	}
	else {
		return RR_n;
	}
}

Pair Translator::F() {
	if (currentToken->token == Token::ID) {
		currentToken++;
		if (currentToken->token == Token::INC) {
			currentToken--;
			Pair newpair = *currentToken;
			match(Token::ID);
			match(Token::INC, false);

			stringstream ss;
			ss << newpair.lexeme <<" = "<< newpair.lexeme << " + " << 1;
			this->translation.push_back(ss.str());
			return newpair;
		}
		else if (currentToken->token == Token::DEC) {
			currentToken--;
			Pair newpair = *currentToken;
			match(Token::ID);
			match(Token::DEC, false);

			stringstream ss;
			ss << newpair.lexeme <<" = "<< newpair.lexeme <<" - " << 1;
			this->translation.push_back(ss.str());
			return newpair;
		}
		else {
			currentToken--;
			Pair newpair = *currentToken;
			match(Token::ID, false);

			return newpair;
		}
	}
	else if (currentToken->token == Token::NUM) {
		Pair newpair = *currentToken;
		match(Token::NUM, false);
		return newpair;
	}
	else if (currentToken->token == Token::LIT) {
		Pair newpair = *currentToken;
		match(Token::LIT, false);
		return newpair;
	}
	else if (currentToken->token == Token::PO) {
		match(Token::PO);
		Pair expr_n = expr();
		match(Token::PC, false);
		return expr_n;
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
	this->translation.push_back(ss.str());
}