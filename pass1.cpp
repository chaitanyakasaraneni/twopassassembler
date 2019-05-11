//This file will define all the procedures necessary for pass1 to take in an assembly file and produce an intermediate file
#include "pass1.h"
#include <string.h>
#include <sstream>
#include <algorithm>
using namespace std;

uint32_t location_counter = 0;
uint32_t starting_address = 0;
uint32_t program_length = 0;
string program_name = "";

instruction_type parse_line(string aLine)
{
	instruction_type instruction;
	stringstream ss;
	ss << aLine;
	if(!Check_Comment(aLine))
	{
		ss >> instruction.instruction_label >> instruction.instruction_opcode >> instruction.instruction_operand;
		if(instruction.instruction_operand.compare("\0") == 0 || instruction.instruction_operand.compare("X") == 0)
		{
			instruction.instruction_operand = instruction.instruction_opcode;
			instruction.instruction_opcode = instruction.instruction_label;
			instruction.instruction_label = '\0';
		}
	}
	return instruction;
}

bool write_to_intermediateFile(ofstream& intermediate_file, string aLine)
{
	if (Check_Comment(aLine))
	{
		intermediate_file << aLine << endl;
	}
	else if (Check_End(aLine))
	{
		intermediate_file << "\t\t" << aLine << endl;
	}
	else
	{
		intermediate_file << hex << uppercase << location_counter << '\t' << aLine << endl;
	}
	return true;
}

bool Check_Comment(string aLine)
{
	char firstChar;
	stringstream ss;
	ss << aLine;
	ss >> firstChar;
	if (firstChar == '.')
	{
		return true;
	}
	else return false;
}

bool Check_Label_Existance(instruction_type cmd)
{
	unordered_map<string, int>::iterator it;
	it = symtab.find(cmd.instruction_label);
	if (it == symtab.end())
	{
		return false;
	}
	else
	{
		return false;
	}
}

uint32_t Create_Opcode(instruction_type cmd)
{
	map<string, int>::iterator it;

	if(strncmp(cmd.instruction_label.c_str(), "", 1))
	{
		if(Check_Label_Existance(cmd))
		{
			cout << "ERROR! (" << cmd.instruction_label << ") multiple declarations. Aborting..." << endl;
			exit(1);
		}
		else
		{
			symtab.insert(pair<string, int>(cmd.instruction_label, location_counter));
		}
	}

	it = optab.find(cmd.instruction_opcode);
	if(it == optab.end())
	{
		if(strncmp(cmd.instruction_opcode.c_str(), "WORD", 5) == 0)
		{
			location_counter += 3;		
		}
		else if(strncmp(cmd.instruction_opcode.c_str(), "RESW", 5) == 0)
		{
			location_counter += (3 * strtol(cmd.instruction_operand.c_str(), NULL, 10)); 
		}
		else if(strncmp(cmd.instruction_opcode.c_str(), "RESB", 5) == 0)
		{
			location_counter += strtol(cmd.instruction_operand.c_str(), NULL, 10); 	
		}
		else if(strncmp(cmd.instruction_opcode.c_str(), "BYTE", 5) == 0)
		{
			location_counter += Constant_Length(cmd);
		}
		else
		{
			cout << "ERROR! (" << cmd.instruction_opcode << ") invalid opcode. Aborting..." << endl;
			exit(1);			
		}
	}
	else
	{
		location_counter += 3;
	}
}

uint32_t Constant_Length(instruction_type cmd)
{
	uint32_t ret = 0;
	if(cmd.instruction_operand.c_str()[0] == 'C')
	{
		ret =  cmd.instruction_operand.size() - 3;
	}
	else if(cmd.instruction_operand.c_str()[0] == 'X')
	{
		ret = 1;
	}
	else
	{
		ret = 3;
	}
	return ret;
}

bool Check_End(string aLine)
{
	bool ret = false;
	string first_word;
	stringstream ss(aLine);
	ss >> first_word;
	transform(first_word.begin(), first_word.end(), first_word.begin(), ::toupper);
	if(strncmp(first_word.c_str(), "END", 4) == 0)
	{
		ret = true;
	}

	return ret;
}

void Run_Pass1(string input_file_name)
{
	instruction_type instruction;
	string asmLine;

	//open input file
	ifstream asmFile;
	asmFile.open(input_file_name);
	if(!asmFile)
	{
		cout << "input file not open.\n";
		exit(1);
	}

	//open output file
	ofstream interFile;
	interFile.open("intermediateFile.txt");
	if(!interFile)
	{
		cout << "output file not open.\n";
		exit(1);
	}

	//read first line
	getline(asmFile, asmLine);
	instruction = parse_line(asmLine);

	if(strncmp(instruction.instruction_opcode.c_str(), "START", 5) == 0)
	{
		starting_address = strtol(instruction.instruction_operand.c_str(), NULL, 16);
		program_name = instruction.instruction_label;
	}
	else
	{
		cout << "first line does not contain \"START\"\n";
		starting_address = 0;
	}
	location_counter = starting_address;

        //cout << "1" << laction_counter << endl;                 ///////////

	write_to_intermediateFile(interFile, asmLine);
	cout << "starting address: " << hex << uppercase << starting_address << endl;
        
        int i = 2;
          
	//pass 1 algorithm
	while(getline(asmFile, asmLine))
	{
		//parse next line
		instruction = parse_line(asmLine);
		
		//write to intermediate file
		write_to_intermediateFile(interFile, asmLine);

		//check symbol and add to location counter
		if (!Check_Comment(asmLine) && !Check_End(asmLine))
		{
                        //cout << i << "\t" << location_counter << endl;
			Create_Opcode(instruction);
		}
		else if (Check_End(asmLine))
		{
			if (instruction.instruction_operand.compare("\0") == 0)
			{
				program_length = location_counter - starting_address;	
			}
			else if (isdigit(instruction.instruction_operand.c_str()[0]))
			{
				program_length = location_counter - strtol(instruction.instruction_operand.c_str(), NULL, 16);
			}
			else
			{
				unordered_map<string, int>::iterator it;
				it = symtab.find(instruction.instruction_operand);
				if (it == symtab.end())
				{
					cout << "ERROR! Symbol for END directive not defined. Aborting...\n";
					exit(1);
				}
				else
				{       
                        //cout << i << "\t" << location_counter << endl;
					program_length = location_counter - it->second;
				}
			}
			cout << "Program Length (Bytes): " << dec << program_length << endl;
		}
	}

	ofstream SymTabFile;
	SymTabFile.open("SymbolTable_File.txt");
	if(!SymTabFile)
	{
		cout << "output file not open.\n";
		exit(1);
	}

        for (auto i : symtab) {
          SymTabFile << i.first << "  " << i.second << endl;
        }




	//close files
	asmFile.close();
	interFile.close();
        SymTabFile.close();
}

string get_program_name()
{
	return program_name;
}

uint16_t get_starting_address()
{
	return starting_address;
}

uint16_t get_program_length()
{
	return program_length;
}
