//
//		Implementation of the symbol table class.
//
#include "stdafx.h"
#include "SymTab.h"
#include "Errors.h"

void SymbolTable::AddSymbol(string &a_symbol, int a_loc)
{

    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st;
    st = m_symbolTable.find( a_symbol );
    if(st != m_symbolTable.end()) {
        st->second = multiplyDefinedSymbol;
		Errors::RecordError(Errors::CreateError(a_loc, string("variable defined more than once")));
        return;
    }

    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}

void SymbolTable::DisplaySymbolTable() {
	cout << "Symbol#\tSymbol\tLocation" << endl;
	count = 0;
	for (map<string, int>::iterator st = m_symbolTable.begin(); st != m_symbolTable.end(); st++) {
		cout << count << "\t" << st->first << "\t" << st->second << endl;
		count++;
	}
}

bool SymbolTable::LookupSymbol(string &a_s, int &a_loc) {

	// iterate through map and check for symbol
	for (map<string, int>::iterator st = m_symbolTable.begin(); st != m_symbolTable.end(); st++) {
		if (st->first.compare(a_s) == 0) {
			a_loc = st->second;
			return true;
		}
	}
	return false;
}