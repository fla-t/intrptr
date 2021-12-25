#include "./gen.h"
#include <sstream>
#include <algorithm>

map<Opcode, string> opcode_to_string {
	{ Opcode::ADD, "add" },		 	// addition
	{ Opcode::MUL, "mul"}, 			// multiply
	{ Opcode::DIV, "div"}, 			// division
	{ Opcode::SUB, "sub"}, 			// subtraction
	{ Opcode::AS, "assign"},				// assignment
	{ Opcode::LT, "LT"}, 			// less than
	{ Opcode::LTE, "LTE" }, 			// less than equal
	{ Opcode::GT, "GT"}, 			// greater than 
	{ Opcode::GTE, "GTE"}, 			// greater than equal
	{ Opcode::EQ, "EQ"}, 			// equal
	{ Opcode::NEQ, "NEQ"}, 			// not equal
	{ Opcode::OUT, "OUT"}, 			// print()
	{ Opcode::IN, "IN"}, 			// input symbol
	{ Opcode::GOTO, "GOTO"}			// goto 
};

bool cmp(pair<string, int>& a, pair<string, int>& b) {
    return a.second < b.second;
}
  
vector<pair<string, int>> sort(map<string, int>& M) {
  
    vector<pair<string, int>> A;
    for (auto& it : M) {
        A.push_back(it);
    }

    sort(A.begin(), A.end(), cmp);
	return A;
}

string stringExtract(stringstream& ss) {
	char buffer[100];
	ss.getline(buffer, 100);
	string temp(buffer);
	return temp.substr(1, temp.size() - 1);
}

int Gen::newTemp(int x) {
	stringstream ss;
	ss<<"t"<<this->tempCount++;
	string name = ss.str();

	this->datatypeTable[name] = Token::INT;
	this->addressTable[name] = this->currentAddr;
	this->initialvalTable[name] = to_string(x);
	this->currentAddr += 4;

	return this->addressTable[name];
}

int Gen::newTemp(string x) {
	stringstream ss;
	ss<<"t"<<this->tempCount++;
	string name = ss.str();

	this->datatypeTable[name] = Token::STR;
	this->addressTable[name] = this->currentAddr;
	this->initialvalTable[name] = (x+"\0");
	this->currentAddr += x.size() + 1;

	return this->addressTable[name];
}

int Gen::newTemp(char x) {
	stringstream ss;
	ss<<"t"<<this->tempCount++;
	string name = ss.str();

	this->datatypeTable[name] = Token::LIT;
	this->addressTable[name] = this->currentAddr;
	string temp(1, x);
	this->initialvalTable[name] = temp;
	this->currentAddr += 1;

	return this->addressTable[name];
}

// void Gen::PrintDataTypeTable() {
// 	cout << "----------------------------------" << endl;
// 	cout << "\tDatatype Table" << endl << endl;
// 	cout << "ID Name" << "\t\t" << "DataType" << endl << endl;
// 	for (auto entry : this->datatypeTable) {
// 		cout << entry.first<< "\t\t" << token_to_symbol[entry.second] <<endl;
// 	}
// 	cout << "----------------------------------" << endl;
// }

void Gen::PrintInitValTable() {
	cout << "----------------------------------" << endl;
	cout << "\tInitVal Table" << endl << endl;
	cout << "Name" << "\t\t" << "Value" << endl << endl;
	for (auto entry : this->initialvalTable) {
		cout << entry.first<< "\t\t" << entry.second << endl;
	}
	cout << "----------------------------------" << endl;
}

void Gen::PrintAddressTable() {
	cout << "----------------------------------" << endl;
	cout << "\tAddress Table" << endl << endl;
	cout << "ID Name" << "\t\t" << "Address" << endl << endl;
	for (auto entry : this->addressTable) {
		cout << entry.first<< "\t\t" << entry.second <<endl;
	}
	cout << "----------------------------------" << endl;
}

Gen::Gen() {
	this->tempCount = 0;
}

