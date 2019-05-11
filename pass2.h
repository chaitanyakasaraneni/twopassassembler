 //This file will declare all the procedures necessary for pass2
#ifndef PASS_2_H_
#define PASS_2_H_

#include "instruction.h"
#include "symbol_table.h"
#include "opcode_table.h"
#include "pass1.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#define OBJ_CODE_LENGTH_TEXT_REC 30
#define MAX_PROG_NAME_SIZE 6
#define SIZE_HEADER_RECORD_BYTES 13
#define SIZE_TEXT_RECORD_BYTES 35
#define SIZE_END_RECORD_BYTES 4
#define X_BIT 7

// Structure to store header record values:
class Header_Record {
  public:
    char start_char; // 1 byte
    char prog_name[MAX_PROG_NAME_SIZE]; // 6 bytes
    uint8_t prefix_start_addr; // 1 byte
    uint16_t start_address; // 2 byte
    uint8_t prefix_program_length; // 1 byte
    uint16_t program_length; // 2 byte
};

/* Structure to store text record values/
 * Length of object codes in one text record is fixed at 0x1E(30)
*/
class Text_Record {
  public:
    char start_char;
    uint8_t prefix_start_addr; // 1 byte
    uint16_t start_addr;	 //2 byte
    uint8_t object_code_length; // 1 byte
    uint8_t object_code_bytes[OBJ_CODE_LENGTH_TEXT_REC];
};

// Structure to store end record values which has address of first executable instruction(Start)
class End_Record {
  public:
    char start_char;
      uint8_t prefix_start_exe_addr;
      uint16_t start_exe_addr;
};

// Structure to represent the 3 byte object code to store opcode value and operand address
class ObjectCode {
  public:
    uint8_t code_byte_1;
    uint8_t code_byte_2;
    uint8_t code_byte_3;
    uint8_t code_byte_4;
};
// Read the input line of intermediate file and populate the command_t struct
instruction_type parse_intermediate_file(string Line);

// write the source intermediate file line with object code to the assembly listing file
void write_to_listingFile(ofstream& list_file, string Line, ObjectCode* objcode=NULL);

// Determine if a valid symbol exists in the operand field of the command_t struct
bool Check_Operand_Existance(instruction_type cmd);

// Search for the opcode in the optable and gte its value in the passed integer pointer
bool search_optable(string opcode, uint8_t* value);

// Search for the operand in the symbol table and get its value in the passed integer pointer
bool search_symtable(string operand, uint16_t* value);

//Initialize header record
void header_record(Header_Record* rec);

//Initialize text record
void text_record(Text_Record* rec);

//Initialize end record
void end_record(End_Record* rec);

//Write the header record to object program which is the final output
void write_header_record(ofstream& obj_file, Header_Record* rec);

//Write the final text record to the final output which is object program
void write_text_record(ofstream& obj_file, Text_Record* rec, uint8_t array_size);

//Writing end record to object program
void write_end_record(ofstream& obj_file, End_Record* rec);

int Run_Pass2();

#endif
