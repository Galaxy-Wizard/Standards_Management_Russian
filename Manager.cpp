#include <windows.h>

#include "Manager.h"



Manager::Manager()
{
}


Manager::~Manager()
{
}


std::list<std::wstring> Manager::Select(Constant std::wstring Database)
{
	Constant DWORD CurrentDatabaseLength = 10000;
	wchar_t CurrentDatabase[CurrentDatabaseLength];
	if (GetCurrentDirectoryW(CurrentDatabaseLength, CurrentDatabase) != 0)
	{
		if (SetCurrentDirectoryW(Database.c_str()) == TRUE)
		{
			std::list<std::wstring> Tables;

			WIN32_FIND_DATAW FindData;

			ZeroMemory(&FindData, DataSize(WIN32_FIND_DATAW));

			HANDLE FindFileHandle = FindFirstFileW(L"*.*", &FindData);

			if (FindFileHandle != NULL)
			{
				Tables.push_back(FindData.cFileName);

				for (; FindNextFileW(FindFileHandle, &FindData) != FALSE;)
				{
					Tables.push_back(FindData.cFileName);
				}

				FindClose(FindFileHandle);
			}

			SetCurrentDirectoryW(CurrentDatabase);

			return Tables;
		}

		SetCurrentDirectoryW(CurrentDatabase);
	}

	return std::list<std::wstring>();
}


std::wstring Manager::Select(Constant std::wstring Table, Constant std::wstring Value)
{
	return std::wstring();
}

