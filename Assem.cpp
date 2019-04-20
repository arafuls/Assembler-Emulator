// Austin Rafuls

#include "stdafx.h"     
#include <stdio.h>

#include "Assembler.h"

int main( int argc, char *argv[] )
{
    Assembler assem( argc, argv );

	/* Create label locations */
    assem.PassI( );

    /* Display symbol table */
    assem.DisplaySymbolTable();
	//system("pause");

    /* Symbol table and translation */
    assem.PassII( );
	//system("pause");

	/* Run emulator on VC3600 from PassII */
    assem.RunEmulator();
	assem.PrintEm();
	//system("pause");
    
	return 0;
}
