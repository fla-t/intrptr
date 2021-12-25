#include "../utils/opcode.h"
#include "../utils/quad.h"

#include <vector>
#include <iostream>
#include <map>

using namespace std;

#ifndef VM_H
#define VM_H

class VM {
	vector<quad> machineCode;
	unsigned char* ds;
public:
	VM(vector<quad>, unsigned char*);
};

#endif