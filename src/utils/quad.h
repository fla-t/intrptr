#include "opcode.h"

#ifndef QUAD_H
#define QUAD_H

struct quad {
	Opcode opcode;
	int var1;
	int var2;
	int store;
};

#endif