Gen::Gen(Translator x) {
	this->datatypeTable = x.datatypeTable;
	this->addressTable = x.addressTable;
	this->translation = x.translation;

	this->tempCount = x.tempCount;
	this->currentAddr = x.currentAddr;

	fillInitValTable();
}

Gen::~Gen() {
	this->tempCount = 0;
}

void Gen::fillInitValTable() {
	for (auto i: this->datatypeTable) {
		this->initialvalTable[i.first] = "";
	}
}

void Gen::PrintQuadTable() {
	cout<<"-------------------------------"<<endl;
	cout<<"OPCODE\tVAR1\tVAR2\tVAR3/Store"<<endl;
	for (auto i: this->converted) {
		cout<<opcode_to_string[i.opcode]<<'\t';
		cout<<i.var1<<'\t';
		cout<<i.var2<<'\t';
		cout<<i.store<<'\t'<<endl;
	}
}

void Gen::convert() {
	for (auto i: this->translation) {
		stringstream ss(i);
		string temp;

		ss >> temp;

		if (temp == "IF") {
			quad tempquad;
			string temp1;
			
			ss >> temp1;
			if (datatypeTable.find(temp1) != datatypeTable.end()) {
				tempquad.var1 = this->addressTable[temp1];
			}
			else {
				if (temp1[0] == '\'') { 
					//literal
					tempquad.var1 = newTemp((char)temp1[1]);
				}
				else if (temp1[0] == '\"') { 
					//str
					throw std::runtime_error("Strings can't be a part of comparison");
				}
				else {
					tempquad.var1 = newTemp(stoi(temp1));
				}
			}

			ss >> temp1;
			if (temp1 == "==") { tempquad.opcode = Opcode::EQ; }
			else if (temp1 == "<=") { tempquad.opcode = Opcode::LTE; }
			else if (temp1 == ">=") { tempquad.opcode = Opcode::GTE; }
			else if (temp1 == "<") { tempquad.opcode = Opcode::LT; }
			else if (temp1 == ">") { tempquad.opcode = Opcode::GT; }
			else if (temp1 == "~=") { tempquad.opcode = Opcode::NEQ; }

			ss >> temp1;
			if (datatypeTable.find(temp1) != datatypeTable.end()) {
				tempquad.var2 = this->addressTable[temp1];
			}
			else {
				if (temp1[0] == '\'') { 
					//literal
					tempquad.var2 = newTemp((char)temp1[1]);
				}
				else if (temp1[0] == '\"') { 
					//str
					throw std::runtime_error("Strings can't be a part of comparison");
				}
				else {
					tempquad.var2 = newTemp(stoi(temp1));
				}
			}

			ss >> temp1;
			if (temp1 == "GOTO") { 
				ss >> temp1;
				tempquad.store = stoi(temp1);
			}

			this->converted.push_back(tempquad);
		}

		else if (temp == "OUT") {
			quad tempquad;
			string temp1;

			tempquad.opcode = Opcode::OUT;
			temp1 = stringExtract(ss);

			if (datatypeTable.find(temp1) != datatypeTable.end()) {
				tempquad.var1 = this->addressTable[temp1];
			}
			else {
				if (temp1[0] == '\'') { 
					//literal
					tempquad.var1 = newTemp((char)temp1[1]);
				}
				else if (temp1[0] == '\"') { 
					//str
					tempquad.var1 = newTemp(temp1.substr(1, temp1.size()-2));
					tempquad.opcode = Opcode::OUTSTR;
				}
				else {
					tempquad.var1 = newTemp(stoi(temp1));
				}
			}

			this->converted.push_back(tempquad);
		}
		else if (temp == "IN") {
			quad tempquad;
			string temp1;

			tempquad.opcode = Opcode::IN;
			ss >> temp1;

			if (datatypeTable.find(temp1) == datatypeTable.end()) {
				throw std::runtime_error("Variable not found");
			}

			tempquad.var1 = this->addressTable[temp1];
			this->converted.push_back(tempquad);
		}
		else if (temp == "GOTO") {
			quad tempquad;

			tempquad.opcode = Opcode::GOTO;
			ss >> tempquad.var1;

			this->converted.push_back(tempquad);
		}	
		else {
			// maths
			quad tempquad;
			string temp1;

			tempquad.store = this->addressTable[temp];
			tempquad.opcode = Opcode::AS;

			ss >> temp1; //assignment "=" skipped
			ss >> temp1;
			if (datatypeTable.find(temp1) != datatypeTable.end()) {
				tempquad.var1 = this->addressTable[temp1];
			}
			else {
				if (temp1[0] == '\'') { 
					//literal
					tempquad.var1 = newTemp((char)temp1[1]);
				}
				else {
					tempquad.var1 = newTemp(stoi(temp1));
				}
			}

			if (!ss.eof()) {	
				ss >> temp1;
				if (temp1 == "+") { tempquad.opcode = Opcode::ADD; }
				else if (temp1 == "/") { tempquad.opcode = Opcode::DIV; }
				else if (temp1 == "-") { tempquad.opcode = Opcode::SUB; }
				else if (temp1 == "*") { tempquad.opcode = Opcode::MUL; }

				ss >> temp1;
				if (datatypeTable.find(temp1) != datatypeTable.end()) {
					tempquad.var2 = this->addressTable[temp1];
				}
				else {
					if (temp1[0] == '\'') { 
						//literal
						tempquad.var2 = newTemp((char)temp1[1]);
					}
					else {
						tempquad.var2 = newTemp(stoi(temp1));
					}
				}
			}
			this->converted.push_back(tempquad);
		}
	}

	PrintQuadTable();
}

