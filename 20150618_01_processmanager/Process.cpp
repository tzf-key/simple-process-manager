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
    // 1. �����ӽ��̣����ж��Ƿ�� ��
    if( !CreateProcess(NULL,lpPath,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi) ) 
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return false;
    }
    // 2. �Ƿ���Ҫ�ȴ�����ִ�н���
    if (bWait)  WaitForSingleObject( pi.hProcess, INFINITE );

    // 3. �رս��̾�����߳̾��
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
        NULL,              // Ĭ�ϰ�ȫ����
        0,                 // Ĭ�϶�ջ��С 
        ThreadProc,        // �̺߳���
        L"CreateThread",   // ����
        0,                 // Ĭ�ϴ�����־
        &dwThreadId);   // ����TID
    if (hThread == NULL) 
    {
        //ExitProcess(0);
        return false;
    }
    return true;
}




bool CProcess::GetProcessList()
{
	HANDLE         hProcessSnap;   // ���̿��վ��
	HANDLE         hProcess;       // ���̾��
	PROCESSENTRY32 stcPe32 = {0};  // ���̿�����Ϣ
	stcPe32.dwSize = sizeof( PROCESSENTRY32 );

	
    // 1. ����һ��������صĿ��վ��
	hProcessSnap = CreateToolhelp32Snapshot(
        TH32CS_SNAPPROCESS,  // ָ�����յ�����
        0 );                 // ָ������
	if( hProcessSnap == INVALID_HANDLE_VALUE )
		return false;

    // 2. ͨ�����̿��վ����ȡ��һ��������Ϣ
	if( !Process32First(
        hProcessSnap, // ���̿��վ�� 
        &stcPe32 ) )  // ���̿�����Ϣ�ṹ��
	{
		CloseHandle( hProcessSnap );
		return false;
	}

    // 3. ѭ������������Ϣ
	do
	{
		ProcessInfo stcInfo = {0};
        // 3.1 ��ȡ����ӳ��·��
		wcscpy_s(stcInfo.szProcess, stcPe32.szExeFile);

		// 3.2 ��ȡ��������ƽ̨��Ϣ�����ȼ���Ϣ
		DWORD dwPriorityClass = 0;
		hProcess = OpenProcess(
            PROCESS_QUERY_INFORMATION, // ���ʽ��̵�Ȩ��-��ѯ��Ϣ 
            FALSE,                     // �Ƿ�̳о��
            stcPe32.th32ProcessID );   // ����ID
        if( hProcess != NULL )
        {
            // a. ��ȡ�������ȼ�
            dwPriorityClass = GetPriorityClass( hProcess );
            // b. �жϽ�����32λ����64λ	
            BOOL Is32 = FALSE;
            IsWow64Process(hProcess, &Is32);
            if (Is32)
                swprintf_s(stcInfo.szProcess, L"%s %s",  stcPe32.szExeFile, L"*32");
            else
                swprintf_s(stcInfo.szProcess, L"%s %s",  stcPe32.szExeFile, L"*64");
            // c. �رվ��
            CloseHandle( hProcess );                         
        }

		// 3.3 ��ȡ���̵����������Ϣ
        stcInfo.dwPid           = stcPe32.th32ProcessID;
        stcInfo.dwThreadCount   = stcPe32.cntThreads;
		stcInfo.dwParentProcess = stcPe32.th32ParentProcessID;
		stcInfo.dwPriorityClass = dwPriorityClass;
        // 3.4 ����ȡ���������Ϣ���浽������
		m_vecProcessList.push_back(stcInfo);
	} while( Process32Next( hProcessSnap, &stcPe32 ) );

    // 4. �رվ���˳�����
	CloseHandle( hProcessSnap );
	return true;
}


