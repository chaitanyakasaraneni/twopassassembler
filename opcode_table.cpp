//This file will define the initialization of the opcode table

#include <map>
#include <string>
#include "opcode_table.h"

map<string, int> optab;

// map<string, int> optable;

void optable()
{
	optab["ADD"] = 0x18;
	optab["ADDF"] = 0x58;
	optab["ADDR"] = 0x90;
	optab["AND"] = 0x40;
	optab["CLEAR"] = 0xB4;
	optab["COMP"] = 0x28;
	optab["COMPF"] = 0x88;
	optab["COMPR"] = 0xA0;
	optab["DIV"] = 0x24;
	optab["DIVR"] = 0x9C;
	optab["FIX"] = 0xC4;
	optab["FLOAT"] = 0xC0;
	optab["HIO"] = 0xF4;
	optab["J"] = 0x3C;
	optab["JEQ"] = 0x30;
	optab["JGT"] = 0x34;
	optab["JLT"] = 0x38;
	optab["JSUB"] = 0x48;
	optab["LDA"] = 0x00;
	optab["LDB"] = 0x68;
	optab["LDCH"] = 0x50;
	optab["LDF"] = 0x70;
	optab["LDL"] = 0x08;
	optab["LDS"] = 0x6C;
	optab["LDT"] = 0x74;
	optab["LDX"] = 0x04;
	optab["LPS"] = 0xD0;
	optab["MUL"] = 0x20;
	optab["MULF"] = 0x60;
	optab["MULR"] = 0x98;
	optab["NORM"] = 0xC8;
	optab["OR"] = 0x44;
	optab["RD"] = 0xD8;
	optab["RMO"] = 0xAC;
	optab["RSUB"] = 0x4C;
	optab["SHIFTL"] = 0xA4;
	optab["SHIFTR"] = 0xA8;
	optab["SIO"] = 0xF0;
	optab["SSK"] = 0xEC;
	optab["STA"] = 0x0C;
	optab["STB"] = 0x78;
	optab["STCH"] = 0x54;
	optab["STF"] = 0x80;
	optab["STI"] = 0xD4;
	optab["STL"] = 0x14;
	optab["STS"] = 0x7C;
	optab["STSW"] = 0xE8;
	optab["STT"] = 0x84;
	optab["STX"] = 0x10;
	optab["SUB"] = 0x1C;
	optab["SUBF"] = 0x5C;
	optab["SUBR"] = 0x94;
	optab["SVC"] = 0xB0;
	optab["TD"] = 0xE0;
	optab["TIO"] = 0xF8;
	optab["TIX"] = 0x2C;
	optab["TIXR"] = 0xB8;
	optab["WD"] = 0xDC;
	return;
}
