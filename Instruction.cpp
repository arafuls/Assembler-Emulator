#include "stdafx.h"
#include "Instruction.h"

using namespace std;

Instruction::InstructionType Instruction::ParseInstruction(string &a_buff) {

	m_OpCode = "";
	m_Operand = "";
	m_Label = "";

	// if string is empty its a comment
	if(a_buff.empty())
		return ST_Comment;


	istringstream iss;
	vector <string> input;
	iss.str(a_buff);
	string word;
	int count = 0;
	bool assembler = false;

	// parse the words into a vector called input
	while (iss >> word) {
		input.push_back(word);
	}
	iss.clear();

	// check if the whole statement is a comment
	if (input.at(0).at(0) == ';') {
		while (!input.empty())
			input.pop_back();
		return ST_Comment;
	}

	// if theres more than one statement this if statement parses out any comments
	if (input.size() >= 2) {
		for (int i = 0; i < input.size();i++) {
			for (int j = 0; j < input.at(i).length();j++) {
				if (input.at(i).at(j) == ';') {
					for (int k = input.size(); k > i+1; k--) {
						input.pop_back();
					}
					input.at(i) = input.at(i).substr(0, j);
				}
			}
		}
	}

	// makes the statement 3 words
	if (input.size() >= 4) {
		input.pop_back();
	}

	if (input.size() > 3) {
		Errors::RecordError(Errors::CreateError(string("too many arguments on the line.")));
	}

	// if it has 3 inputs there is a different label
	if (input.size() == 3) {

		// put the input into the strings for label, opcodes, and operands
		m_Label = input.at(0);
		m_OpCode = input.at(1);
		m_Operand = input.at(2);

		// convert the opcode to uppercase
		for (int i = 0; i < input.at(1).length();i++) {
			input.at(1)[i] = toupper(input.at(1)[i]);
		}

		// check if it is a assembler instruction or machine language instruction and empty the vector
		if (input.at(1).compare("DC") == 0 || input.at(1).compare("DS") == 0) {
			while (!input.empty())
				input.pop_back();
			return ST_AssemblerInstr;
		}
		else {
			while (!input.empty())
				input.pop_back();
			return ST_MachineLanguage;
		}
	}

	// if the input size its 2 theres just an op code and operand
	else if (input.size() == 2) {
		m_OpCode = input.at(0);
		m_Operand = input.at(1);

		// capitalize the opcode
		for (int i = 0;i < input.at(0).length();i++) {
			input.at(0)[i] = toupper(input.at(0)[i]);
		}

		// check if it is machine language or assembler and empty the input
		if (input.at(0).compare("ORG") == 0) {
			while (!input.empty())
				input.pop_back();
			return ST_AssemblerInstr;
		}
		else {
			while (!input.empty())
				input.pop_back();
			return ST_MachineLanguage;
		}
	}

	// it is length of one, just an op code
	else {

		// uppser case it
		m_OpCode = input.at(0);
		for (int i = 0;i < input.at(0).length();i++) {
			input.at(0)[i] = toupper(input.at(0)[i]);
		}

		// check if it is a end or machine language instruction and empty input
		if (input.at(0).compare("END") == 0) {
			while (!input.empty())
				input.pop_back();
			return ST_End;
		}
		else {
			while (!input.empty())
				input.pop_back();
			return ST_MachineLanguage;
		}
	}

	//should not get here
	return ST_End;
}

int Instruction::LocationNextInstruction(int a_loc) {
	string s;
	s = m_OpCode;
	for (int i = 0; i < s.length(); i++) {
		s[i] = toupper(s[i]);
	}
	if (m_type == ST_MachineLanguage && !m_Operand.empty()) {
		if (s.compare("ORG") == 0) {
			return atoi(m_Operand.c_str());
		}
		else if (s.compare("DS") == 0) {
			return a_loc + atoi(m_Operand.c_str());
		}
	}
	return a_loc + 1;
}

int Instruction::Translate() {
	map<string, int>::iterator a_it;
	string s;
	int loc;
	s = m_OpCode;
	for (int i = 0; i < s.length(); i++) {
		s[i] = toupper(s[i]);
	}
	a_it = commands.find(s);
	if (a_it->first.compare("DC")==0 || a_it->second != 0) {
		return a_it->second;
	}
	return 9999999;
}