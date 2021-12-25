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
	vector<unsigned char> data;
public:
	VM(vector<quad>, vector<unsigned char>);
};

#endif