//This file will declare all the procedures necessary for pass1.
#ifndef PASS_1_H_
#define PASS_1_H_

#include "instruction.h"
#include "symbol_table.h"
#include "opcode_table.h"
#include <string>
#include <stdint.h>
#include <cstdio>
#include <fstream>
#include <iostream>

extern uint32_t location_counter;
extern uint32_t starting_address;
extern uint32_t program_length;
extern string program_name;

instruction_type parse_line(string aLine);

bool write_to_intermediateFile(ofstream& intermediate_file, string aLine);
bool Check_Comment(string aLine);
bool Check_Label_Existance(instruction_type);
uint32_t Create_Opcode(instruction_type);
uint32_t Constant_Length(instruction_type);
bool Check_End(string aLine);
void Run_Pass1(string input_file_name);
string get_program_name();
uint16_t get_starting_address();
uint16_t get_program_length();

#endif