unsigned char* Gen::GenerateData() {
	// vector<unsigned char> DataArray;
	vector<pair<string, int>> sortedAddr = sort(this->addressTable);
	PrintAddressTable();
	PrintInitValTable();

	int maxAddr = 0; 

	for (auto i: this->addressTable) {
		if (i.second >= maxAddr) {
			maxAddr = i.second;
		}
	}

	unsigned char* DataArray = new unsigned char[maxAddr] {0};

	for (int i=0; i < sortedAddr.size(); i++) {
		if (this->initialvalTable[sortedAddr[i].first] != "") {
			if (this->datatypeTable[sortedAddr[i].first] == Token::INT) {
				int paramInt = stoi(this->initialvalTable[sortedAddr[i].first]);
				for (int j = sortedAddr[i].second + 4; j > sortedAddr[i].second; j--) {
					DataArray[j] = (paramInt >> ((j-3) * 8));
				}
			}
			else {
				int initvalindex = 0;
				int endLength = sortedAddr[i + 1].second;

				if (i - 1 == sortedAddr.size()) {
					int endLength = maxAddr;
				}
				else {
					int endLength = sortedAddr[i + 1].second;
				}

				for (int j = sortedAddr[i].second; j < endLength; j++, initvalindex++)
					DataArray[j] = initialvalTable[sortedAddr[i].first][initvalindex];
			}
		}
	}
	return DataArray;
	
	// for (auto i : addressTable) {
	// 	cout << i.first << ": ";
	// 	if (datatypeTable[i.first] == Token::INT) {
	// 		int a = int((unsigned char)(DataArray[i.second]) << 24 |
	// 			(unsigned char)(DataArray[i.second + 1]) << 16 |
	// 			(unsigned char)(DataArray[i.second + 2]) << 8 |
	// 			(unsigned char)(DataArray[i.second + 3]));

	// 		cout << a << endl;
	// 	}
	// 	else if (datatypeTable[i.first] == Token::CHAR) {
	// 		char a = DataArray[i.second];
	// 		cout << a << endl;
	// 	}
	// 	else if (datatypeTable[i.first] == Token::STR) {
	// 		for (int j = i.second; DataArray[j] != '\0'; j++) {
	// 			cout << DataArray[j];
	// 		}
	// 		cout << endl;
	// 	}
	// }
}