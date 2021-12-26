#include "vm.h"

VM::VM(vector<quad> converted, unsigned char* ds, map<int, Token> addrmap) {
	this->ds = ds;
	this->machineCode = converted;
	this->addrtypeMap = addrmap;
	this->pc = 0;
}

void VM::storeIntInDS(int index, int value) {
	for (int i = index + 3, k = 3; i >= index; i--, k--) {
		ds[i] = (value >> ((k - 3) * 8));
	}
}

bool VM::match(int addr1, int addr2) {
	// id id match	
	Token x = addrtypeMap[addr1];
	Token y = addrtypeMap[addr2];

	if (x == y) {
		return true;
	}
	else {
		switch(y) {
			case Token::INT:
			case Token::NUM:
				if (x == Token::INT) {return true;} 
				break;
			
			case Token::CHAR:
			case Token::LIT:
				if (x == Token::CHAR) {return true;}
				break;

			default:
				return false;
		}
		switch(x) {
			case Token::INT:
			case Token::NUM:
				if (y == Token::INT) {return true;} 
				break;
			
			case Token::CHAR:
			case Token::LIT:
				if (y == Token::CHAR) {return true;}
				break;

			default:
				return false;
		}
	}
}

void VM::typeCheck(int addr1, int addr2) {
	if (!match(addr1, addr2)) {
		throw std::runtime_error("Type Mismatch in VM");
	}
}

int VM::getIntFromDS(int index) {
	int a = int((unsigned char)(ds[index]) << 24 |
            (unsigned char)(ds[index + 1]) << 16 |
            (unsigned char)(ds[index + 2]) << 8 |
            (unsigned char)(ds[index + 3]));

	return a;
}

