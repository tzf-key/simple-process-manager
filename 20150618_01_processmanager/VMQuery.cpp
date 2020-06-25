#include "stdafx.h"
#include "VMQuery.h"
#include <strsafe.h>

CVMQuery::CVMQuery()
{
}


CVMQuery::~CVMQuery()
{
}


void CVMQuery::GetProtectText(DWORD dwProtect, PTSTR szBuf, int nSize, BOOL bShowFlags)
{
	PCTSTR p = L"[UnKnown]";
	switch (dwProtect & ~(PAGE_GUARD | PAGE_NOCACHE | PAGE_WRITECOMBINE))
	{
	case PAGE_READONLY:
	{
						  p = L"[ -R--  ]";
						  break;
	}
	case PAGE_READWRITE:
	{
						   p = L"[ -RW-  ]";
						   break;
	}
	case PAGE_WRITECOPY:
	{
						   p = L"[ -RWC  ]";
						   break;
	}
	case PAGE_EXECUTE:
	{
						 p = L"[ E---  ]";
						 break;
	}
	case PAGE_EXECUTE_READ:
	{
							  p = L"[ ER--  ]";
							  break;
	}
	case PAGE_EXECUTE_READWRITE:
	{
								   p = L"[ ERW-  ]";
								   break;
	}
	case PAGE_EXECUTE_WRITECOPY:
	{
								   p = L"[ ERWC  ]";
								   break;
	}
	case PAGE_NOACCESS:
	{
						  p = L"[ ----  ]";
						  break;
	}
	}
	StringCchCopy(szBuf, nSize, p);

	if (bShowFlags)
	{
		StringCchCat(szBuf, nSize, L" ");
		StringCchCat(szBuf, nSize, (dwProtect&PAGE_GUARD) ? L"G" : L"-");
		StringCchCat(szBuf, nSize, (dwProtect&PAGE_NOCACHE) ? L"N" : L"-");
		StringCchCat(szBuf, nSize, (dwProtect&PAGE_WRITECOMBINE) ? L"W" : L"-");
	}
}

void CVMQuery::GetMemStorageText(DWORD dwStorage, PTSTR szBuf, int nSize)
{
	PCTSTR p = L"Unknown";
	switch (dwStorage)
	{
	case MEM_FREE:
	{
					 p = L"Free   ";
					 break;
	}
	case MEM_RESERVE:
	{
						p = L"Reserve";
						break;
	}
	case MEM_IMAGE:
	{
					  p = L"Image  ";
					  break;
	}
	case MEM_MAPPED:
	{
					   p = L"Mapped ";
					   break;
	}
	case MEM_PRIVATE:
	{
						p = L"Private";
						break;
	}
	}

	StringCchCopy(szBuf, nSize, p);
}

BOOL CVMQuery::GetVMBlockInfo(HANDLE hProcess, LPCVOID lpAddress, VMQUERYEX *pVMBlock)
{
	ZeroMemory(pVMBlock, sizeof(*pVMBlock));
	//获取地址空间中内存地址信息，保存到Memory basic infomatino结构体中

	MEMORY_BASIC_INFORMATION mbiRgn;
	if (!(VirtualQueryEx(hProcess, lpAddress, &mbiRgn, sizeof(mbiRgn)) == sizeof(mbiRgn)))
	{
		return FALSE;
	}

	PVOID pvRgnBaseAddress = mbiRgn.AllocationBase;
	//预划分内存区域基址
	PVOID pvAddressBlk = mbiRgn.AllocationBase;
	//第一个内存块的基址

	MEMORY_BASIC_INFORMATION mbiBlk;
	while (true)
	{
		if (!(VirtualQueryEx(hProcess, pvAddressBlk, &mbiBlk, sizeof(mbiBlk)) == sizeof(mbiBlk)))
		{
			break;

		}
		if (mbiBlk.AllocationBase != pvRgnBaseAddress)
		{
			break;
		}
		pVMBlock->dwRgnBlocks++;
		pVMBlock->nRgnSize += mbiBlk.RegionSize;
		if ((mbiBlk.Protect&PAGE_GUARD) == PAGE_GUARD)
		{
			pVMBlock->dwRgnGuardBlks++;
		}
		if (MEM_PRIVATE == mbiRgn.Type)
		{
			pVMBlock->dwRgnStorage = mbiBlk.Type;
		}
		pvAddressBlk = (PVOID)((PBYTE)pvAddressBlk + mbiBlk.RegionSize);

	}

	pVMBlock->bRgnIsStack = (pVMBlock->dwRgnGuardBlks > 0);
	return (TRUE);

}



