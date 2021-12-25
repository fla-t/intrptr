#include "vm.h"

VM::VM(vector<quad> quad, unsigned char* ds) {
	this->ds = ds;
	this->machineCode = quad;
	this->pc = 0;
}


void VM::run() {
	for (pc = 0; pc < machineCode.size(); pc++) {
		switch(machineCode[pc].opcode) {
			case Opcode::ADD:
				

			case Opcode::MUL:
			case Opcode::DIV:
			case Opcode::SUB:
			case Opcode::AS:
			
			 
			case Opcode::LT:
			case Opcode::LTE:
			case Opcode::GT:
			case Opcode::GTE:
			case Opcode::EQ:
			case Opcode::NEQ:
			
			 
			case Opcode::GOTO:

			case Opcode::IN:
			case Opcode::OUT:
			case Opcode::OUTSTR:
			
		}
	}
}