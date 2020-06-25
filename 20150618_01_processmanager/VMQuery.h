#pragma once
#include <windows.h>
#include <vector>

using std::vector;

typedef struct _VMQUERYEX
{
	int nRgnSize;
	//预订区域大小

	DWORD dwRgnStorage;
	//预划分区域的屋里存储器类型（MEM_*: Free,Image,Mapped,Private

	DWORD dwRgnBlocks;
	//区域中块的数量

	DWORD dwRgnGuardBlks;
	//具有Page_Guard保护属性的块的数量，如果>0,则此区域是为线程栈而预订的

	BOOL bRgnIsStack;
	//此区域是否包含线程栈，是则此区域是为线程栈而预订的

}VMQUERYEX;


typedef struct _VMQUERY
{
	//区域信息
	PVOID lpRgnBaseAddress;
	//预划分内存区域的其实地址

	DWORD dwRgnProtection;
	//预划分此内存区域时的原始保护属性(PAGE_ *

	int nRgnSize;
	//与划分区域大小

	DWORD dwRgnStorage;
	//预划分区域的屋里存储器类型(MEM_*: free, image,mapped,private

	DWORD dwRgnBlocks;
	//预划分区域中块的数量

	DWORD dwRgnGuardBlks;
	//具有PAGE_Guard保护属性的块的数量，如果>0，则此区域是为线程栈而预订的

	BOOL bRgnIsStack;
	//此区域是否包含线程栈，是则此区域是为线程栈而预订的

	//块信息
	PVOID lpBlkBaseAddress;
	//内存块的其实地址

	DWORD dwBlkProtection;
	//内存块的保护属性(PAGE_*

	int nBlkSize;
	//内存块的大小

	DWORD dwBlkStorage;
	//内存块的存储类型(MEM_*: free,image,mapped,private,reserve

	//文本信息
	BOOL bIsExpandInfo;
	//此条信息是否为区域展开后的块信息

	WCHAR szRgnBaseAddress[10];
	//[文本]预划分内存区域的其实地址

	WCHAR szRgnProtection[16];
	//[文本]原始保护属性

	WCHAR szRgnSize[16];
	//[文本]区域大小

	WCHAR szRgnStorage[16];
	//[文本]物理存储器类型

	WCHAR szRgnBlocks[8];
	//[文本]块的数量

	WCHAR szRgnGuardBlks[8];
	//[文本]具有page_guard保护属性的块的数量

	WCHAR szBlkBaseAddress[10];
	//[文本]内存块的起始地址

	WCHAR szBlkProtection[16];
	//[文本]内存块的保护属性

	WCHAR szBlkSize[16];
	//[文本]内存块的大小

	WCHAR szBlkStorage[16];
	//[文本]内存块的存储类型
}VMQUERY, *PVMQUERY;


class CVMQuery
{
public:
	CVMQuery();
	~CVMQuery();
	BOOL GetVMinfo(HANDLE hProcess, LPCVOID lpAddress, PVMQUERY pVMQ);

protected:
	BOOL GetVMBlockInfo(HANDLE hProcess, LPCVOID lpAddress, VMQUERYEX *pVMBlock);
	void GetProtectText(DWORD dwProtect, PTSTR szBuf, int nSize, BOOL bShowFlags);
	void GetMemStorageText(DWORD dwStorage, PTSTR szBuf, int nSize);

private:
	vector<VMQUERY> m_vecVMINFOlist;
};

