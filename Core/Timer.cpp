
#include "Timer.h"
#include "windows.h"

CTimer::CTimer(void)
{
}

CTimer::~CTimer(void)
{
}

DWORD CTimer::Time()
{
	return GetTickCount();
}