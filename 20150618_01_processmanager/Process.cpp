#include "stdafx.h"
#include "Process.h"
#include <stdio.h>
#include <Tlhelp32.h>
#include <Psapi.h>
#pragma comment (lib, "psapi.lib")


CProcess::CProcess(void)
{
}


CProcess::~CProcess(void)
{
}


bool CProcess::CreateChildProcess(LPWSTR lpPath, BOOL bWait)
{
    STARTUPINFO         si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);
    // 1. 创建子进程，并判断是否成 功
    if( !CreateProcess(NULL,lpPath,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi) ) 
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return false;
    }
    // 2. 是否需要等待进程执行结束
    if (bWait)  WaitForSingleObject( pi.hProcess, INFINITE );

    // 3. 关闭进程句柄和线程句柄
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
    return true;
}

bool CProcess::IncreaseProcessPriority(DWORD dwPId, DWORD dwCurrentPriority)
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, dwPId);
	switch (dwCurrentPriority)
	{
	case 0x0:
		SetPriorityClass(hProcess, 0x20);
		return true;
		break;
	case 0x20:
		SetPriorityClass(hProcess, 0x40);
		return true;
		break;
	case 0x40:
		SetPriorityClass(hProcess, 0x80);
		return true;
		break;
	case 0x80:
		SetPriorityClass(hProcess, 0x100);
		return true;
		break;
	case 0x100:
		SetPriorityClass(hProcess, 0x4000);
		return true;
		break;
	case 0x4000:
		SetPriorityClass(hProcess, 0x8000);
		return true;
		break;
	case 0x8000:
		SetPriorityClass(hProcess, 0x100000);
		return true;
		break;
	case 0x100000:
		SetPriorityClass(hProcess, 0x200000);
		return true;
		break;
	case 0x200000:
		SetPriorityClass(hProcess, 0x200000);
		return true;
		break;
	default:
		SetPriorityClass(hProcess, 0x0);
		return true;
		break;
	}
	return true;
}

bool CProcess::DecreaseProcessPriority(DWORD dwPId, DWORD dwCurrentPriority)
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, dwPId);
	switch (dwCurrentPriority)
	{
	case 0x0:
		SetPriorityClass(hProcess, 0x0);
		return true;
		break;
	case 0x20:
		SetPriorityClass(hProcess, 0x0);
		return true;
		break;
	case 0x40:
		SetPriorityClass(hProcess, 0x20);
		return true;
		break;
	case 0x80:
		SetPriorityClass(hProcess, 0x40);
		return true;
		break;
	case 0x100:
		SetPriorityClass(hProcess, 0x80);
		return true;
		break;
	case 0x4000:
		SetPriorityClass(hProcess, 0x100);
		return true;
		break;
	case 0x8000:
		SetPriorityClass(hProcess, 0x4000);
		return true;
		break;
	case 0x100000:
		SetPriorityClass(hProcess, 0x8000);
		return true;
		break;
	case 0x200000:
		SetPriorityClass(hProcess, 0x100000);
		return true;
		break;
	default:
		SetPriorityClass(hProcess, 0x0);
		return true;
		break;
	}
	return true;
}

DWORD WINAPI ThreadProc( LPVOID lpParam ) 
{ 
    MessageBox(NULL,(LPCWSTR)lpParam,L"CreateThread",MB_OK);
    return 0; 
} 
bool CreateChildThread()
{
    DWORD  dwThreadId = 0;
    HANDLE hThread    = CreateThread( 
        NULL,              // 默认安全属性
        0,                 // 默认堆栈大小 
        ThreadProc,        // 线程函数
        L"CreateThread",   // 参数
        0,                 // 默认创建标志
        &dwThreadId);   // 返回TID
    if (hThread == NULL) 
    {
        //ExitProcess(0);
        return false;
    }
    return true;
}