void VM::run() {
	cout<<"Initiating Code Execution: "<<endl;
	cout<<"-----------------------------------"<<endl;

	for (pc = 0; pc < machineCode.size(); pc++) {
		switch(machineCode[pc].opcode) {
			case Opcode::ADD:
				typeCheck(machineCode[pc].var1, machineCode[pc].var2);

				if (this->addrtypeMap[machineCode[pc].var1] == Token::INT || this->addrtypeMap[machineCode[pc].var1] == Token::NUM) {
					int var1 = getIntFromDS(machineCode[pc].var1);
					int var2 = getIntFromDS(machineCode[pc].var2);
					int res = var1 + var2;

					storeIntInDS(machineCode[pc].store, res);
				}
				else {
					char var1 = ds[machineCode[pc].var1];
					char var2 = ds[machineCode[pc].var1];
					char res = var1 + var2;

					ds[machineCode[pc].store] = res;
				}
				break;

			case Opcode::MUL:
				typeCheck(machineCode[pc].var1, machineCode[pc].var2);

				if (this->addrtypeMap[machineCode[pc].var1] == Token::INT || this->addrtypeMap[machineCode[pc].var1] == Token::NUM) {
					int var1 = getIntFromDS(machineCode[pc].var1);
					int var2 = getIntFromDS(machineCode[pc].var2);
					int res = var1 * var2;

					storeIntInDS(machineCode[pc].store, res);
				}
				else {
					char var1 = ds[machineCode[pc].var1];
					char var2 = ds[machineCode[pc].var1];
					char res = var1 * var2;

					ds[machineCode[pc].store] = res;
				}
				break;

			case Opcode::DIV:
				typeCheck(machineCode[pc].var1, machineCode[pc].var2);

				if (this->addrtypeMap[machineCode[pc].var1] == Token::INT || this->addrtypeMap[machineCode[pc].var1] == Token::NUM) {
					int var1 = getIntFromDS(machineCode[pc].var1);
					int var2 = getIntFromDS(machineCode[pc].var2);
					int res = var1 / var2;

					storeIntInDS(machineCode[pc].store, res);
				}
				else {
					char var1 = ds[machineCode[pc].var1];
					char var2 = ds[machineCode[pc].var1];
					char res = var1 / var2;

					ds[machineCode[pc].store] = res;
				}
				break;
			
			case Opcode::SUB:
				typeCheck(machineCode[pc].var1, machineCode[pc].var2);

				if (this->addrtypeMap[machineCode[pc].var1] == Token::INT || this->addrtypeMap[machineCode[pc].var1] == Token::NUM) {
					int var1 = getIntFromDS(machineCode[pc].var1);
					int var2 = getIntFromDS(machineCode[pc].var2);
					int res = var1 - var2;

					storeIntInDS(machineCode[pc].store, res);
				}
				else {
					char var1 = ds[machineCode[pc].var1];
					char var2 = ds[machineCode[pc].var1];
					char res = var1 - var2;

					ds[machineCode[pc].store] = res;
				}
				break;



			case Opcode::AS:
				typeCheck(machineCode[pc].var1, machineCode[pc].store);

				if (this->addrtypeMap[machineCode[pc].var1] == Token::INT || this->addrtypeMap[machineCode[pc].var1] == Token::NUM) {
					int var1 = getIntFromDS(machineCode[pc].var1);
					storeIntInDS(machineCode[pc].store, var1);
				}
				else {
					char var1 = ds[machineCode[pc].var1];
					ds[machineCode[pc].store] = var1;
				}
				break;
 


			case Opcode::LT:
				typeCheck(machineCode[pc].var1, machineCode[pc].var2);
				if (this->addrtypeMap[machineCode[pc].var1] == Token::INT || this->addrtypeMap[machineCode[pc].var1] == Token::NUM) {
					int var1 = getIntFromDS(machineCode[pc].var1);
					int var2 = getIntFromDS(machineCode[pc].var2);
					if (var1 < var2) {
						this->pc = machineCode[pc].store - 1;				
					}
				}
				else {
					char var1 = ds[machineCode[pc].var1];
					char var2 = ds[machineCode[pc].var1];
					if (var1 < var2) {
						this->pc = machineCode[pc].store - 1;				
					}
				}
				break;

			case Opcode::LTE:
				typeCheck(machineCode[pc].var1, machineCode[pc].var2);
				if (this->addrtypeMap[machineCode[pc].var1] == Token::INT || this->addrtypeMap[machineCode[pc].var1] == Token::NUM) {
					int var1 = getIntFromDS(machineCode[pc].var1);
					int var2 = getIntFromDS(machineCode[pc].var2);
					if (var1 <= var2) {
						this->pc = machineCode[pc].store - 1;				
					}
				}
				else {
					char var1 = ds[machineCode[pc].var1];
					char var2 = ds[machineCode[pc].var1];
					if (var1 <= var2) {
						this->pc = machineCode[pc].store - 1;				
					}
				}
				break;

			case Opcode::GT:
				typeCheck(machineCode[pc].var1, machineCode[pc].var2);
				if (this->addrtypeMap[machineCode[pc].var1] == Token::INT || this->addrtypeMap[machineCode[pc].var1] == Token::NUM) {
					int var1 = getIntFromDS(machineCode[pc].var1);
					int var2 = getIntFromDS(machineCode[pc].var2);
					if (var1 > var2) {
						this->pc = machineCode[pc].store - 1;				
					}
				}
				else {
					char var1 = ds[machineCode[pc].var1];
					char var2 = ds[machineCode[pc].var1];
					if (var1 > var2) {
						this->pc = machineCode[pc].store - 1;				
					}
				}
				break;

			case Opcode::GTE:
				typeCheck(machineCode[pc].var1, machineCode[pc].var2);
				if (this->addrtypeMap[machineCode[pc].var1] == Token::INT || this->addrtypeMap[machineCode[pc].var1] == Token::NUM) {
					int var1 = getIntFromDS(machineCode[pc].var1);
					int var2 = getIntFromDS(machineCode[pc].var2);
					if (var1 <= var2) {
						this->pc = machineCode[pc].store - 1;				
					}
				}
				else {
					char var1 = ds[machineCode[pc].var1];
					char var2 = ds[machineCode[pc].var1];
					if (var1 <= var2) {
						this->pc = machineCode[pc].store - 1;				
					}
				}
				break;

			case Opcode::EQ:
				typeCheck(machineCode[pc].var1, machineCode[pc].var2);
				if (this->addrtypeMap[machineCode[pc].var1] == Token::INT || this->addrtypeMap[machineCode[pc].var1] == Token::NUM) {
					int var1 = getIntFromDS(machineCode[pc].var1);
					int var2 = getIntFromDS(machineCode[pc].var2);
					if (var1 == var2) {
						this->pc = machineCode[pc].store - 1;				
					}
				}
				else {
					char var1 = ds[machineCode[pc].var1];
					char var2 = ds[machineCode[pc].var1];
					if (var1 == var2) {
						this->pc = machineCode[pc].store - 1;				
					}
				}
				break;

			case Opcode::NEQ:
				typeCheck(machineCode[pc].var1, machineCode[pc].var2);
				if (this->addrtypeMap[machineCode[pc].var1] == Token::INT || this->addrtypeMap[machineCode[pc].var1] == Token::NUM) {
					int var1 = getIntFromDS(machineCode[pc].var1);
					int var2 = getIntFromDS(machineCode[pc].var2);
					if (var1 != var2) {
						this->pc = machineCode[pc].store - 1;				
					}
				}
				else {
					char var1 = ds[machineCode[pc].var1];
					char var2 = ds[machineCode[pc].var1];
					if (var1 != var2) {
						this->pc = machineCode[pc].store - 1;				
					}
				}
				break;
			 

			
			case Opcode::GOTO:
				this->pc = machineCode[pc].var1 - 1;
				break;

			case Opcode::IN:
				if (this->addrtypeMap[machineCode[pc].var1] == Token::INT || this->addrtypeMap[machineCode[pc].var1] == Token::NUM) {
					int temp;
					cin >> temp;

					storeIntInDS(machineCode[pc].var1, temp);
				}
				else if (this->addrtypeMap[machineCode[pc].var1] == Token::CHAR || this->addrtypeMap[machineCode[pc].var1] == Token::LIT) {
					char temp;
					cin >> temp;

					ds[machineCode[pc].var1] =  temp;
				}
				break;

			case Opcode::OUT:
				if (this->addrtypeMap[machineCode[pc].var1] == Token::INT || this->addrtypeMap[machineCode[pc].var1] == Token::NUM) {
					int temp = getIntFromDS(machineCode[pc].var1);
					cout << temp;
				}
				else if ((this->addrtypeMap[machineCode[pc].var1] == Token::CHAR) || (this->addrtypeMap[machineCode[pc].var1] == Token::LIT)) {
					char temp = ds[machineCode[pc].var1];
					cout << temp;
				}
				break;

			case Opcode::OUTSTR:
				int i = machineCode[pc].var1;
				for (; ds[i] != '\0'; i++) {
					cout << ds[i];
				}
				break;
		}
	}
	cout<<endl<<"-----------------------------------"<<endl;
}