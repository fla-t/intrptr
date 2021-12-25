#include "../utils/opcode.h"
#include "../utils/quad.h"
#include "../utils/token.h"
#include "../gen/gen.h"

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
	map<int, Token> addrtypeMap;
public:
	VM(vector<quad>, unsigned char*, map<int, Token>);
	void run();

	void typeCheck(int, int);

	int getIntFromDS(int);
	void storeIntInDS(int, int);

};

#endif