bool CProcess::GetProcessList()
{
	HANDLE         hProcessSnap;   // 进程快照句柄
	HANDLE         hProcess;       // 进程句柄
	PROCESSENTRY32 stcPe32 = {0};  // 进程快照信息
	stcPe32.dwSize = sizeof( PROCESSENTRY32 );

	
    // 1. 创建一个进程相关的快照句柄
	hProcessSnap = CreateToolhelp32Snapshot(
        TH32CS_SNAPPROCESS,  // 指定快照的类型
        0 );                 // 指定进程
	if( hProcessSnap == INVALID_HANDLE_VALUE )
		return false;

    // 2. 通过进程快照句柄获取第一个进程信息
	if( !Process32First(
        hProcessSnap, // 进程快照句柄 
        &stcPe32 ) )  // 进程快照信息结构体
	{
		CloseHandle( hProcessSnap );
		return false;
	}

    // 3. 循环遍历进程信息
	do
	{
		ProcessInfo stcInfo = {0};
        // 3.1 获取进程映像路径
		wcscpy_s(stcInfo.szProcess, stcPe32.szExeFile);

		// 3.2 获取进程运行平台信息及优先级信息
		DWORD dwPriorityClass = 0;
		hProcess = OpenProcess(
            PROCESS_QUERY_INFORMATION, // 访问进程的权限-查询信息 
            FALSE,                     // 是否继承句柄
            stcPe32.th32ProcessID );   // 进程ID
        if( hProcess != NULL )
        {
            // a. 获取进程优先级
            dwPriorityClass = GetPriorityClass( hProcess );
            // b. 判断进程是32位还是64位	
            BOOL Is32 = FALSE;
            IsWow64Process(hProcess, &Is32);
            if (Is32)
                swprintf_s(stcInfo.szProcess, L"%s %s",  stcPe32.szExeFile, L"*32");
            else
                swprintf_s(stcInfo.szProcess, L"%s %s",  stcPe32.szExeFile, L"*64");
            // c. 关闭句柄
            CloseHandle( hProcess );                         
        }

		// 3.3 获取进程的其他相关信息
        stcInfo.dwPid           = stcPe32.th32ProcessID;
        stcInfo.dwThreadCount   = stcPe32.cntThreads;
		stcInfo.dwParentProcess = stcPe32.th32ParentProcessID;
		stcInfo.dwPriorityClass = dwPriorityClass;
        // 3.4 将获取到的相关信息保存到向量中
		m_vecProcessList.push_back(stcInfo);
	} while( Process32Next( hProcessSnap, &stcPe32 ) );

    // 4. 关闭句柄退出函数
	CloseHandle( hProcessSnap );
	return true;
}


bool CProcess::GetThreadList(DWORD dwPId)
{
    HANDLE        hThread     = NULL;                 // 线程句柄
	HANDLE        hThreadSnap = INVALID_HANDLE_VALUE; // 线程快照句柄
	THREADENTRY32 stcTe32     = {0};                  // 线程快照结构体
	stcTe32.dwSize = sizeof(THREADENTRY32 ); 

	// 1. 创建一个线程相关的快照句柄
	hThreadSnap = CreateToolhelp32Snapshot(
        TH32CS_SNAPTHREAD,  // 指定快照的类型
        0 );                // 指定进程
	if( hThreadSnap == INVALID_HANDLE_VALUE ) 
		return false; 

	// 2. 通过线程快照句柄获取第一个线程信息
	if( !Thread32First(
        hThreadSnap, // 线程快照句柄
        &stcTe32 ) ) // 线程快照信息结构体
	{
		CloseHandle( hThreadSnap );
		return false;
	}

    // 3. 清除线程向量信息，并循环遍历线程信息
    m_vecThreadList.clear();
	do 
	{ 
        // 3.1 如图遍历到的线程属于本进程，则直接保存其信息
		if( stcTe32.th32OwnerProcessID == dwPId )
			m_vecThreadList.push_back(stcTe32);
	} while( Thread32Next(hThreadSnap, &stcTe32 ) );

    // 4. 关闭句柄并退出函数
	CloseHandle( hThreadSnap );
	return true;
}



