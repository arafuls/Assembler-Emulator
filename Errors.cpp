#include "Errors.h"
#include "stdafx.h"

using namespace std;

vector<string> Errors::m_ErrorMsgs;

void Errors::InitErrorReporting() {
	m_ErrorMsgs.clear();
	return;
}

void Errors::RecordError(string &a_emsg) {
	if (a_emsg.size() == 0) {
		return;
	}
	m_ErrorMsgs.push_back(a_emsg);
}

void Errors::DisplayErrors() {
	cout << "There were " << m_ErrorMsgs.size() << " errors" << endl;
	while (!m_ErrorMsgs.empty()) {
		cout << m_ErrorMsgs.back() << endl;
		m_ErrorMsgs.pop_back();
	}
}

string Errors::CreateError(int a_loc, const string &a_errMsg) {
	string location = to_string(a_loc);

	return ("Error at location " + location + " :: " + a_errMsg + "\n");
}

string Errors::CreateError(const string &a_errMsg) {
	return ("Error :: " + a_errMsg + "\n");
}