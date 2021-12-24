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

	fillInitValTable();
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
		this->initialvalTable[i.first] = "";
	}
}

void Gen::convert() {
	statement();
}

void Gen::statement() {
	for (auto i: this->translation) {
		stringstream ss(i);
		string temp;

		ss >> temp;

		if (temp == "IF") {

		}
		else if (temp == "OUT") {
			quad tempquad;
			string temp1;

			tempquad.opcode = Opcode::OUT;
			ss >> temp1;

			if (datatypeTable.find(temp1) == datatypeTable.end()) {
				tempquad.var1 = this->addressTable[temp1];
			}
			else {
				if (temp1[0] == "\'") { 
					//literal
				}
				else if (temp1[0] == '\"') { 

				}
			}
		}
		else if (temp == "IN") {
			quad tempquad;
			string temp1;

			tempquad.opcode = Opcode::IN;
			ss >> temp1;

			if (datatypeTable.find(temp1) != datatypeTable.end()) {
				throw std::runtime_error("Variable not found");
			}

			tempquad.var1 = this->addressTable[temp1];
			this->converted.push_back(tempquad);
		}
		else if (temp == "GO") {
			quad tempquad;

			tempquad.opcode = Opcode::GOTO;
			ss >> tempquad.var1;

			this->converted.push_back(tempquad);
		}	
		else {
			// maths
			
		}

	}
}