BOOL CVMQuery::GetVMinfo(HANDLE hProcess, LPCVOID lpAddress, PVMQUERY pVMQ)
{
	ZeroMemory(pVMQ, sizeof(*pVMQ));

	MEMORY_BASIC_INFORMATION mbi;
	if (!(VirtualQueryEx(hProcess, lpAddress, &mbi, sizeof(mbi)) == sizeof(mbi)))
	{
		return FALSE;
	}
	VMQUERYEX VMQHelp;
	switch (mbi.State)
	{
	case MEM_FREE:
	{
					 pVMQ->lpRgnBaseAddress = mbi.BaseAddress;
					 pVMQ->dwRgnProtection = mbi.AllocationProtect;
					 pVMQ->nRgnSize = mbi.RegionSize;
					 pVMQ->dwRgnStorage = MEM_FREE;
					 pVMQ->dwRgnBlocks = 0;
					 pVMQ->dwRgnGuardBlks = 0;
					 pVMQ->bRgnIsStack = FALSE;
					 break;
	}
	case MEM_RESERVE:
	{
						pVMQ->lpRgnBaseAddress = mbi.AllocationBase;
						pVMQ->dwRgnProtection = mbi.AllocationProtect;

						GetVMBlockInfo(hProcess, lpAddress, &VMQHelp);
						pVMQ->nRgnSize = VMQHelp.nRgnSize;
						pVMQ->dwRgnStorage = VMQHelp.dwRgnStorage;
						pVMQ->dwRgnBlocks = VMQHelp.dwRgnBlocks;
						pVMQ->dwRgnGuardBlks = VMQHelp.dwRgnGuardBlks;
						pVMQ->bRgnIsStack = VMQHelp.bRgnIsStack;
						break;
	}
	case MEM_COMMIT:
	{
					   pVMQ->lpRgnBaseAddress = mbi.AllocationBase;
					   pVMQ->dwRgnProtection = mbi.AllocationProtect;
					   GetVMBlockInfo(hProcess, lpAddress, &VMQHelp);

					   pVMQ->nRgnSize = VMQHelp.nRgnSize;
					   pVMQ->dwRgnStorage = VMQHelp.dwRgnStorage;
					   pVMQ->dwRgnBlocks = VMQHelp.dwRgnBlocks;
					   pVMQ->dwRgnGuardBlks = VMQHelp.dwRgnGuardBlks;
					   pVMQ->bRgnIsStack = VMQHelp.bRgnIsStack;
					   break;
	}
	default:
		DebugBreak();
		break;
	}

	GetProtectText(pVMQ->dwRgnProtection, pVMQ->szRgnProtection, _countof(pVMQ->szRgnProtection), true);
	GetMemStorageText(pVMQ->dwRgnStorage, pVMQ->szRgnStorage, _countof(pVMQ->szRgnStorage));

	StringCchPrintf(pVMQ->szRgnBaseAddress, _countof(pVMQ->szRgnBaseAddress), L"0x%08x", pVMQ->lpRgnBaseAddress);
	StringCchPrintf(pVMQ->szRgnSize, _countof(pVMQ->szRgnSize), L"%10d", pVMQ->nRgnSize);
	StringCchPrintf(pVMQ->szRgnBlocks, _countof(pVMQ->szRgnBlocks), L"%3d", pVMQ->dwRgnBlocks);

	return TRUE;
}



