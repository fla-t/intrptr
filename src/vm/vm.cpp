#include "vm.h"

VM::VM(vector<quad> quad, unsigned char* ds) {
	this->ds = ds;
	this->machineCode = quad;
}