#pragma once

#include <string>
#include <list>

#define Constant const
#define DataSize sizeof
#define RECTANGLE RECT
#define Integer int
#define Automatic auto

class Manager
{
public:
	Manager();
	~Manager();

	std::list<std::wstring> Select(Constant std::wstring Database);
	std::wstring Select(Constant std::wstring Table, Constant std::wstring Value);
};