bool CProcess::GetThreadList(DWORD dwPId)
{
    HANDLE        hThread     = NULL;                 // �߳̾��
	HANDLE        hThreadSnap = INVALID_HANDLE_VALUE; // �߳̿��վ��
	THREADENTRY32 stcTe32     = {0};                  // �߳̿��սṹ��
	stcTe32.dwSize = sizeof(THREADENTRY32 ); 

	// 1. ����һ���߳���صĿ��վ��
	hThreadSnap = CreateToolhelp32Snapshot(
        TH32CS_SNAPTHREAD,  // ָ�����յ�����
        0 );                // ָ������
	if( hThreadSnap == INVALID_HANDLE_VALUE ) 
		return false; 

	// 2. ͨ���߳̿��վ����ȡ��һ���߳���Ϣ
	if( !Thread32First(
        hThreadSnap, // �߳̿��վ��
        &stcTe32 ) ) // �߳̿�����Ϣ�ṹ��
	{
		CloseHandle( hThreadSnap );
		return false;
	}

    // 3. ����߳�������Ϣ����ѭ�������߳���Ϣ
    m_vecThreadList.clear();
	do 
	{ 
        // 3.1 ��ͼ���������߳����ڱ����̣���ֱ�ӱ�������Ϣ
		if( stcTe32.th32OwnerProcessID == dwPId )
			m_vecThreadList.push_back(stcTe32);
	} while( Thread32Next(hThreadSnap, &stcTe32 ) );

    // 4. �رվ�����˳�����
	CloseHandle( hThreadSnap );
	return true;
}



bool CProcess::GetModuleList(DWORD dwPId)
{
    // 1. ��ȡ����ģ������
    m_vecModuleList.clear();
    // 1.1 �򿪽���
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, dwPId);
    // 1.2 ȷ������ģ�����������С
    HMODULE hModules[0x1000] = {0}; // ģ������
    DWORD   cbNeed      = 0;   // ʵ�ʻ�ȡ�Ĵ�С
    EnumProcessModulesEx(  /* ö�ٽ���ģ�� */
        hProcess,            // ���̾��
        hModules,            // ģ��������
        sizeof(hModules),    // ģ��������Ĵ�С
        &cbNeed,             // ʵ����Ҫ�������С
        LIST_MODULES_ALL);   // ö��ģ�������
    // 1.3 ��ȡģ������
    DWORD    dwModCount = cbNeed/sizeof(HMODULE);  // ģ������
    HMODULE* pModBuf    = new HMODULE[dwModCount]; // ����ģ�����Ļ���
    EnumProcessModulesEx(  /* ö�ٽ���ģ�� */
        hProcess,                   // ���̾��
        pModBuf,                    // ģ��������
        dwModCount*sizeof(HMODULE), // ģ��������Ĵ�С
        &cbNeed,                    // ʵ����Ҫ�������С
        LIST_MODULES_ALL);          // ö��ģ�������

    // 2. ö�ٽ���ģ����Ϣ
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
    // 1. �򿪽���
	HANDLE hProcess = OpenProcess(
        PROCESS_TERMINATE, // ���ʽ��̵�Ȩ�ޣ������ʲô�������򿪽��̾���ҪʲôȨ�ޣ�
        FALSE,             // �Ƿ�̳�
        dwPid);            // ����ID
	if( hProcess==INVALID_HANDLE_VALUE )
        return false;

    // 2. ��������
    TerminateProcess(
        hProcess,  // ���̾�� 
        0);        // ���̵��˳���
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

	// 1. ��ȡĿ����̵ľ��
	if ( !(hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPid)) )
			return FALSE;
    
	// 2. ����DLL·������Ҫ���ֽ���.
	dwSize = (DWORD)(wcslen(lpszLibName)*sizeof(WCHAR) + sizeof(WCHAR));

	// 3. ��Զ���߳���Ϊ·��������ռ�.
	pszLibFileRemote = (PSTR)VirtualAllocEx(
			hProcess,         // ���̾��
			NULL,             // ��Ҫ����ĵ�ַ
			dwSize,           // ����Ĵ�С
			MEM_COMMIT,       // ������ڴ�����
			PAGE_READWRITE ); // ������ڴ��ҳ����
	if ( !pszLibFileRemote )
    {
        CloseHandle(hProcess);
		return FALSE;
    }

	// 4. ��DLL��·�������Ƶ�Զ�̽��̵��ڴ�ռ�.
    if ( !WriteProcessMemory(
           hProcess,                 // ���̾��
		   (PVOID)pszLibFileRemote,  // д���Ŀ����̵��ڴ��ַ
		   (PVOID)lpszLibName,       // ������
		   dwSize,                   // д����ֽ���
		   NULL)  )                  // ����ֵ������д����ֽ���
		return FALSE;

	// 5. ���LoadLibraryA��Kernel32.dll�е�������ַ. 
	pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(
		                           GetModuleHandle(L"Kernel32"), // ģ����
                                   "LoadLibraryW");              // ������
	if ( !pfnThreadRtn )
    {
        VirtualFreeEx(
			hProcess,                // ���̾�� 
            (PVOID)pszLibFileRemote, // ��Ҫ�ͷ�Ŀ����̵ĵ�ַ
            0,                       // �ͷŵĴ�С  MEM_RELEASE ʱ����Ϊ0
            MEM_RELEASE);            // �ͷ����� MEM_RELEASE �����ͷ� 
        CloseHandle(hProcess);
		return FALSE;
    }
	// 6. ����Զ���̣߳���ͨ��Զ���̵߳����û���DLL�ļ�. 
	hThread = CreateRemoteThread(
		hProcess,                // ���̾��
		NULL,                    // ��ȫ����
		0,                       // ջ��С
		pfnThreadRtn,            // �̻߳ص�������ַ
		(PVOID)pszLibFileRemote, // �̻߳ص���������
		0,                       // ������־
		NULL );                  // ����ֵ���߳�ID
	if (hThread == NULL)
		return FALSE;

	// 7. �ȴ�Զ���߳���ֹ
	WaitForSingleObject(hThread, INFINITE);

	// 8. �ͷ������Դ���رվ��
	if (pszLibFileRemote)  VirtualFreeEx(hProcess,(PVOID)pszLibFileRemote,0,MEM_RELEASE);
	if (hThread)           CloseHandle(hThread);
	if (hProcess)          CloseHandle(hProcess);

	return TRUE;
}


