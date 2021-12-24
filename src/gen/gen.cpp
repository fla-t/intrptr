#include "./gen.h"
#include <sstream>

int bytesNeeded(Token x) {
	if (x == Token::INT) {return 4;}
	else if (x == Token::CHAR) {return 1;}
}

int Gen::newTemp(int x) {
	stringstream ss;
	ss<<"t"<<this->tempCount++;
	string name = ss.str();

	this->datatypeTable[name] = Token::INT;
	this->addressTable[name] = this->currentAddr;
	this->currentAddr += 4;

	return this->addressTable[name];
}

int Gen::newTemp(string x) {
	stringstream ss;
	ss<<"t"<<this->tempCount++;
	string name = ss.str();

	this->datatypeTable[name] = Token::STR;
	this->addressTable[name] = this->currentAddr;
	this->currentAddr += x.size();

	return this->addressTable[name];
}

int Gen::newTemp(char x) {
	stringstream ss;
	ss<<"t"<<this->tempCount++;
	string name = ss.str();

	this->datatypeTable[name] = Token::LIT;
	this->addressTable[name] = this->currentAddr;
	this->currentAddr += 1;

	return this->addressTable[name];
}

Gen::Gen() {
	this->tempCount = 0;
}

Gen::Gen(map<string, Token> datatypeTable, map<string, int> addressTable, vector<string> translation, int tempCount, int currentAddr) {
	this->datatypeTable = datatypeTable;
	this->addressTable = addressTable;
	this->translation = translation;

	this->tempCount = tempCount;
	this->currentAddr = currentAddr;
}

Gen::Gen(Translator x) {
	this->datatypeTable = x.datatypeTable;
	this->addressTable = x.addressTable;
	this->translation = x.translation;

	this->tempCount = x.tempCount;
	this->currentAddr = x.currentAddr;
}

Gen::~Gen() {
	this->tempCount = 0;
}

void Gen::fillInitValTable() {
	for (auto i: this->datatypeTable) {
		this->initialvalTable[i->first] = "";
	}
}

void Gen::convert() {
	for (auto i: translation) {
		stringstream ss(i);
		string op;

		ss >> op;

		if (op == "IF") {

		}
		else if (op == "GOTO") {
			quad tempquad;
			tempquad.opcode = Opcode::GOTO;

			ss >> op;

			tempquad.var1 = stoi(op);
			this->converted.push_back(tempquad);
		}
		else if (op == "OUT") { 
				
		}
		else if (op == "IN") {
			quad tempquad;
			tempquad.opcode = Opcode::IN;	

			ss >> op;
			
			if (datatypeTable.find(op) == datatypeTable.end()) {
				
			}
			else {
				throw std::runtime_error("bad variable");
			}
		}
		else {
			quad tempquad;
			// confirming identifiers
			if (datatypeTable.find(op) == datatypeTable.end()){
				tempquad.store = this->addressTable[op];
				tempquad.opcode = Opcode::AS;

				ss >> op; // miss. always =
				ss >> op;

				if (datatypeTable.find(op) == datatypeTable.end()) {
					tempquad.var1 = this->addressTable[op];
				}
				else {	
					int addr;				
					if (op[0] == '\'') {
						//char
						char temp = op[1];
						addr = newTemp(temp);
					}
					else if (op[0] == '\"') {
						//string
						string temp = op.substr(1, op.size()-2);
						addr = newTemp(temp);
					}
					else {
						//int 
						int temp = stoi(op);
						addr = newTemp(temp);
					}

					tempquad.var1 = addr;
				}
			}

			if (!ss.eof()) {
				ss >> op;

				if (op == "+" ) {
					tempquad.opcode = Opcode::ADD;
				}
				else if (op == "-") {
					tempquad.opcode = Opcode::SUB;
				}
				else if (op == "/") {
					tempquad.opcode = Opcode::DIV;
				}
				else if (op == "*") {
					tempquad.opcode = Opcode::MUL;
				}

				ss >> op;

				if (datatypeTable.find(op) == datatypeTable.end()) {
					tempquad.var2 = this->addressTable[op];
				}
				else {	
					int addr;				
					if (op[0] == '\'') {
						//char
						char temp = op[1];
						addr = newTemp(temp);
					}
					else if (op[0] == '\"') {
						//string
						string temp = op.substr(1, op.size()-2);
						addr = newTemp(temp);
					}
					else {
						//int 
						int temp = stoi(op);
						addr = newTemp(temp);
					}

					tempquad.var2 = addr;
				}	
			}

			this->converted.push_back(tempquad);
		}
	}
}