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
	stringstream ss;
	ss<<"t"<<this->tempCount++;
	newpair.lexeme = ss.str();
	newpair.token = x;

	this->datatypeTable[newpair.lexeme] = x;
	this->addressTable[newpair.lexeme] = this->currentAddr;
	this->currentAddr += bytesNeeded(x);

	return newpair;
}

Translator::Translator(vector<Pair> _TokenStream) {
	this->tokenStream = _TokenStream;
	this->currentToken = this->tokenStream.begin();
	this->tempCount = 0;
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

void Translator::PrintAddressTable() {
	cout << "----------------------------------" << endl;
	cout << "\tAddress Table" << endl << endl;
	cout << "ID Name" << "\t\t" << "Address" << endl << endl;
	for (auto entry : this->addressTable) {
		cout << entry.first<< "\t\t" << entry.second <<endl;
	}
	cout << "----------------------------------" << endl;
}

void Translator::PrintTranslation() {
	cout << "----------------------------------" << endl;
	cout << "\tTranslation Table" << endl << endl;
	for (int i = 0; i < this->translation.size(); i++) {
		cout<<i<<"\t"<<this->translation[i]<<endl;
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
		PrintAddressTable();
		PrintTranslation();
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
		addressTable[currentToken->lexeme] = currentAddr;
		this->currentAddr += bytesNeeded(Token::CHAR);
	}

	Pair newPair;
	newPair.token = currentToken->token;
	newPair.lexeme = currentToken->lexeme;
	currentToken++;

	return newPair;
}


Pair Translator::charinit() {
	Pair charid_n = charid();

	// charinit part
	if (currentToken->token == Token::AS) {
		match(Token::AS);

		//translation
		Pair expr_n = expr();
		stringstream ss;
		ss << charid_n.lexeme <<" = "<<expr_n.lexeme;
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
			this->currentAddr += bytesNeeded(Token::CHAR);
		}
		
		Pair newPair;
		newPair.token = currentToken->token;
		newPair.lexeme = currentToken->lexeme;
		currentToken++;

		if (currentToken->token == Token::AS) {
			match(Token::AS);
			Pair expr_n = expr();
			
			//sdt
			stringstream ss;
			ss<<newPair.lexeme<<" = "<<expr_n.lexeme;
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
		addressTable[currentToken->lexeme] = currentAddr;
		this->currentAddr += bytesNeeded(Token::INT);
	}

	Pair newPair;
	newPair.token = currentToken->token;
	newPair.lexeme = currentToken->lexeme;
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
		ss << intid_n.lexeme <<" = "<< expr_n.lexeme;
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
	
		Pair newPair;
		newPair.token = currentToken->token;
		newPair.lexeme = currentToken->lexeme;
		if (datatypeTable.find(currentToken->lexeme) == datatypeTable.end()) {
			datatypeTable[currentToken->lexeme] = Token::INT;
			addressTable[currentToken->lexeme] = currentAddr;
			this->currentAddr += bytesNeeded(Token::INT);
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
	Pair newPair;
	newPair.token = currentToken->token;
	newPair.lexeme = currentToken->lexeme;
	match(Token::ID);
	match(Token::SCOL, false);

	//sdt
	stringstream ss;
	ss << "IN" <<" "<< newPair.lexeme;
	this->translation.push_back(ss.str());
}

void Translator::funcs() {
	if (currentToken->token == Token::PRINT) {
		match(Token::PRINT);
		match(Token::PO);
		

		Pair param_n = params();
		
		//sdt
		stringstream ss;
		ss << "OUT ";
		if (param_n.token == Token::STR) {
			ss << "\""<< param_n.lexeme <<"\"";
		}
		else {
			ss << param_n.lexeme;
		}
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
		ss << "OUT ";
		if (param_n.token == Token::STR) {
			ss << "\""<< param_n.lexeme <<"\"";
		}
		else {
			ss << param_n.lexeme;
		}
		this->translation.push_back(ss.str());
		
		ss.str("");

		ss << "OUT \"\\n\"";
		this->translation.push_back(ss.str());
		
		match(Token::PC);
		match(Token::SCOL, false);
	}
}

Pair Translator::params() {
	if (currentToken->token == Token::ID) {
		Pair newPair;
		newPair.token = currentToken->token;
		newPair.lexeme = currentToken->lexeme;
		match(Token::ID, false);
		return newPair;
	}
	else if (currentToken->token == Token::STR) {
		Pair newPair;
		newPair.token = currentToken->token;
		newPair.lexeme = currentToken->lexeme;
		match(Token::STR, false);
		return newPair;
	}
	else if (currentToken->token == Token::LIT) {
		Pair newPair;
		newPair.token = currentToken->token;
		newPair.lexeme = currentToken->lexeme;
		match(Token::LIT, false);
		return newPair;	
	}
	else if (currentToken->token == Token::NUM) {
		Pair newPair;
		newPair.token = currentToken->token;
		newPair.lexeme = currentToken->lexeme;
		match(Token::NUM, false);
		return newPair;
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
	
	//starting if statement
	ss << "IF "<< comp_n << " GOTO " << this->translation.size() + 2;
	this->translation.push_back(ss.str());

	//the exit goto
	ss.str("");
	ss << "GOTO ";
	this->translation.push_back(ss.str());
	int exitGoto = this->translation.size() - 1;

	docmd();

	//backpatch the exit goto
	this->translation[exitGoto] += to_string(this->translation.size());

	branch();
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
		Pair newPair;
		newPair.token = currentToken->token;
		newPair.lexeme = currentToken->lexeme;
		
		match(Token::LT);
		return newPair;
	}
	else if (currentToken->token == Token::LTE) {
		Pair newPair;
		newPair.token = currentToken->token;
		newPair.lexeme = currentToken->lexeme;
		match(Token::LTE);
		return newPair;
	}
	else if (currentToken->token == Token::GT) {
		Pair newPair;
		newPair.token = currentToken->token;
		newPair.lexeme = currentToken->lexeme;
		match(Token::GT);
		return newPair;
	}
	else if (currentToken->token == Token::GTE) {
		Pair newPair;
		newPair.token = currentToken->token;
		newPair.lexeme = currentToken->lexeme;
		match(Token::GTE);
		return newPair;
	}
	else if (currentToken->token == Token::NEQ) {
		Pair newPair;
		newPair.token = currentToken->token;
		newPair.lexeme = currentToken->lexeme;
		match(Token::NEQ);
		return newPair;
	}
	else if (currentToken->token == Token::EQ) {
		Pair newPair;
		newPair.token = currentToken->token;
		newPair.lexeme = currentToken->lexeme;
		match(Token::EQ);
		return newPair;
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
		
		//starting if statement
		ss << "IF "<< comp_n << " GOTO " << this->translation.size() + 2;
		this->translation.push_back(ss.str());
		
		//the exit goto 
		ss.str("");
		ss << "GOTO ";
		this->translation.push_back(ss.str());
		int exitGoto = this->translation.size() - 1;

		docmd();

		//backpatch the exit goto
		this->translation[exitGoto] += to_string(this->translation.size());

		branch();
	}
}

void Translator::whilecmd() {
	match(Token::WHILE);
	string comp_n = comparison();
	
	stringstream ss;

	//the starting if statement
	int loopStartPosition = this->translation.size();
	ss << "IF " << comp_n << " GOTO " << this->translation.size() + 2;
	this->translation.push_back(ss.str());
	
	int exitGoto = this->translation.size();
	
	//loop exit goto
	ss.str("");
	ss << "GOTO ";
	this->translation.push_back(ss.str());

	docmd();

	//backpatch the exit goto
	this->translation[exitGoto] += to_string(this->translation.size() + 1);
	
	//the return goto
	ss.str("");
	ss << "GOTO " <<loopStartPosition;
	this->translation.push_back(ss.str());
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
		Pair temp = newTemp(Token::INT);
		ss << temp.lexeme << " = " << R_n.lexeme << " + " << T_n.lexeme;
		this->translation.push_back(ss.str());

		return R(temp);
		
	}
	else if (currentToken->token == Token::SUB) {
		match(Token::SUB);
		Pair T_n = T();

		stringstream ss;
		Pair temp = newTemp(Token::INT);
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
		Pair temp = newTemp(Token::INT);
		ss << temp.lexeme << " = " << RR_n.lexeme << " * " << F_n.lexeme;
		this->translation.push_back(ss.str());

		return Rprime(temp);
	}
	else if (currentToken->token == Token::DIV) {
		match(Token::DIV);
		Pair F_n = F();

		//sdt	
		stringstream ss;
		Pair temp = newTemp(Token::INT);
		ss << temp.lexeme << " = " << RR_n.lexeme << " / " << F_n.lexeme;
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
			Pair newPair;
			newPair.token = currentToken->token;
			newPair.lexeme = currentToken->lexeme;
			
			match(Token::ID);
			match(Token::INC, false);

			stringstream ss;
			ss << newPair.lexeme <<" = "<< newPair.lexeme << " + " << 1;
			this->translation.push_back(ss.str());
			return newPair;
		}
		else if (currentToken->token == Token::DEC) {
			currentToken--;
			Pair newPair;
			newPair.token = currentToken->token;
			newPair.lexeme = currentToken->lexeme;
			match(Token::ID);
			match(Token::DEC, false);

			stringstream ss;
			ss << newPair.lexeme <<" = "<< newPair.lexeme <<" - " << 1;
			this->translation.push_back(ss.str());
			return newPair;
		}
		else {
			currentToken--;
			Pair newPair;
			newPair.token = currentToken->token;
			newPair.lexeme = currentToken->lexeme;
			match(Token::ID, false);

			return newPair;
		}
	}
	else if (currentToken->token == Token::NUM) {
		Pair newPair;
		newPair.token = currentToken->token;
		newPair.lexeme = currentToken->lexeme;
		match(Token::NUM, false);
		return newPair;
	}
	else if (currentToken->token == Token::LIT) {
		Pair newPair;
		newPair.token = currentToken->token;
		newPair.lexeme = currentToken->lexeme;
		match(Token::LIT, false);
		return newPair;
	}
	else if (currentToken->token == Token::PO) {
		match(Token::PO);
		Pair expr_n = expr();
		match(Token::PC, false);
		return expr_n;
	}
}

void Translator::assignment() {
	Pair newPair;
	newPair.token = currentToken->token;
	newPair.lexeme = currentToken->lexeme;
	match(Token::ID);
	match(Token::AS);
	Pair expr_n = expr();
	match(Token::SCOL, false);
	
	//sdt
	stringstream ss;
	ss << newPair.lexeme <<" = " <<expr_n.lexeme;
	this->translation.push_back(ss.str());
}