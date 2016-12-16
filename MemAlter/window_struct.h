#ifndef WINDOW_STRUCT_H
#define WINDOW_STRUCT_H
#include <Windows.h>

struct Window
{
	char class_name[96];
	char title[96];
};

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam);

#endif