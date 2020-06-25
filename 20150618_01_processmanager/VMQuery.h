#pragma once
#include <windows.h>
#include <vector>

using std::vector;

typedef struct _VMQUERYEX
{
	int nRgnSize;
	//Ԥ�������С

	DWORD dwRgnStorage;
	//Ԥ�������������洢�����ͣ�MEM_*: Free,Image,Mapped,Private

	DWORD dwRgnBlocks;
	//�����п������

	DWORD dwRgnGuardBlks;
	//����Page_Guard�������ԵĿ�����������>0,���������Ϊ�߳�ջ��Ԥ����

	BOOL bRgnIsStack;
	//�������Ƿ�����߳�ջ�������������Ϊ�߳�ջ��Ԥ����

}VMQUERYEX;


typedef struct _VMQUERY
{
	//������Ϣ
	PVOID lpRgnBaseAddress;
	//Ԥ�����ڴ��������ʵ��ַ

	DWORD dwRgnProtection;
	//Ԥ���ִ��ڴ�����ʱ��ԭʼ��������(PAGE_ *

	int nRgnSize;
	//�뻮�������С

	DWORD dwRgnStorage;
	//Ԥ�������������洢������(MEM_*: free, image,mapped,private

	DWORD dwRgnBlocks;
	//Ԥ���������п������

	DWORD dwRgnGuardBlks;
	//����PAGE_Guard�������ԵĿ�����������>0�����������Ϊ�߳�ջ��Ԥ����

	BOOL bRgnIsStack;
	//�������Ƿ�����߳�ջ�������������Ϊ�߳�ջ��Ԥ����

	//����Ϣ
	PVOID lpBlkBaseAddress;
	//�ڴ�����ʵ��ַ

	DWORD dwBlkProtection;
	//�ڴ��ı�������(PAGE_*

	int nBlkSize;
	//�ڴ��Ĵ�С

	DWORD dwBlkStorage;
	//�ڴ��Ĵ洢����(MEM_*: free,image,mapped,private,reserve

	//�ı���Ϣ
	BOOL bIsExpandInfo;
	//������Ϣ�Ƿ�Ϊ����չ����Ŀ���Ϣ

	WCHAR szRgnBaseAddress[10];
	//[�ı�]Ԥ�����ڴ��������ʵ��ַ

	WCHAR szRgnProtection[16];
	//[�ı�]ԭʼ��������

	WCHAR szRgnSize[16];
	//[�ı�]�����С

	WCHAR szRgnStorage[16];
	//[�ı�]����洢������

	WCHAR szRgnBlocks[8];
	//[�ı�]�������

	WCHAR szRgnGuardBlks[8];
	//[�ı�]����page_guard�������ԵĿ������

	WCHAR szBlkBaseAddress[10];
	//[�ı�]�ڴ�����ʼ��ַ

	WCHAR szBlkProtection[16];
	//[�ı�]�ڴ��ı�������

	WCHAR szBlkSize[16];
	//[�ı�]�ڴ��Ĵ�С

	WCHAR szBlkStorage[16];
	//[�ı�]�ڴ��Ĵ洢����
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

