#include <Windows.h>
#include "debug.h"

HWND _hwnd = NULL;

void DebugOut(const wchar_t* fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	wchar_t dbg_out[4096];
	vswprintf_s(dbg_out, fmt, argp);
	va_end(argp);
	OutputDebugString(dbg_out);

	std::wstring wideStr(dbg_out);
	std::string narrowStr(wideStr.begin(), wideStr.end());

	// Write the debug information to a log file
	std::ofstream logFile("DEBUG.LOG", std::ios_base::app);
	if (logFile.is_open())
	{
		logFile << narrowStr << std::endl;
		logFile.close();
	}
}

void DebugOutTitle(const wchar_t* fmt, ...)
{
	wchar_t s[1024];
	VA_PRINTS(s);
	SetWindowText(_hwnd, s);
}

void SetDebugWindow(HWND hwnd)
{
	_hwnd = hwnd;
}