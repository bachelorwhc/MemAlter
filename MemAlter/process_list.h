#ifndef PROCESS_LIST_H
#define PROCESS_LIST_H
#include <vector>
#include <Windows.h>

class ProcessList
{
	ProcessList() :
		process_detected(0),
		processes()
	{
		
	}

	void Rescan()
	{
		EnumProcesses(
	}

private:
	unsigned int process_detected;
	std::vector<DWORD> processes;
};

#endif