bool CProcess::GetModuleList(DWORD dwPId)
{
    // 1. 获取进程模块句柄表
    m_vecModuleList.clear();
    // 1.1 打开进程
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, dwPId);
    // 1.2 确定保存模块句柄的数组大小
    HMODULE hModules[0x1000] = {0}; // 模块数组
    DWORD   cbNeed      = 0;   // 实际获取的大小
    EnumProcessModulesEx(  /* 枚举进程模块 */
        hProcess,            // 进程句柄
        hModules,            // 模块句柄数组
        sizeof(hModules),    // 模块句柄数组的大小
        &cbNeed,             // 实际需要的数组大小
        LIST_MODULES_ALL);   // 枚举模块的类型
    // 1.3 获取模块句柄表
    DWORD    dwModCount = cbNeed/sizeof(HMODULE);  // 模块数量
    HMODULE* pModBuf    = new HMODULE[dwModCount]; // 保存模块句柄的缓存
    EnumProcessModulesEx(  /* 枚举进程模块 */
        hProcess,                   // 进程句柄
        pModBuf,                    // 模块句柄数组
        dwModCount*sizeof(HMODULE), // 模块句柄数组的大小
        &cbNeed,                    // 实际需要的数组大小
        LIST_MODULES_ALL);          // 枚举模块的类型

    // 2. 枚举进程模块信息
    char          szModuleName[0x200] = { 0 };
    MODULEENTRY32 stcMod32 = { sizeof(MODULEENTRY32) };
    MODULEINFO	  stcModInfo;
    for (UINT i = 0; i < dwModCount; i++)
    {
        GetModuleFileNameEx(hProcess, pModBuf[i], stcMod32.szExePath, MAX_PATH);
        GetModuleInformation(hProcess, pModBuf[i], &stcModInfo, sizeof(MODULEINFO));
        stcMod32.modBaseAddr   = (PBYTE)stcModInfo.lpBaseOfDll;
        stcMod32.modBaseSize   = stcModInfo.SizeOfImage;
        stcMod32.th32ProcessID = dwPId;
        m_vecModuleList.push_back(stcMod32);
    }


	CloseHandle( hProcess );
	return true;
}

//UnhookWindowsHookEx 
//FreeLibrary
bool CProcess::KillProcess(DWORD dwPid)
{
    // 1. 打开进程
	HANDLE hProcess = OpenProcess(
        PROCESS_TERMINATE, // 访问进程的权限（句柄做什么操作，打开进程就需要什么权限）
        FALSE,             // 是否继承
        dwPid);            // 进程ID
	if( hProcess==INVALID_HANDLE_VALUE )
        return false;

    // 2. 结束进程
    TerminateProcess(
        hProcess,  // 进程句柄 
        0);        // 进程的退出码
	CloseHandle(hProcess); 

	return true;
}


