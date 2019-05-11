//This file will handle calling the high-level functions
#include "pass2.h"

int main(int argc, char const *argv[])
{
   	if(argc == 1)
	{
		cout << "Please specify an SIC assembly file to assemble." << endl;
		exit(1);
	}
	else if(argc > 2)
	{
		cout << "Please only specify one SIC assembly file to assemble." << endl;
		exit(1);
	}
	else
	{
		optable();

		Run_Pass1(argv[1]);

		int err = Run_Pass2();

		if(!err)
		{
			cout << "\nAssembly complete.\n\n";			
		}
		else
		{
			return err;
		}

	}

	return 0;
}
