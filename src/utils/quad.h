#include "opcode.h"

#ifndef QUAD_H
#define QUAD_H

struct quad {
	Opcode opcode;
	int var1 = -1;
	int var2 = -1;
	int store = -1;
};

#endif