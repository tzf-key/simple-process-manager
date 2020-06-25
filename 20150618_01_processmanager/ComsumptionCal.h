#pragma once
#include <windows.h>
#include <stdio.h>

class CConsumptionCal
{
public:
	CConsumptionCal(void);
	~CConsumptionCal(void);
	DWORDLONG GetVitualMemoryTotal();
	DWORDLONG GetVitualMemoryUsed();
	CHAR GetCPUUsage();
	void GetSystemTimesAddress();
};

