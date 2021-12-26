#include "translator.h"
#include "../utils/tokenMap.h"
#include <string>
#include <sstream>

#define POP_PREFIX_8 prefix.erase(prefix.end()-9, prefix.end());
#define POP_PREFIX_3 prefix.erase(prefix.end()-4, prefix.end());

string CONT_BAR = "│  ";
string TAB_SPACE = "    ";
static string prefix;

void printTabs(bool continued = true) {
	cout<<prefix;
	cout << (continued ?  "├── " : "└── ");
}

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
		printTabs(continued);
		if (currentToken->token == Token::ID) {
			cout << currentToken->lexeme << endl;
		}
		else 
			cout << token_to_symbol[currentToken->token] << endl;
		this->currentToken++;
	}
	else {
		stringstream error;
		error << "Bad token, Unexpected: " << token_to_symbol[currentToken->token] << endl; 
		throw std::runtime_error(error.str().c_str());
	}
}


bool Translator::match(Pair s1, Pair s2) {
	// id id match	
	if (s1.token == s2.token) {
		return true;
	}
	else if (s1.token == Token::ID && s2.token != Token::ID) {
		switch(s2.token) {
			case Token::INT:
			case Token::NUM:
				if (datatypeTable[s1.lexeme] == Token::INT) {return true;} 
				break;
			
			case Token::CHAR:
			case Token::LIT:
				if (datatypeTable[s1.lexeme] == Token::CHAR) {return true;}
				break;

			default:
				return false;
		}
	}
	else if (s2.token == Token::ID && s1.token != Token::ID) {
		switch(s1.token) {
			case Token::INT:
			case Token::NUM:
				if (this->datatypeTable[s2.lexeme] == Token::INT) {return true;}
				break;
			
			case Token::CHAR:
			case Token::LIT:
				if (this->datatypeTable[s2.lexeme] == Token::CHAR) {return true;}
				break;

			default:
				return false;
		}
	}
	else if (s2.token == Token::ID && s1.token == Token::ID) {
		return (datatypeTable[s1.lexeme] == datatypeTable[s2.lexeme]);
	}
	return false;
}

void Translator::matchPair(Pair x, Pair y) {
	if (!match(x, y)) {
		stringstream error;
		error << "Type Mismatch Error" << endl; 
		throw std::runtime_error(error.str().c_str());
	}
}

void Translator::statement() {
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

void Translator::initialization() {
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

Pair Translator::charid() {
	printTabs();
	cout << "Charid" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);

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

	POP_PREFIX_8

	return newPair;
}


void Translator::charinit() {
	printTabs( false);
	cout << "Charinit" << endl;
	prefix.append(TAB_SPACE);

	Pair charid_n = charid();

	// charinit part
	if (currentToken->token == Token::AS) {
		match(Token::AS);

		//translation
		Pair expr_n = expr();
		matchPair(expr_n, charid_n);

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

	POP_PREFIX_3	
}

void Translator::charinitlist() {
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
			matchPair(expr_n, newPair);

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

	POP_PREFIX_8
}

Pair Translator::intid() {
	printTabs();
	cout << "Intid" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);

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
	
	POP_PREFIX_8

	return newPair;
}


void Translator::intinit() {
	printTabs(false);
	cout << "Intinit" << endl;
	prefix.append(TAB_SPACE);

	Pair intid_n = intid();

	if (currentToken->token == Token::AS) {
		match(Token::AS);
		Pair expr_n = expr();
		matchPair(expr_n, intid_n);

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

	POP_PREFIX_3
}

void Translator::intinitlist() {
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
			matchPair(expr_n, newPair);

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

	POP_PREFIX_8
}

void Translator::input() {
	printTabs(false);
	cout << "Input" <<endl;
	prefix.append(TAB_SPACE);

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

	POP_PREFIX_3
}

void Translator::funcs() {
	printTabs(false);
	cout << "Funcs" << endl;
	prefix.append(TAB_SPACE);


	if (currentToken->token == Token::PRINT) {
		match(Token::PRINT);
		match(Token::PO);
		

		Pair param_n = params();
		
		//sdt
		stringstream ss;
		ss << "OUT ";
		if (param_n.token == Token::STR) {
			ss << param_n.lexeme;
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
			ss << param_n.lexeme;
		}
		else {
			ss << param_n.lexeme;
		}
		this->translation.push_back(ss.str());
		
		ss.str("");

		ss << "OUT \'\n\'";
		this->translation.push_back(ss.str());
		
		match(Token::PC);
		match(Token::SCOL, false);
	}

	POP_PREFIX_3
}

Pair Translator::params() {
	printTabs();
	cout << "Params" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);

	Pair newPair;
	newPair.token = currentToken->token;
	newPair.lexeme = currentToken->lexeme;

	if (currentToken->token == Token::ID) {
		match(Token::ID, false);
		POP_PREFIX_8
		return newPair;
	}
	else if (currentToken->token == Token::STR) {
		match(Token::STR, false);
		POP_PREFIX_8
		return newPair;
	}
	else if (currentToken->token == Token::LIT) {
		match(Token::LIT, false);
		POP_PREFIX_8
		return newPair;	
	}
	else if (currentToken->token == Token::NUM) {
		match(Token::NUM, false);
		POP_PREFIX_8
		return newPair;
	}
	else {
		Pair expr_n = expr();
		POP_PREFIX_8
		return expr_n;
	}
}

void Translator::ifcmd() {
	printTabs();
	cout << "IF" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);

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

	POP_PREFIX_8
}

string Translator::comparison() {
	printTabs();
	cout << "Comparison" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);

	Pair expr1_n = expr();
	Pair ro = RO();
	Pair expr2_n = expr();
	matchPair(expr1_n, expr2_n);

	POP_PREFIX_8
	
	string temp = expr1_n.lexeme + " " + ro.lexeme + " " + expr2_n.lexeme;
	return temp;

}

