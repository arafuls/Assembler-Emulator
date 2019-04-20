#include "Emulator.h"
#include "stdafx.h"

using namespace std;

bool emulator::InsertMemory(int a_loc, int a_content) {
	if (m_memory[a_loc] != 0) {
		return false;
	}
	m_memory[a_loc] = a_content;
	return true;
}

bool emulator::RunProgram() {
	int currloc = 0;
	string opCode = "";
	string memLoc = "";
	string memoryCont = "";
	int opCodenum = 0;
	int memLocNum = 0;
	int a_num = 0;

	// loops with condition location is less than number of memory
	for (; currloc < MEMSZ;) {
		opCode = "";
		memLoc = "";
		a_num = 0;
		opCodenum = 0;
		memLocNum = 0;

		// check the number to see what kind of command it is
		if (m_memory[currloc] < 9999) {
			memoryCont = to_string(m_memory[currloc]);
		}
		else {
			
			// check if it is a five digit code or not build opcode and memory location
			if (m_memory[currloc] < 99999) {
				memoryCont = to_string(m_memory[currloc]);
				opCode = opCode + memoryCont[0];
				for (int i = 1; i < memoryCont.length(); i++) {
					memLoc = memLoc + memoryCont[i];
				}
			}

			// 6 digit op code build op code and memory location
			else {
				memoryCont = to_string(m_memory[currloc]);
				opCode = opCode + memoryCont[0];
				opCode = opCode + memoryCont[1];
				for (int i = 2; i < memoryCont.length(); i++) {
					memLoc = memLoc + memoryCont[i];
				}
			}

			// make the op code and location into integers
			opCodenum = atoi(opCode.c_str());
			memLocNum = atoi(memLoc.c_str());
			
			// switch statement based on the opCodenum defined by the VC3600 codes
			switch (opCodenum) {
			case 1:
				acceumulator += m_memory[memLocNum];
				break;
			case 2:
				acceumulator -= m_memory[memLocNum];
				break;
			case 3:
				acceumulator *= m_memory[memLocNum];
				break;
			case 4:
				if (m_memory[memLocNum] != 0) {
					acceumulator /= m_memory[memLocNum];
				}
				else {
					cout << "Division by 0" << endl;
					exit(1);
				}
				break;
			case 5:
				acceumulator = m_memory[memLocNum];
				break;
			case 6:
				m_memory[memLocNum] = acceumulator;
				break;
			case 7:
				Read(memLocNum);
				//read line
				break;
			case 8:
				cout << m_memory[memLocNum] << endl;
				break;
			case 9: 
				currloc = memLocNum;
				continue;
			case 10: 
				if (acceumulator < 0) {
					currloc = memLocNum;
					continue;
				}
				break;
			case 11:
				if (acceumulator == 0) {
					currloc = memLocNum;
					continue;
				}
				break;
			case 12:
				if (acceumulator > 0) {
					currloc = memLocNum;
					continue;
				}
				break;
			case 13:
				return false;
				break;
			default:
				cout << "Unknown Operation" << endl;
				//not supposed to get here
				return true;
			}
		}
		currloc++;
	}
	return true;
}

void emulator::Read(int a_location) {
	string a_input = "";
	bool word = true;

	// make sure that they are putting in a number not a string
	while (word == true) {
		word = true;
		cout << "? ";
		cin >> a_input;

		// make the input 6 digits maximum
		if (a_input.size() > 6) {
			if (a_input[0] == '-') {
				a_input = a_input.substr(0, 7);
			}
			else {
				a_input = a_input.substr(0, 6);
			}
		}

		//check if the input is a number or not
		if (a_input[0] != '-') {
			for (int i = 0; i < a_input.length(); i++) {
				if (a_input.at(i) >= '0' && a_input.at(i) <= '9') {
					word = false;
				}
				else {
					cout << "not valid" << endl;
					word = true;
					i = a_input.length();
				}
			}
		}
		else {
			if (a_input.size() == 1) {
				cout << "not valid" << endl;
				word = true;
			}
			for (int i = 1; i < a_input.length(); i++) {
				if (a_input.at(i) >= '0' && a_input.at(i) <= '9') {
					word = false;
				}
				else {
					cout << "not valid" << endl;
					word = true;
					i = a_input.length();
				}
			}
		}
	}

	// store the number as an integer and put it into the memory location 
	int a_val = stoi(a_input);
	m_memory[a_location] = a_val;
}