// ж��DLL
BOOL CProcess::FreeDll(DWORD dwPid, HMODULE hDll)
{
    HANDLE hProcess         = NULL;
    HANDLE hThread          = NULL;
    PTHREAD_START_ROUTINE pfnThreadRtn;

    // 1. ��ȡĿ����̵ľ��
    if ( !(hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPid)) )
        return false;

    // 2. ���FreeLibrary�����ĵ�ַ
    pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"Kernel32"), "FreeLibrary");
    if (pfnThreadRtn == NULL)
        return false;

    // 3. ����Զ���̣߳���ͨ��Զ���߳�ж��ָ����DLL�ļ�
    if ( !(hThread=CreateRemoteThread(hProcess,NULL,0,pfnThreadRtn,(PVOID)hDll,0,NULL)) )
        return false;

    // 4. �ȴ�Զ���߳���ֹ.
    WaitForSingleObject(hThread, INFINITE);

    // 5. �رվ��. 
    if ( hThread )   CloseHandle(hThread);
    if ( hProcess )  CloseHandle(hProcess);

    return true;
}

BOOL CProcess::MyFreeDll(HMODULE hDll)
{
	FreeLibrary(hDll);

	return true;
}


// �����߳�
BOOL CProcess::SuspendThread(DWORD dwTid)
{
    // 1. ���߳�
    HANDLE hThread = nullptr;
    if ( !(hThread=OpenThread(THREAD_SUSPEND_RESUME, FALSE, dwTid)) )
        return FALSE;

    // 2. �����̺߳�رվ��
    ::SuspendThread(hThread);
    CloseHandle(hThread);
    return TRUE;
}


BOOL CProcess::ResumeThread(DWORD dwTid)
{
    // 1. ���߳�
    HANDLE hThread = nullptr;
    if ( !(hThread=OpenThread(THREAD_SUSPEND_RESUME, FALSE, dwTid)) )
        return FALSE;

    // 2. �����̺߳�رվ��
    ::ResumeThread(hThread); 
    CloseHandle(hThread);
    return TRUE;
}


BOOL CProcess::KillThread(DWORD dwTid)
{
    // 1. ���߳�
    HANDLE hThread = nullptr;
    if ( !(hThread=OpenThread(THREAD_TERMINATE, FALSE, dwTid)) )
        return FALSE;

    // 2. �����̲߳��ر��߳̾��
    TerminateThread(hThread, 0); 
    CloseHandle(hThread);
    return TRUE;
}