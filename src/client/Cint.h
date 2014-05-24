#pragma once
#include <iostream>
#include <windows.h>
#include <Winbase.h>

class CInt {
public:
	CInt();
	~CInt();
	int Get();
	void Set(const int & iValue);
	CInt & operator=(const int &iValue);
	operator int();
protected:
	int * m_pValue;
	int m_iKeyValue;
	unsigned long old;

};