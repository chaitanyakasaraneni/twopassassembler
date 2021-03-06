 //This file will define all the procedures necessary for pass2 to take in an intermediate file and produce an object code file, and a listing file
#include "pass2.h"
#include <string.h>
#include <iomanip>
using namespace std;

int loction_counter = 0;
bool x_flag = false;

instruction_type parse_intermediate_file(string Line)
{
	x_flag = false;
	instruction_type instruction;
	size_t pos = Line.find(";");
	string line_without_comments = Line.substr(0,pos);
	
	string columns[4];
	stringstream ss(line_without_comments);
	int num_columns = 0;
	while(ss >> columns[num_columns])
	{
		num_columns++;
		if(num_columns==4)
		{
			break;
		}
	}
	if (num_columns == 4) { 
		if(columns[3].compare("X") == 0)
		{
			instruction.instruction_operand = columns[2].substr(0, columns[2].size() - 1);
			instruction.instruction_opcode = columns[1];
			instruction.Check_Operand_Exist = true;
			x_flag = true;
		}
		else
		{
			instruction.instruction_operand = columns[3];
			instruction.instruction_opcode = columns[2];
			instruction.instruction_label = columns[1];
			instruction.Check_Operand_Exist = true;
		}
	} else if (num_columns == 3) {
		instruction.instruction_operand = columns[2];
		instruction.instruction_opcode = columns[1];
		instruction.Check_Operand_Exist = true;
	} else if (num_columns == 2) { 
		instruction.instruction_opcode = columns[1];
		instruction.Check_Operand_Exist = false;
	}
	loction_counter = strtol(columns[0].c_str(), NULL, 16);
	return instruction;
}

bool Check_Operand_Existance(instruction_type cmd)
{
	return cmd.Check_Operand_Exist;
}

bool search_optable(string opcode, uint8_t* value)
{
	if (optab.find(opcode)==optab.end()) {
		return false;
	} else
	{	// Opcode found
		*value = optab.find(opcode)->second;
		return true;
	}
}

bool search_symtable(string operand, uint16_t* value)
{
	if (symtab.find(operand)==symtab.end()) {
		return false;
	} else
	{
		// Operand found
		*value = symtab.find(operand)->second;
		return true;
	}
}

void header_record(Header_Record* rec) {
	rec->start_char = 'H';
	string prog_name = get_program_name();
	memcpy(rec->prog_name, prog_name.c_str(), prog_name.length());
	if (prog_name.length() < MAX_PROG_NAME_SIZE) {
		uint8_t i = prog_name.length();
		while (i < MAX_PROG_NAME_SIZE) {
			rec->prog_name[i] = ' ';
			i++;
		}
	}
	rec->prefix_start_addr = 0;
	rec->start_address = get_starting_address();
	rec->prefix_program_length = 0;
	rec->program_length = get_program_length();
}

void text_record(Text_Record* rec) {
	rec->start_char = 'T';
	rec->prefix_start_addr = 0;
	rec->start_addr = location_counter;
	rec->object_code_length = 0;
	memset(rec->object_code_bytes, 0, OBJ_CODE_LENGTH_TEXT_REC);
}

void end_record(End_Record* rec)
{
	rec->start_char = 'E';
	rec->prefix_start_exe_addr = 0;
	rec->start_exe_addr = get_starting_address();
}

void write_header_record(ofstream& obj_file, Header_Record* rec) {
	obj_file << rec->start_char << rec->prog_name << setfill('0') << setw(6) << hex << uppercase << rec->start_address << rec->program_length << endl;
}

void write_text_record(ofstream& obj_file, Text_Record* rec, uint8_t array_size) {
	obj_file << rec->start_char << setfill('0') << setw(2) << hex << uppercase << rec->start_addr << (uint)rec->object_code_length;
	for(int i = 0; i < array_size; i++)
	{
		obj_file << setfill('0') << setw(2) << hex << uppercase << (uint)rec->object_code_bytes[i];
	}
	obj_file << endl;
}

void write_end_record(ofstream& obj_file, End_Record* rec) {
	obj_file << rec->start_char << setfill('0') << setw(6) << hex << uppercase << rec->start_exe_addr << endl;
}

void write_to_listingFile(ofstream& list_file, string Line, ObjectCode* objcode)
{
	if (objcode != NULL) 
	{
		char objcodeToPrint[9];
		list_file << Line.substr(0, Line.size() - 1);
		sprintf(objcodeToPrint, "\t%02X%02X%02X\n\0", (uint)objcode->code_byte_1, (uint)objcode->code_byte_2, (uint)objcode->code_byte_3);
		list_file << objcodeToPrint;
	}
	else
	{
		list_file << Line;
	}
}