BOOL CProcess::LoadDll(DWORD dwPid, LPTSTR lpszLibName)
{
	HANDLE hProcess         = NULL;
	HANDLE hThread          = NULL;
	PSTR   pszLibFileRemote = NULL;
	DWORD  dwSize = 0;
	PTHREAD_START_ROUTINE pfnThreadRtn;

	// 1. 获取目标进程的句柄
	if ( !(hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPid)) )
			return FALSE;
    
	// 2. 计算DLL路径名需要的字节数.
	dwSize = (DWORD)(wcslen(lpszLibName)*sizeof(WCHAR) + sizeof(WCHAR));

	// 3. 在远程线程中为路径名分配空间.
	pszLibFileRemote = (PSTR)VirtualAllocEx(
			hProcess,         // 进程句柄
			NULL,             // 想要申请的地址
			dwSize,           // 申请的大小
			MEM_COMMIT,       // 申请的内存类型
			PAGE_READWRITE ); // 申请的内存分页类型
	if ( !pszLibFileRemote )
    {
        CloseHandle(hProcess);
		return FALSE;
    }

	// 4. 将DLL的路径名复制到远程进程的内存空间.
    if ( !WriteProcessMemory(
           hProcess,                 // 进程句柄
		   (PVOID)pszLibFileRemote,  // 写入的目标进程的内存地址
		   (PVOID)lpszLibName,       // 缓冲区
		   dwSize,                   // 写入的字节数
		   NULL)  )                  // 传出值，返回写入的字节数
		return FALSE;

	// 5. 获得LoadLibraryA在Kernel32.dll中的真正地址. 
	pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(
		                           GetModuleHandle(L"Kernel32"), // 模块句柄
                                   "LoadLibraryW");              // 函数名
	if ( !pfnThreadRtn )
    {
        VirtualFreeEx(
			hProcess,                // 进程句柄 
            (PVOID)pszLibFileRemote, // 需要释放目标进程的地址
            0,                       // 释放的大小  MEM_RELEASE 时必须为0
            MEM_RELEASE);            // 释放类型 MEM_RELEASE 彻底释放 
        CloseHandle(hProcess);
		return FALSE;
    }
	// 6. 创建远程线程，并通过远程线程调用用户的DLL文件. 
	hThread = CreateRemoteThread(
		hProcess,                // 进程句柄
		NULL,                    // 安全类型
		0,                       // 栈大小
		pfnThreadRtn,            // 线程回调函数地址
		(PVOID)pszLibFileRemote, // 线程回调函数参数
		0,                       // 创建标志
		NULL );                  // 传出值，线程ID
	if (hThread == NULL)
		return FALSE;

	// 7. 等待远程线程终止
	WaitForSingleObject(hThread, INFINITE);

	// 8. 释放相关资源并关闭句柄
	if (pszLibFileRemote)  VirtualFreeEx(hProcess,(PVOID)pszLibFileRemote,0,MEM_RELEASE);
	if (hThread)           CloseHandle(hThread);
	if (hProcess)          CloseHandle(hProcess);

	return TRUE;
}


// 卸载DLL
BOOL CProcess::FreeDll(DWORD dwPid, HMODULE hDll)
{
    HANDLE hProcess         = NULL;
    HANDLE hThread          = NULL;
    PTHREAD_START_ROUTINE pfnThreadRtn;

    // 1. 获取目标进程的句柄
    if ( !(hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPid)) )
        return false;

    // 2. 获得FreeLibrary函数的地址
    pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"Kernel32"), "FreeLibrary");
    if (pfnThreadRtn == NULL)
        return false;

    // 3. 创建远程线程，并通过远程线程卸载指定的DLL文件
    if ( !(hThread=CreateRemoteThread(hProcess,NULL,0,pfnThreadRtn,(PVOID)hDll,0,NULL)) )
        return false;

    // 4. 等待远程线程终止.
    WaitForSingleObject(hThread, INFINITE);

    // 5. 关闭句柄. 
    if ( hThread )   CloseHandle(hThread);
    if ( hProcess )  CloseHandle(hProcess);

    return true;
}

BOOL CProcess::MyFreeDll(HMODULE hDll)
{
	FreeLibrary(hDll);

	return true;
}


// 挂起线程
BOOL CProcess::SuspendThread(DWORD dwTid)
{
    // 1. 打开线程
    HANDLE hThread = nullptr;
    if ( !(hThread=OpenThread(THREAD_SUSPEND_RESUME, FALSE, dwTid)) )
        return FALSE;

    // 2. 挂起线程后关闭句柄
    ::SuspendThread(hThread);
    CloseHandle(hThread);
    return TRUE;
}


BOOL CProcess::ResumeThread(DWORD dwTid)
{
    // 1. 打开线程
    HANDLE hThread = nullptr;
    if ( !(hThread=OpenThread(THREAD_SUSPEND_RESUME, FALSE, dwTid)) )
        return FALSE;

    // 2. 唤醒线程后关闭句柄
    ::ResumeThread(hThread); 
    CloseHandle(hThread);
    return TRUE;
}


BOOL CProcess::KillThread(DWORD dwTid)
{
    // 1. 打开线程
    HANDLE hThread = nullptr;
    if ( !(hThread=OpenThread(THREAD_TERMINATE, FALSE, dwTid)) )
        return FALSE;

    // 2. 结束线程并关闭线程句柄
    TerminateThread(hThread, 0); 
    CloseHandle(hThread);
    return TRUE;
}