#pragma once
#include <windows.h>
#include <vector>
#include <Tlhelp32.h>
using namespace std;

typedef struct _ProcessInfo
{
	DWORD   dwPid;              // ����ID
	TCHAR   szProcess[MAXBYTE]; // ������
	DWORD   dwThreadCount;      // �߳�����
	DWORD   dwParentProcess;    // ������ID
	DWORD   dwPriorityClass;    // ���ȼ�
}ProcessInfo, *PProcessInfo;

class CProcess
{
public:
	CProcess(void);
	~CProcess(void);

public:
    bool CreateChildProcess(LPWSTR lpPath, BOOL bWait);
    bool CreateChildThread();
	bool GetProcessList();                     // ��ȡ�����б�
	bool GetThreadList(DWORD dwPId);           // ��ȡ�߳��б�
	bool GetModuleList(DWORD dwPId);           // ��ȡģ���б�
	bool KillProcess(DWORD dwPid);             // ɱ������
	BOOL LoadDll(DWORD dwPid, LPTSTR pszPath); // ����DLL��ָ������
    BOOL FreeDll(DWORD dwPid, HMODULE hDll);   // ж��DLL
    BOOL SuspendThread(DWORD dwTid);           // �����߳�
    BOOL ResumeThread(DWORD dwTid);            // �����߳�
    BOOL KillThread(DWORD dwTid);              // ɱ���߳�
	
	//�Զ���ĺ���
	bool IncreaseProcessPriority(DWORD dwPId, DWORD dwCurrentPriority);
	bool DecreaseProcessPriority(DWORD dwPId, DWORD dwCurrentPriority);
	BOOL MyFreeDll(HMODULE hDll);   // ж��DLL
public:
	vector<ProcessInfo>    m_vecProcessList; // �����б�
	vector<MODULEENTRY32>  m_vecModuleList;  // ����б�
	vector<THREADENTRY32>  m_vecThreadList;  // �߳��б�
};

