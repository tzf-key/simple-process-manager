#pragma once
#include <windows.h>
#include <vector>
#include <Tlhelp32.h>
using namespace std;

typedef struct _ProcessInfo
{
	DWORD   dwPid;              // 进程ID
	TCHAR   szProcess[MAXBYTE]; // 进程名
	DWORD   dwThreadCount;      // 线程数量
	DWORD   dwParentProcess;    // 父进程ID
	DWORD   dwPriorityClass;    // 优先级
}ProcessInfo, *PProcessInfo;

class CProcess
{
public:
	CProcess(void);
	~CProcess(void);

public:
    bool CreateChildProcess(LPWSTR lpPath, BOOL bWait);
    bool CreateChildThread();
	bool GetProcessList();                     // 获取进程列表
	bool GetThreadList(DWORD dwPId);           // 获取线程列表
	bool GetModuleList(DWORD dwPId);           // 获取模块列表
	bool KillProcess(DWORD dwPid);             // 杀死进程
	BOOL LoadDll(DWORD dwPid, LPTSTR pszPath); // 加载DLL到指定进程
    BOOL FreeDll(DWORD dwPid, HMODULE hDll);   // 卸载DLL
    BOOL SuspendThread(DWORD dwTid);           // 挂起线程
    BOOL ResumeThread(DWORD dwTid);            // 唤醒线程
    BOOL KillThread(DWORD dwTid);              // 杀死线程
	
	//自定义的函数
	bool IncreaseProcessPriority(DWORD dwPId, DWORD dwCurrentPriority);
	bool DecreaseProcessPriority(DWORD dwPId, DWORD dwCurrentPriority);
	BOOL MyFreeDll(HMODULE hDll);   // 卸载DLL
public:
	vector<ProcessInfo>    m_vecProcessList; // 进程列表
	vector<MODULEENTRY32>  m_vecModuleList;  // 句柄列表
	vector<THREADENTRY32>  m_vecThreadList;  // 线程列表
};

