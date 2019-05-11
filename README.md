# Two-Pass Assembler
Assembler is a language processor that converts an assembly code/instruction into to machine executable/object code.<br>

We will be using two data storage structures the OPTAB (Operational Code Table) and the SYMTAB (Symbol Table). <br>
The input to our code will be in Assembly language which will be analyzed using OPTAB and SYMTAB and converted to an intermediate code by the end of pass 1 and that intermediate code serves as an input to our pass 2 the resulting output from Pass 2 will be the machine code in binary format.

### Design
![design](https://github.com/chaitanyakasaraneni/twopassassembler/blob/master/images/design.PNG "Design of Two pass assembler")
## Psuedocodes:
### Pass-1 
```
Input : Source Program.
Read entire source program.
Separate Symbols, Literals, Mnemonics
Performs Location Counter Processing
Collected labels and their assigned addresses are stored in the SYMBOL TABLE.
Output : Intermediate File. 

```

### Pass-2
```
Input : Intermediate File.
Instruction are again read and assembled using SYMBOL TABLE.
Computes actual address of every label.
Translates operands name to actual register.
Immediate values is translated to binary strings(1’s and 0’s)
Output : Object Code.

```