Pair Translator::RO () {
	printTabs();
	cout << "RelationalOp" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);

	Pair newPair;
	newPair.token = currentToken->token;
	newPair.lexeme = currentToken->lexeme;

	if (currentToken->token == Token::LT) {
		match(Token::LT);
		POP_PREFIX_8
		return newPair;
	}
	else if (currentToken->token == Token::LTE) {
		match(Token::LTE);
		return newPair;
	}
	else if (currentToken->token == Token::GT) {
		match(Token::GT);
		POP_PREFIX_8
		return newPair;
	}
	else if (currentToken->token == Token::GTE) {
		match(Token::GTE);
		POP_PREFIX_8
		return newPair;
	}
	else if (currentToken->token == Token::NEQ) {
		match(Token::NEQ);
		POP_PREFIX_8
		return newPair;
	}
	else if (currentToken->token == Token::EQ) {
		match(Token::EQ);
		POP_PREFIX_8
		return newPair;
	}

}

void Translator::docmd () {
	printTabs(false);
	cout << "DO" << endl;
	prefix.append(TAB_SPACE);

	match(Token::COL);
	match(Token::FBO);
	statement();
	match(Token::FBC, false);

	POP_PREFIX_3
}

void Translator::branch() {
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

	POP_PREFIX_8
}

void Translator::whilecmd() {
	printTabs(false);
	cout << "WHILE" << endl;
	prefix.append(TAB_SPACE);

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

	POP_PREFIX_3
}

Pair Translator::expr() {
	printTabs();
	cout << "Expression" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);


	Pair T_n = T();
	Pair expr_n = R(T_n);

	POP_PREFIX_8
	
	return expr_n;
}

Pair Translator::T() {
	printTabs();
	cout << "Term" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);

	Pair F_n = F(); 
	Pair T_n = Rprime(F_n);
	
	POP_PREFIX_8

	return T_n;
}

Pair Translator::R(Pair R_n) {
	printTabs( false);
	cout << "Residual Term" << endl;
	prefix.append(TAB_SPACE);

	if (currentToken->token == Token::ADD) {
		match(Token::ADD);
		Pair T_n = T();
		
		//sdt	
		stringstream ss;
		Pair temp = newTemp(R_n.token);
		ss << temp.lexeme << " = " << R_n.lexeme << " + " << T_n.lexeme;
		this->translation.push_back(ss.str());

		return R(temp);
		
	}
	else if (currentToken->token == Token::SUB) {
		match(Token::SUB);
		Pair T_n = T();

		stringstream ss;
		Pair temp = newTemp(R_n.token);
		ss << temp.lexeme << " = " << R_n.lexeme << " - " << T_n.lexeme;
		this->translation.push_back(ss.str());

		return R(temp);
	}
	
	POP_PREFIX_3
	
	return R_n;
}

Pair Translator::Rprime(Pair RR_n) {
	printTabs( false);
	cout << "Residual \'" << endl;
	prefix.append(TAB_SPACE);

	if (currentToken->token == Token::MUL) {
		match(Token::MUL);
		Pair F_n = F();

		//sdt	
		stringstream ss;
		Pair temp = newTemp(RR_n.token);
		ss << temp.lexeme << " = " << RR_n.lexeme << " * " << F_n.lexeme;
		this->translation.push_back(ss.str());

		return Rprime(temp);
	}
	else if (currentToken->token == Token::DIV) {
		match(Token::DIV);
		Pair F_n = F();

		//sdt	
		stringstream ss;
		Pair temp = newTemp(RR_n.token);
		ss << temp.lexeme << " = " << RR_n.lexeme << " / " << F_n.lexeme;
		this->translation.push_back(ss.str());

		return Rprime(temp);
	}
	
	POP_PREFIX_3
	
	return RR_n;
}

Pair Translator::F() {
	printTabs();
	cout << "Factor" << endl;
	prefix.append(CONT_BAR);
	prefix.append(TAB_SPACE);

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
			POP_PREFIX_8
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
			POP_PREFIX_8
			return newPair;
		}
		else {
			currentToken--;
			Pair newPair;
			newPair.token = currentToken->token;
			newPair.lexeme = currentToken->lexeme;
			match(Token::ID, false);
			POP_PREFIX_8
			return newPair;
		}
	}
	else if (currentToken->token == Token::NUM) {
		Pair newPair;
		newPair.token = currentToken->token;
		newPair.lexeme = currentToken->lexeme;
		match(Token::NUM, false);
		POP_PREFIX_8
		return newPair;
	}
	else if (currentToken->token == Token::LIT) {
		Pair newPair;
		newPair.token = currentToken->token;
		newPair.lexeme = currentToken->lexeme;
		match(Token::LIT, false);
		POP_PREFIX_8
		return newPair;
	}
	else if (currentToken->token == Token::PO) {
		match(Token::PO);
		Pair expr_n = expr();
		match(Token::PC, false);
		POP_PREFIX_8
		return expr_n;
	}
}

void Translator::assignment() {
	printTabs(false);
	cout << "Assignment" << endl;
	prefix.append(TAB_SPACE);

	Pair newPair;
	newPair.token = currentToken->token;
	newPair.lexeme = currentToken->lexeme;
	match(Token::ID);
	match(Token::AS);
	Pair expr_n = expr();
	matchPair(expr_n, newPair);
	match(Token::SCOL, false);
	
	//sdt
	stringstream ss;
	ss << newPair.lexeme <<" = " <<expr_n.lexeme;
	this->translation.push_back(ss.str());

	POP_PREFIX_3
}