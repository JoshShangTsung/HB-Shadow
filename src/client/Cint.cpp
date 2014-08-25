#include "Cint.h"

CInt::CInt() {
	m_iKeyValue = 0;
}

CInt::~CInt() {
}

int CInt::Get() {
	return m_iKeyValue;
}

void CInt::Set(const int & iValue) {
	m_iKeyValue = iValue;
}

CInt & CInt::operator=(const int &iValue) {
	m_iKeyValue = iValue;
	return *this;
}

CInt::operator int() {
	return m_iKeyValue;
}