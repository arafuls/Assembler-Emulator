/*  README 	
 *	Contains the implementation of the steps it takes to assemble the VC3600 programming language
 *	including opening the file to read lines of the programing language. These will be parsed and 
 *	utilized through Pass I, Pass II, and the Emulator. 									   */

#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"
#include "Emulator.h"

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
// See main program.

Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv )
{
}

void Assembler::PassI( ) 
{
	Errors::InitErrorReporting();

    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for( ; ; ) {

        // Read the next line from the source file.
        string buff; 
        if( ! m_facc.GetNextLine( buff ) ) {
			return;
        }


        // Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( buff );

        // If this is an end statement, there is nothing left to do in pass I.
        if( st == Instruction::ST_End ) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip other instruction types.
        if( st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr ) 
        {
        	continue;
		}

        // If the instruction has a label, record it and its location in the
        // symbol table.
        if( m_inst.isLabel( ) ) {
            m_symtab.AddSymbol( m_inst.GetLabel( ), loc );
        }
        loc = m_inst.LocationNextInstruction( loc );
    }
}

void Assembler::PassII() {
	
	// bring the file back to the beginning so we parse from the start
	m_facc.Rewind();

	int loc = 0;
	string buffer;
	cout << "Translation of Program:" << endl << endl;
	cout << "Location\tContents\tOriginal Statement" << endl;
	
	for (; ; ) {
		buffer = "";
	
		// read the next line in the file
		if (!m_facc.GetNextLine(buffer)) {
			Errors::RecordError(Errors::CreateError(loc, string("Missing END statement")));
			Errors::DisplayErrors();
			system("pause");
			exit(1);
			return;
		}
	
		// Parse the line and get the instruction time
		Instruction::InstructionType it = m_inst.ParseInstruction(buffer);

		// if the line is a comment or the end it just prints the original line
		// check if there are lines after the end
		if (it == Instruction::ST_End) {
			if (m_facc.GetNextLine(buffer)) {
				Errors::RecordError(Errors::CreateError(loc, string("Instruction after END statement")));
				Errors::DisplayErrors();
				system("pause");
				exit(1);
			}
			else {
				cout << "\t\t\t\t" << buffer << endl;
				return;
			}
		}
		if (it == Instruction::ST_Comment || it == Instruction::ST_End) {
			cout << "\t\t\t\t" << buffer << endl;
		}

		// otherwise it translates the line into a decimal command and outputs it
		else {
			cout << loc << "\t\t";
			if (m_inst.Translate() != 9999999) {
				
				// if the command was found print its op code with 2 digits
				cout << setfill('0') << setw(2) << m_inst.Translate();
			}

			if (it != Instruction::ST_Comment || it != Instruction::ST_End) {
				if (GetLoc().compare("NULL") != 0) {

					// if the location exists then print the operand location to complete the op code
					cout << setfill('0') << setw(4) << GetLoc();
				}
			}

			cout << "\t\t" << buffer << endl;
			if (loc > emulator::MEMSZ) {
				Errors::RecordError(Errors::CreateError(loc, "location out of memory"));
				Errors::DisplayErrors();
				system("pause");
				exit(1);
			}
			loc = m_inst.LocationNextInstruction(loc);
		}
		if (!Errors::IsEmpty()) {
			Errors::DisplayErrors();
		}
	}
}

string Assembler::GetLoc() {
	string s;
	int operandLoc;
	s = m_inst.GetOpcode();
	for (int i = 0; i < s.length(); i++) {
		s[i] = toupper(s[i]);
	}
	if (s.compare("ORG") == 0 || s.compare("DS")==0) {
		return "NULL";
	}
	s = m_inst.GetOperand();
	if (s.compare("") == 0) {
		return "0000";
	}
	else if (s[0] >= '0' && s[0] <= '9') {
		return s;
	}
	else if (m_symtab.LookupSymbol(s, operandLoc)) {
		s = to_string(operandLoc);
		return s;
	}
	string error = "Does not exist";
	Errors::RecordError(error);
}

void Assembler::RunEmulator() {
	cout << "\nResults from emulating program: \n\n";
	m_facc.Rewind();
	int loc = 0;
	int command = 0;
	string buffer;
	string l4 = "";
	for (;;) {
		buffer = "";
		if (!m_facc.GetNextLine(buffer)) {
			return;
		}
		Instruction::InstructionType it = m_inst.ParseInstruction(buffer);
		if (it != Instruction::ST_Comment && it != Instruction::ST_End) {
			if (m_inst.Translate() != 9999999) {
				command = m_inst.Translate() * 10000;
				string l4 = GetLoc();
				command += stoi(l4);
				if (!m_emul.InsertMemory(loc, command)) {
					return;
				}
			}
		}
		if (it != Instruction::ST_Comment) {
			loc = m_inst.LocationNextInstruction(loc);
		}
	}
}

void Assembler::PrintEm() {
	bool done = false;
	done = m_emul.RunProgram();
	if (done == false) {
		cout << endl << "End of emulation" << endl;
		system("pause");
		exit(1);
	}
}