int Run_Pass2()
{
	// Input intermediate text file generated by pass1
	ifstream inputprogram("intermediateFile.txt");
	if (!inputprogram) {
		cout << "Cannot open input file, exiting" << endl;
		return 1;
	}
	// Output binary file containing the object program
	ofstream objprogram("object_code.txt"/*, ios::out | ios::binary*/);
	if (!objprogram) {
		cout << "Cannot open output object program file, exiting" << endl;
		return 1;
	}
	// Output text file containing the assembly listing
	ofstream listing_program("assembly_listing_file.txt");
	if (!listing_program) {
		cout << "Cannot open output  assembly listing file, exiting" << endl;
		return 1;
	}
	Text_Record txt_rec;
	uint8_t current_index_objcode_ary;
	string Line;
	getline(inputprogram, Line);
	while(!inputprogram.eof()) {
		if(!Check_Comment(Line)) {
			instruction_type instruction = parse_intermediate_file(Line);
			ObjectCode objcode;
			if (instruction.instruction_opcode == "START") {
				Header_Record hdr_rec;
				header_record(&hdr_rec);
				write_header_record(objprogram, &hdr_rec);
				text_record(&txt_rec);
				current_index_objcode_ary = 0;
				write_to_listingFile(listing_program, Line);
			} else if(instruction.instruction_opcode == "END") {
				write_text_record(objprogram, &txt_rec, current_index_objcode_ary);
				End_Record end_rec;
				end_record(&end_rec);
				write_end_record(objprogram, &end_rec);
				write_to_listingFile(listing_program, Line);
			} else {
				if (instruction.instruction_opcode == "BYTE" || instruction.instruction_opcode == "WORD") {
					if (instruction.instruction_operand.find("C'", 0) == 0) {
						objcode.code_byte_1 = (uint8_t)instruction.instruction_operand[2];
						objcode.code_byte_2 = (uint8_t)instruction.instruction_operand[3];
						objcode.code_byte_3 = (uint8_t)instruction.instruction_operand[4];
                                                objcode.code_byte_4 = (uint8_t)0;
					}
					else if (instruction.instruction_operand.find("X'", 0) == 0) {
						objcode.code_byte_1 = 0;
						objcode.code_byte_2 = 0;
						objcode.code_byte_3 = strtol(instruction.instruction_operand.substr(2, instruction.instruction_operand.size() - 3).c_str(), NULL, 16);
					}
					else if (isdigit(instruction.instruction_operand[0])) {
						uint16_t constant;
						constant = stoi(instruction.instruction_operand);
						objcode.code_byte_1 = 0;
						objcode.code_byte_2 = constant >> 8;
					        objcode.code_byte_3 = (uint8_t)(constant & 0xFF); 
                                                objcode.code_byte_4 = (uint8_t)0;
					} else {
						cout << "Error, opcode not found" << endl;
					}
				}
				else {
					if (search_optable(instruction.instruction_opcode, &objcode.code_byte_1)) { 
						if (instruction.Check_Operand_Exist) {
							uint16_t operand_address;
							if (!search_symtable(instruction.instruction_operand, &operand_address)) {
								cout << "Error, symbol not found in the symbol table" << endl;
								return 1;
							}
							else {
								objcode.code_byte_2 = operand_address >> 8;
								objcode.code_byte_3 = (uint8_t)(operand_address & 0xFF);
                                                                objcode.code_byte_4 = (uint8_t)0;
							}
						}
						else {
							objcode.code_byte_2 = 0;
							objcode.code_byte_3 = 0;
                                                        objcode.code_byte_4 = (uint8_t)0;
						}
					}
				}
				if(x_flag) //set x bit if x flag is set
				{
					objcode.code_byte_2 |= (1 << X_BIT);
				}
				// Write the current text record to object program file and create a new one if the number of object codes
				// added to the object_code_byte array has resulted in it being full
				if (current_index_objcode_ary >= OBJ_CODE_LENGTH_TEXT_REC) {
					write_text_record(objprogram, &txt_rec, current_index_objcode_ary);
					text_record(&txt_rec);
					current_index_objcode_ary = 0;
				}
				// Increment the index by the no of bytes added to the array in the current object code.
				// Write listing line to assembly listing file
				if (instruction.instruction_opcode == "RESW" || instruction.instruction_opcode == "RESB")
				{
					write_to_listingFile(listing_program, Line, NULL);
				}
				else
				{
					// add the assembled object code instruction to text record
					memcpy((txt_rec.object_code_bytes + current_index_objcode_ary), &objcode, sizeof(objcode));
					current_index_objcode_ary += sizeof(objcode);
					txt_rec.object_code_length = current_index_objcode_ary;
					write_to_listingFile(listing_program, Line, &objcode);
				}
			}// end of opcode check
		} // end of is comment check
		else
		{
			write_to_listingFile(listing_program, Line, NULL);
		}
		// Read the next input line
		getline(inputprogram, Line);
	} // end of input file
	// Close all the files in the end
	inputprogram.close();
	objprogram.close();
	listing_program.close();
	return 0;
}

