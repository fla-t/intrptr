#include "../utils/opcode.h"
#include "../utils/quad.h"
#include "../utils/token.h"

#include <vector>
#include <iostream>
#include <map>

using namespace std;

#ifndef VM_H
#define VM_H

class VM {
	vector<quad> machineCode;
	unsigned char* ds;
	int pc;
public:
	VM(vector<quad>, unsigned char*);
	void run();
};

#endif