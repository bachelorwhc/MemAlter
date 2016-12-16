#include <iostream>
#include <string>
#include <Windows.h>
#include "privilege.h"

bool EnableDebugPrivilege()
{
	HANDLE token(NULL);
	if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &token))
	{
		std::cout << "OpenThreadToken failed." << std::endl;
		if (GetLastError() == ERROR_NO_TOKEN)
		{
			if (!ImpersonateSelf(SecurityImpersonation))
				std::cout << "ImpersonateSelf failed." << std::endl;

			if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &token))
			{
				std::cout << "OpenThreadToken failed again." << std::endl;
				return false;
			}
			else
			{
				std::cout << "OpenThreadToken repaired." << std::endl;
			}
		}
		else
		{
			std::cout << "Unknown error." << std::endl;
			return false;
		}
	}
	LUID luid;
	TOKEN_PRIVILEGES tp = { 0 };
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
	{
		std::cout << "LookupPrivilegeValue failed." << std::endl;
		return false;
	}
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(token, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
	{
		std::cout << "AdjustTokenPrivileges failed." << std::endl;
		return false;
	}
	return true;
}

int main()
{
	if (!EnableDebugPrivilege())
	{
		std::cout << "EnableDebugPrivilege failed." << std::endl;
		return 0;
	}

	HWND hwnd(nullptr);
	std::string windowname;
	GETWINDOW:
	while (!hwnd)
	{
		std::cout << "Please enter the name of windows you want to edit its memory." << std::endl;
		std::cout << "NAME OF WINDOW: ";
		std::cin >> windowname;
		hwnd = FindWindow(0, windowname.c_str());
	}
	DWORD process_id;
	GetWindowThreadProcessId(hwnd, &process_id);
	HANDLE handle_of_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
	if (!handle_of_process)
	{
		std::cout << "OpenProcess failed." << std::endl;
		return 0;
	}
	std::cout << "ID is " << process_id << std::endl;
	std::cout << "Handle is " << handle_of_process << std::endl;
	std::string command;
	std::cout << "Enter command, exit this program by 'exit'" << std::endl;
	while (std::cin >> command, command != "exit")
	{
		if(command == "edit")
		{
			unsigned long address;
			int data;
			std::cin >> std::hex >> address;
			std::cin >> std::dec >> data;
			if(WriteProcessMemory(handle_of_process, (LPVOID)address, &data, sizeof(data), NULL))
			{
				std::cout << "done." << std::endl;
			}
			else
			{
				std::cout << "failed." << std::endl;
				std::cout << "ERROR CODE : " << GetLastError() << std::endl;
			}
		}
		else if (command == "reselect")
		{
			CloseHandle(handle_of_process);
			hwnd = NULL;
			goto GETWINDOW;
		}
	}
	CloseHandle(handle_of_process);
	return 0;
}