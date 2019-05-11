output: main_test.o opcode_table.o pass1.o pass2.o symbol_table.o
	g++ pass1.cpp  pass2.h instruction.h  opcode_table.cpp  pass1.h    symbol_table.cpp main_test.cpp  opcode_table.h pass2.cpp  symbol_table.h

main_test.o: main_test.cpp
	g++ -c main_test.cpp

pass1.o: pass1.h pass1.cpp
	g++ -c pass1.cpp

pass2.o: pass2.h pass2.cpp
	g++ -c pass2.cpp

symbol_table.o: symbol_table.h symbol_table.cpp
	g++ -c symbol_table.cpp

opcode_table.o: opcode_table.h 
	g++ -c opcode_table.h

clean:
	rm *.o *.h.gch *.out output assembly_listing_file.txt object_code.txt SymbolTable_File.txt intermediateFile.txt
