
// 20150618_01_processmanagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "20150618_01_processmanager.h"
#include "20150618_01_processmanagerDlg.h"
#include "afxdialogex.h"
//#include <windows.h>
#include <string>
#include "Process.h"
#include "VMQuery.h"
#include <Psapi.h>
#include "ComsumptionCal.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

vector<LPCTSTR> g_vecWindowInfo;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMy20150618_01_processmanagerDlg 对话框



CMy20150618_01_processmanagerDlg::CMy20150618_01_processmanagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy20150618_01_processmanagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	memset(m_CPUnumber, 0, sizeof(int) * POINT_COUNT); 
	memset(m_MemNumber, 0, sizeof(int) * POINT_COUNT);
}

void CMy20150618_01_processmanagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_MyListCtrl);
	DDX_Control(pDX, IDC_LIST3, m_MyListCtrlT);
	DDX_Control(pDX, IDC_LIST2, m_MyListCtrlM);
	DDX_Control(pDX, IDC_LIST4, m_MyListCtrlVM);
	DDX_Control(pDX, IDC_LIST5, m_MyListCtrlCPUInfo);
	DDX_Control(pDX, IDC_LIST6, m_MyListCtrlMem);
	DDX_Control(pDX, IDC_LIST7, m_MyListCtrlPercentage);
	DDX_Control(pDX, IDC_CPU_Wave, m_StaticCPUWave);
	DDX_Control(pDX, IDC_Mem_Wave, m_StaticMemWave);
	DDX_Control(pDX, IDC_LIST8, m_MyListCtrlW);
}

BEGIN_MESSAGE_MAP(CMy20150618_01_processmanagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMy20150618_01_processmanagerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CMy20150618_01_processmanagerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CMy20150618_01_processmanagerDlg::OnBnClickedButton3)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST4, &CMy20150618_01_processmanagerDlg::OnLvnItemchangedList4)
	ON_BN_CLICKED(IDC_BUTTON2, &CMy20150618_01_processmanagerDlg::OnBnClickedButton2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST5, &CMy20150618_01_processmanagerDlg::OnLvnItemchangedList5)
	ON_BN_CLICKED(IDC_BUTTON4, &CMy20150618_01_processmanagerDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMy20150618_01_processmanagerDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMy20150618_01_processmanagerDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CMy20150618_01_processmanagerDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CMy20150618_01_processmanagerDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CMy20150618_01_processmanagerDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CMy20150618_01_processmanagerDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CMy20150618_01_processmanagerDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CMy20150618_01_processmanagerDlg::OnBnClickedButton12)
END_MESSAGE_MAP()

/*
bool showVM(DWORD dwProcessID)
{
	CVMQuery objVM;
	HANDLE   hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessID);
	PVOID    lpAddress = NULL;
	VMQUERY  stcVMQ = { 0 };
	while (objVM.GetVMinfo(hProcess, lpAddress, &stcVMQ))
	{
		wprintf(L"%s %s %s %s %s\n",
			stcVMQ.szRgnBaseAddress,
			stcVMQ.szRgnStorage,
			stcVMQ.szRgnSize,
			stcVMQ.szRgnBlocks,
			stcVMQ.szRgnProtection);

		lpAddress = ((PBYTE)stcVMQ.lpRgnBaseAddress + stcVMQ.nRgnSize);
		ZeroMemory(&stcVMQ, sizeof(VMQUERY));
	}

	CloseHandle(hProcess);
	return true;
}*/

void CMy20150618_01_processmanagerDlg::GetVMListAndPaint(DWORD dwPid)
{
	CVMQuery objVM;
	HANDLE   hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);
	PVOID    lpAddress = NULL;
	VMQUERY  stcVMQ = { 0 };
	int i = 0;
	m_MyListCtrlVM.DeleteAllItems();
	while (objVM.GetVMinfo(hProcess, lpAddress, &stcVMQ))
	{
		/*wprintf(L"%s %s %s %s %s\n",
			stcVMQ.szRgnBaseAddress,
			stcVMQ.szRgnStorage,
			stcVMQ.szRgnSize,
			stcVMQ.szRgnBlocks,
			stcVMQ.szRgnProtection);*/

		int nRow = m_MyListCtrlVM.InsertItem(i, 
			(LPCTSTR)stcVMQ.szRgnBaseAddress);
		m_MyListCtrlVM.SetItemText(nRow, 1, (LPCTSTR)stcVMQ.szRgnStorage);
		m_MyListCtrlVM.SetItemText(nRow, 2, (LPCTSTR)stcVMQ.szRgnSize);
		m_MyListCtrlVM.SetItemText(nRow, 3, (LPCTSTR)stcVMQ.szRgnBlocks);
		m_MyListCtrlVM.SetItemText(nRow, 4, (LPCTSTR)stcVMQ.szRgnProtection);

		lpAddress = ((PBYTE)stcVMQ.lpRgnBaseAddress + stcVMQ.nRgnSize);
		ZeroMemory(&stcVMQ, sizeof(VMQUERY));
		i++;
	}

	CloseHandle(hProcess);
}

BOOL CALLBACK EnumWindowsProc(
	_In_  HWND hwnd,
	_In_  LPARAM lParam
	)
{
	int cTxtLen;
	PSTR pszMem;
	// 窗口是否可视
	if (!IsWindowVisible(hwnd))
	{
		return TRUE;
	}

	// 窗口是否可激活
	if (!IsWindowEnabled(hwnd))
	{
		return TRUE;
	}
	cTxtLen = GetWindowTextLength(hwnd); 
	pszMem = (PSTR) VirtualAlloc((LPVOID) NULL, 
		(DWORD) (cTxtLen + 1), MEM_COMMIT, 
		PAGE_READWRITE); 
	GetWindowText(hwnd, (LPWSTR)pszMem, 
		cTxtLen + 1); 

	/*LPTSTR lpszTemp = L"HELLO";
	GetWindowText(hwnd, lpszTemp, MAX_PATH);*/
	/*CMy20150618_01_processmanagerDlg pWnd = 
		(CMy20150618_01_processmanagerDlg)CWnd::FromHandle(hWnd);*/

	if (pszMem != NULL && *pszMem != '\0')
	{
		g_vecWindowInfo.push_back((LPCTSTR)pszMem);
		//::MessageBox(NULL, (LPCTSTR)pszMem, NULL, MB_OK);
	}
	
	return TRUE;
}

void CMy20150618_01_processmanagerDlg::PaintWindowList()
{
	m_MyListCtrlW.DeleteAllItems();
	for (int i = 0; i < g_vecWindowInfo.size(); i++)
	{
		int nRow = m_MyListCtrlW.InsertItem(i, g_vecWindowInfo[i]);
	}
}

vector<CString> SplitCString(CString strSource, CString ch)
{
	vector <CString> vecString;
	int iPos = 0;
	CString strTmp;
	strTmp = strSource.Tokenize(ch,iPos);
	while(strTmp.Trim() != _T(""))
	{
		vecString.push_back(strTmp);
		strTmp = strSource.Tokenize(ch,iPos);
	}
	return vecString;
}

void CMy20150618_01_processmanagerDlg::GetModuleListAndPaint(DWORD dwPid)
{
	CString str;
	LPCTSTR pStr;
	CProcess obj;
	CString char1 = _T("\\");
	vector<CString> vecSzTemp;
	obj.m_vecModuleList.clear();
	obj.GetModuleList(dwPid);
	m_MyListCtrlM.DeleteAllItems();
	for (int i = 0; i < obj.m_vecModuleList.size(); i++)
	{
		/*int nRow = m_MyListCtrlM.InsertItem(i, 
		(LPCTSTR)obj.m_vecModuleList[i].szExePath);*/
		vecSzTemp = SplitCString((CString)obj.m_vecModuleList[i].szExePath, 
			char1);
		int nRow = m_MyListCtrlM.InsertItem(i, vecSzTemp[vecSzTemp.size() - 1]);
			
		m_MyListCtrlM.SetItemText(nRow, 1, 
			(LPCTSTR)obj.m_vecModuleList[i].szExePath);

		str.Format(_T("0x%x"),obj.m_vecModuleList[i].modBaseAddr);
		pStr = LPCTSTR(str);
		m_MyListCtrlM.SetItemText(nRow, 2, pStr);

		str.Format(_T("%d"),obj.m_vecModuleList[i].th32ProcessID);
		pStr = LPCTSTR(str);
		m_MyListCtrlM.SetItemText(nRow, 3, pStr);

		str.Format(_T("%d"),obj.m_vecModuleList[i].dwSize);
		pStr = LPCTSTR(str);
		m_MyListCtrlM.SetItemText(nRow, 4, pStr);

		
		/*str.Format(_T("%d"),obj.m_vecProcessList[i].dwPriorityClass);
		pStr = LPCTSTR(str);
		m_MyListCtrlT.SetItemText(nRow, 4, pStr);*/
	}
}

void CMy20150618_01_processmanagerDlg::GetMemInfoAndPaint()
{
	CString str;
	LPCTSTR pStr;
	CConsumptionCal objConsum;
	m_MyListCtrlMem.DeleteAllItems();
	PERFORMANCE_INFORMATION mMemInfo;
	GetPerformanceInfo(&mMemInfo, sizeof(mMemInfo));

	m_MyListCtrlMem.InsertItem(0, _T("进程"));
	str.Format(_T("%u"),mMemInfo.ProcessCount);
	pStr = LPCTSTR(str);
	m_MyListCtrlMem.SetItemText(0, 1, pStr);

	m_MyListCtrlMem.InsertItem(1, _T("线程"));
	str.Format(_T("%u"),mMemInfo.ThreadCount);
	pStr = LPCTSTR(str);
	m_MyListCtrlMem.SetItemText(1, 1, pStr);

	m_MyListCtrlMem.InsertItem(2, _T("句柄"));
	str.Format(_T("%u"),mMemInfo.HandleCount);
	pStr = LPCTSTR(str);
	m_MyListCtrlMem.SetItemText(2, 1, pStr);

	m_MyListCtrlMem.InsertItem(3, _T("总物理内存"));
	str.Format(_T("%.2fGB"), (double)objConsum.GetVitualMemoryTotal() / 
		(1024 * 1024 * 1024));
	pStr = LPCTSTR(str);
	m_MyListCtrlMem.SetItemText(3, 1, pStr);

	m_MyListCtrlMem.InsertItem(4, _T("已用物理内存"));
	str.Format(_T("%.2fGB"), (double)objConsum.GetVitualMemoryUsed() / 
		(1024 * 1024 * 1024));
	pStr = LPCTSTR(str);
	m_MyListCtrlMem.SetItemText(4, 1, pStr);

	m_MyListCtrlMem.InsertItem(5, _T("SystemCache"));
	str.Format(_T("%u"),mMemInfo.SystemCache);
	pStr = LPCTSTR(str);
	m_MyListCtrlMem.SetItemText(5, 1, pStr);

	m_MyListCtrlMem.InsertItem(6, _T("CommitTotal"));
	str.Format(_T("%u"),mMemInfo.CommitTotal);
	pStr = LPCTSTR(str);
	m_MyListCtrlMem.SetItemText(6, 1, pStr);

	m_MyListCtrlMem.InsertItem(7, _T("CommitLimit"));
	str.Format(_T("%u"),mMemInfo.CommitLimit);
	pStr = LPCTSTR(str);
	m_MyListCtrlMem.SetItemText(7, 1, pStr);

	m_MyListCtrlMem.InsertItem(8, _T("KernelTotal"));
	str.Format(_T("%u"),mMemInfo.KernelTotal);
	pStr = LPCTSTR(str);
	m_MyListCtrlMem.SetItemText(8, 1, pStr);

	m_MyListCtrlMem.InsertItem(9, _T("KernelPaged"));
	str.Format(_T("%u"),mMemInfo.KernelPaged);
	pStr = LPCTSTR(str);
	m_MyListCtrlMem.SetItemText(9, 1, pStr);

	m_MyListCtrlMem.InsertItem(10, _T("PageSize"));
	str.Format(_T("%u"),mMemInfo.PageSize);
	pStr = LPCTSTR(str);
	m_MyListCtrlMem.SetItemText(10, 1, pStr);

	m_Mempercentage = (double)objConsum.GetVitualMemoryUsed() / 
		(double)objConsum.GetVitualMemoryTotal();
	
}

void CMy20150618_01_processmanagerDlg::GetCPUInfoAndPaint()
{
	CString str;
	LPCTSTR pStr;
	SYSTEM_INFO siSysInfo;
	m_MyListCtrlCPUInfo.DeleteAllItems();
	// Copy the hardware information to the SYSTEM_INFO structure. 
	GetSystemInfo(&siSysInfo); 
	// Display the contents of the SYSTEM_INFO structure. 
	m_MyListCtrlCPUInfo.InsertItem(0, _T("OEM ID"));
	str.Format(_T("%u"),siSysInfo.dwOemId);
	pStr = LPCTSTR(str);
	m_MyListCtrlCPUInfo.SetItemText(0, 1, pStr);

	m_MyListCtrlCPUInfo.InsertItem(1, _T("处理器数量"));
	str.Format(_T("%u"),siSysInfo.dwNumberOfProcessors);
	pStr = LPCTSTR(str);
	m_MyListCtrlCPUInfo.SetItemText(1, 1, pStr);

	m_MyListCtrlCPUInfo.InsertItem(2, _T("Page size"));
	str.Format(_T("%u"),siSysInfo.dwPageSize);
	pStr = LPCTSTR(str);
	m_MyListCtrlCPUInfo.SetItemText(2, 1, pStr);

	m_MyListCtrlCPUInfo.InsertItem(3, _T("处理器型号"));
	str.Format(_T("%u"),siSysInfo.dwProcessorType);
	pStr = LPCTSTR(str);
	m_MyListCtrlCPUInfo.SetItemText(3, 1, pStr);

	m_MyListCtrlCPUInfo.InsertItem(4, _T("最小地址"));
	str.Format(_T("%lx"),siSysInfo.lpMinimumApplicationAddress);
	pStr = LPCTSTR(str);
	m_MyListCtrlCPUInfo.SetItemText(4, 1, pStr);

	m_MyListCtrlCPUInfo.InsertItem(5, _T("最大地址"));
	str.Format(_T("%lx"),siSysInfo.lpMaximumApplicationAddress);
	pStr = LPCTSTR(str);
	m_MyListCtrlCPUInfo.SetItemText(5, 1, pStr);

	m_MyListCtrlCPUInfo.InsertItem(6, _T("mask"));
	str.Format(_T("%u"),siSysInfo.dwActiveProcessorMask);
	pStr = LPCTSTR(str);
	m_MyListCtrlCPUInfo.SetItemText(6, 1, pStr);

}

void CMy20150618_01_processmanagerDlg::GetPercentageInfoAndPaint()
{
	CString str;
	LPCTSTR pStr;
	char chTemp;
	m_MyListCtrlPercentage.DeleteAllItems();
	CConsumptionCal objConsum;
	m_MyListCtrlPercentage.InsertItem(0, _T("内存"));
	str.Format(_T("%.2f%%"), m_Mempercentage * 100);
	pStr = LPCTSTR(str);
	m_MyListCtrlPercentage.SetItemText(0, 1, pStr);

	objConsum.GetSystemTimesAddress();
	m_MyListCtrlPercentage.InsertItem(1, _T("CPU"));
	chTemp = objConsum.GetCPUUsage();
	str.Format(_T("%2d%%"), chTemp);
	pStr = LPCTSTR(str);
	m_MyListCtrlPercentage.SetItemText(1, 1, pStr);

	m_CPUnumber[POINT_COUNT - 1] = (int)chTemp;
	m_MemNumber[POINT_COUNT - 1] = (int)(m_Mempercentage * 100);
}


void CMy20150618_01_processmanagerDlg::DrawWave(CDC *pDC, CRect &rectPicture, 
												int nCPU1Mem2)   
{   
	float fDeltaX;     // x轴相邻两个绘图点的坐标距离   
	float fDeltaY;     // y轴每个逻辑单位对应的坐标值   
	int nX;      // 在连线时用于存储绘图点的横坐标   
	int nY;      // 在连线时用于存储绘图点的纵坐标   
	CPen newPen;       // 用于创建新画笔   
	CPen *pOldPen;     // 用于存放旧画笔   
	CBrush newBrush;   // 用于创建新画刷   
	CBrush *pOldBrush; // 用于存放旧画刷   

	// 计算fDeltaX和fDeltaY   
	fDeltaX = (float)rectPicture.Width() / (POINT_COUNT - 1);   
	fDeltaY = (float)rectPicture.Height() / 100;   

	// 创建黑色新画刷   
	newBrush.CreateSolidBrush(RGB(0,0,0));   
	// 选择新画刷，并将旧画刷的指针保存到pOldBrush   
	pOldBrush = pDC->SelectObject(&newBrush);   
	// 以黑色画刷为绘图控件填充黑色，形成黑色背景   
	pDC->Rectangle(rectPicture);   
	// 恢复旧画刷   
	pDC->SelectObject(pOldBrush);   
	// 删除新画刷   
	newBrush.DeleteObject();   

	// 创建实心画笔，粗度为1，颜色为绿色   
	newPen.CreatePen(PS_SOLID, 1, RGB(0,255,0));   
	// 选择新画笔，并将旧画笔的指针保存到pOldPen   
	pOldPen = pDC->SelectObject(&newPen);   

	// 将当前点移动到绘图控件窗口的左下角，以此为波形的起始点   
	pDC->MoveTo(rectPicture.left, rectPicture.bottom);   
	// 计算m_nzValues数组中每个点对应的坐标位置，并依次连接，最终形成曲线   
	for (int i=0; i<POINT_COUNT; i++)   
	{   
		nX = rectPicture.left + (int)(i * fDeltaX);   
		if (nCPU1Mem2 == 1)
		{
			nY = rectPicture.bottom - (int)(m_CPUnumber[i] * fDeltaY);
		}
		else
		{
			nY = rectPicture.bottom - (int)(m_MemNumber[i] * fDeltaY);
		}
		   
		pDC->LineTo(nX, nY);   
	}   

	// 恢复旧画笔   
	pDC->SelectObject(pOldPen);   
	// 删除新画笔   
	newPen.DeleteObject();   
}  

void CMy20150618_01_processmanagerDlg::GetThreadListAndPaint(DWORD dwPid)
{
	CString str;
	LPCTSTR pStr;
	CProcess obj;
	obj.m_vecThreadList.clear();
	obj.GetThreadList(dwPid);
	m_MyListCtrlT.DeleteAllItems();
	for (int i = 0; i < obj.m_vecThreadList.size(); i++)
	{
		str.Format(_T("%d"),obj.m_vecThreadList[i].th32ThreadID);
		pStr = LPCTSTR(str);
		int nRow = m_MyListCtrlT.InsertItem(i, pStr);

		str.Format(_T("%d"),obj.m_vecThreadList[i].th32OwnerProcessID);
		pStr = LPCTSTR(str);
		m_MyListCtrlT.SetItemText(nRow, 1, pStr);

		str.Format(_T("%d"),obj.m_vecThreadList[i].tpBasePri);
		pStr = LPCTSTR(str);
		m_MyListCtrlT.SetItemText(nRow, 2, pStr);

		/*str.Format(_T("%d"),obj.m_vecProcessList[i].dwParentProcess);
		pStr = LPCTSTR(str);
		m_MyListCtrlT.SetItemText(nRow, 3, pStr);

		str.Format(_T("%d"),obj.m_vecProcessList[i].dwPriorityClass);
		pStr = LPCTSTR(str);
		m_MyListCtrlT.SetItemText(nRow, 4, pStr);*/
	}
}

void CMy20150618_01_processmanagerDlg::GetProcessListAndPaint()
{
	CString str;
	LPCTSTR pStr;
	CProcess obj;
	obj.m_vecProcessList.clear();
	obj.GetProcessList();
	m_MyListCtrl.DeleteAllItems();
	for (int i = 0; i < obj.m_vecProcessList.size(); i++)
	{
		int nRow = m_MyListCtrl.InsertItem(i, 
			LPCTSTR(obj.m_vecProcessList[i].szProcess));

		str.Format(_T("%d"),obj.m_vecProcessList[i].dwPid);
		pStr = LPCTSTR(str);
		m_MyListCtrl.SetItemText(nRow, 1, pStr);

		str.Format(_T("%d"),obj.m_vecProcessList[i].dwThreadCount);
		pStr = LPCTSTR(str);
		m_MyListCtrl.SetItemText(nRow, 2, pStr);

		str.Format(_T("%d"),obj.m_vecProcessList[i].dwParentProcess);
		pStr = LPCTSTR(str);
		m_MyListCtrl.SetItemText(nRow, 3, pStr);

		str.Format(_T("%d"),obj.m_vecProcessList[i].dwPriorityClass);
		pStr = LPCTSTR(str);
		m_MyListCtrl.SetItemText(nRow, 4, pStr);

		switch (obj.m_vecProcessList[i].dwPriorityClass)
		{
		case 0x0:
			m_MyListCtrl.SetItemText(nRow, 5, _T("最低级别"));
			break;
		case 0x20:
			m_MyListCtrl.SetItemText(nRow, 5, _T("NORMAL_PRIORITY_CLASS"));
			break;
		case 0x40:
			m_MyListCtrl.SetItemText(nRow, 5, _T("IDLE_PRIORITY_CLASS"));
			break;
		case 0x80:
			m_MyListCtrl.SetItemText(nRow, 5, _T("HIGH_PRIORITY_CLASS"));
			break;
		case 0x100:
			m_MyListCtrl.SetItemText(nRow, 5, _T("REALTIME_PRIORITY_CLASS"));
			break;
		case 0x4000:
			m_MyListCtrl.SetItemText(nRow, 5, _T("BELOW_NORMAL_PRIORITY_CLASS"));
			break;
		case 0x8000:
			m_MyListCtrl.SetItemText(nRow, 5, _T("ABOVE_NORMAL_PRIORITY_CLASS"));
			break;
		case 0x100000:
			m_MyListCtrl.SetItemText(nRow, 5, _T("PROCESS_MODE_BACKGROUND_BEGIN"));
			break;
		case 0x200000:
			m_MyListCtrl.SetItemText(nRow, 5, _T("PROCESS_MODE_BACKGROUND_END"));
			break;
		default:
			m_MyListCtrl.SetItemText(nRow, 5, _T("Custom"));
			break;
		}
	}
}

void CALLBACK MyTimerProc(
	HWND hWnd,      // handle of CWnd that called SetTimer
	UINT nMsg,      // WM_TIMER
	UINT_PTR nIDEvent,   // timer identification
	DWORD dwTime    // system time
	)
{
	CMy20150618_01_processmanagerDlg *pWnd = 
		(CMy20150618_01_processmanagerDlg *)CWnd::FromHandle(hWnd); 
	pWnd->GetMemInfoAndPaint();
	pWnd->GetPercentageInfoAndPaint();
	pWnd->ReAssignArray(pWnd->m_CPUnumber);
	pWnd->ReAssignArray(pWnd->m_MemNumber);

	CRect rectPicture; 
	// 获取绘图控件的客户区坐标   
	// （客户区坐标以窗口的左上角为原点，这区别于以屏幕左上角为原点的屏幕坐标)
	pWnd->m_StaticCPUWave.GetClientRect(&rectPicture);
	pWnd->DrawWave(pWnd->m_StaticCPUWave.GetDC(), rectPicture, 1);   

	CRect rectPicture2; 
	pWnd->m_StaticMemWave.GetClientRect(&rectPicture2);
	pWnd->DrawWave(pWnd->m_StaticMemWave.GetDC(), rectPicture2, 2);

}

void CMy20150618_01_processmanagerDlg::GetWindowList()
{
	g_vecWindowInfo.clear();
	EnumWindows(EnumWindowsProc, NULL);
}

void CMy20150618_01_processmanagerDlg::ReAssignArray(int nArray[])
{
	for (int i = 1; i < POINT_COUNT; i++)
	{
		nArray[i - 1] = nArray[i];
	}
}

// CMy20150618_01_processmanagerDlg 消息处理程序

BOOL CMy20150618_01_processmanagerDlg::OnInitDialog()
{
	

	CDialogEx::OnInitDialog();
	
	DWORD dwStyle = m_MyListCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_SHOWSELALWAYS;
	m_MyListCtrl.SetExtendedStyle(dwStyle); //设置扩展风格

	m_MyListCtrl.InsertColumn(0,_T("名称"),LVCFMT_LEFT,100);
	m_MyListCtrl.InsertColumn(1,_T("PID"),LVCFMT_LEFT,70);
	m_MyListCtrl.InsertColumn(2,_T("线程数量"),LVCFMT_LEFT,70);
	m_MyListCtrl.InsertColumn(3,_T("父进程ID"),LVCFMT_LEFT,70);
	m_MyListCtrl.InsertColumn(4,_T("优先级"),LVCFMT_LEFT,70);
	m_MyListCtrl.InsertColumn(5,_T("优先级解释"),LVCFMT_LEFT,100);

	m_MyListCtrlT.SetExtendedStyle(dwStyle); //设置扩展风格

	/*stcTe32.cntUsage
			stcTe32.dwFlags
			stcTe32.dwSize
			stcTe32.th32OwnerProcessID
			stcTe32.th32ThreadID
			stcTe32.tpBasePri
			stcTe32.tpDeltaPri*/

	m_MyListCtrlT.InsertColumn(0,_T("TID"),LVCFMT_LEFT,70);
	m_MyListCtrlT.InsertColumn(1,_T("PID"),LVCFMT_LEFT,70);
	m_MyListCtrlT.InsertColumn(2,_T("优先级"),LVCFMT_LEFT,70);
	m_MyListCtrlT.InsertColumn(3,_T("预留"),LVCFMT_LEFT,70);
	m_MyListCtrlT.InsertColumn(4,_T("预留"),LVCFMT_LEFT,70);
	m_MyListCtrlT.InsertColumn(5,_T("预留"),LVCFMT_LEFT,100);

	m_MyListCtrlM.SetExtendedStyle(dwStyle); //设置扩展风格

	m_MyListCtrlM.InsertColumn(0,_T("名称"),LVCFMT_LEFT,170);
	m_MyListCtrlM.InsertColumn(1,_T("路径"),LVCFMT_LEFT,70);
	m_MyListCtrlM.InsertColumn(2,_T("基地址"),LVCFMT_LEFT,70);
	m_MyListCtrlM.InsertColumn(3,_T("PID"),LVCFMT_LEFT,70);
	m_MyListCtrlM.InsertColumn(4,_T("大小"),LVCFMT_LEFT,70);
	m_MyListCtrlM.InsertColumn(5,_T("预留"),LVCFMT_LEFT,100);

	m_MyListCtrlVM.SetExtendedStyle(dwStyle); //设置扩展风格

	m_MyListCtrlVM.InsertColumn(0,_T("基地址"),LVCFMT_LEFT,70);
	m_MyListCtrlVM.InsertColumn(1,_T("Storage"),LVCFMT_LEFT,70);
	m_MyListCtrlVM.InsertColumn(2,_T("Size"),LVCFMT_LEFT,70);
	m_MyListCtrlVM.InsertColumn(3,_T("Blocks"),LVCFMT_LEFT,70);
	m_MyListCtrlVM.InsertColumn(4,_T("Protection"),LVCFMT_LEFT,70);
	m_MyListCtrlVM.InsertColumn(5,_T("预留"),LVCFMT_LEFT,100);

	m_MyListCtrlCPUInfo.SetExtendedStyle(dwStyle);
	m_MyListCtrlCPUInfo.InsertColumn(0,_T("名称"),LVCFMT_LEFT,70);
	m_MyListCtrlCPUInfo.InsertColumn(1,_T("内容"),LVCFMT_LEFT,50);

	m_MyListCtrlMem.SetExtendedStyle(dwStyle);
	m_MyListCtrlMem.InsertColumn(0,_T("名称"),LVCFMT_LEFT,100);
	m_MyListCtrlMem.InsertColumn(1,_T("内容"),LVCFMT_LEFT,100);

	m_MyListCtrlPercentage.SetExtendedStyle(dwStyle);
	m_MyListCtrlPercentage.InsertColumn(0,_T("设备名称"),LVCFMT_LEFT,70);
	m_MyListCtrlPercentage.InsertColumn(1,_T("使用率"),LVCFMT_LEFT,70);

	m_MyListCtrlW.SetExtendedStyle(dwStyle);
	m_MyListCtrlW.InsertColumn(0,_T("窗口名称"),LVCFMT_LEFT,115);
	m_MyListCtrlW.InsertColumn(1,_T("预留"),LVCFMT_LEFT,70);

	GetProcessListAndPaint();
	GetCPUInfoAndPaint();
	GetMemInfoAndPaint();
	GetPercentageInfoAndPaint();
	GetWindowList();
	PaintWindowList();
	SetTimer(2, 1000, MyTimerProc);
	
	//初始化用于画图的数据
	for (int i = 0; i < 11; i++)
	{
		m_CPUnumber[i] = 20;
	}
	

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}


	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}



void CMy20150618_01_processmanagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy20150618_01_processmanagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMy20150618_01_processmanagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CMy20150618_01_processmanagerDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

//查看线程
void CMy20150618_01_processmanagerDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString szTemp;
	char * pstr;
	size_t sz=0;
	int n = 0;
	for(int i=0; i<m_MyListCtrl.GetItemCount(); i++)
	{
		if( m_MyListCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		{
			szTemp = m_MyListCtrl.GetItemText(i, 1);
			const size_t strsize=(szTemp.GetLength()+1)*2; // 宽字符的长度;
			pstr= new char[strsize]; //分配空间;
			sz=0;
			wcstombs_s(&sz,pstr,strsize,szTemp,_TRUNCATE);
			n=atoi((const char*)pstr); // 字符串已经由原来的CString 转换成了 const char*
			GetThreadListAndPaint(n);
		}
	}
}


void CAboutDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

//查看模块
void CMy20150618_01_processmanagerDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString szTemp;
	char * pstr;
	size_t sz=0;
	int n = 0;
	for(int i=0; i<m_MyListCtrl.GetItemCount(); i++)
	{
		if( m_MyListCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		{
			szTemp = m_MyListCtrl.GetItemText(i, 1);
			const size_t strsize=(szTemp.GetLength()+1)*2; // 宽字符的长度;
			pstr= new char[strsize]; //分配空间;
			sz=0;
			wcstombs_s(&sz,pstr,strsize,szTemp,_TRUNCATE);
			n=atoi((const char*)pstr); // 字符串已经由原来的CString 转换成了 const char*
			GetModuleListAndPaint(n);
		}
	}
}


void CMy20150618_01_processmanagerDlg::OnLvnItemchangedList4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

//查看虚拟内存
void CMy20150618_01_processmanagerDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString szTemp;
	char * pstr;
	size_t sz=0;
	int n = 0;
	for(int i=0; i<m_MyListCtrl.GetItemCount(); i++)
	{
		if( m_MyListCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		{
			szTemp = m_MyListCtrl.GetItemText(i, 1);
			const size_t strsize=(szTemp.GetLength()+1)*2; // 宽字符的长度;
			pstr= new char[strsize]; //分配空间;
			sz=0;
			wcstombs_s(&sz,pstr,strsize,szTemp,_TRUNCATE);
			n=atoi((const char*)pstr); // 字符串已经由原来的CString 转换成了 const char*
			GetVMListAndPaint(n);
		}
	}
}


void CMy20150618_01_processmanagerDlg::OnLvnItemchangedList5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

//手动刷新进程信息
void CMy20150618_01_processmanagerDlg::OnBnClickedButton4()
{
	GetProcessListAndPaint();
	// TODO: 在此添加控件通知处理程序代码
}

//一键查看线程、虚拟内存、模块信息
void CMy20150618_01_processmanagerDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton1();
	OnBnClickedButton2();
	OnBnClickedButton3();
}

//结束进程
void CMy20150618_01_processmanagerDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CString szTemp;
	char * pstr;
	size_t sz=0;
	int n = 0;
	CProcess obj;
	for(int i=0; i<m_MyListCtrl.GetItemCount(); i++)
	{
		if( m_MyListCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		{
			szTemp = m_MyListCtrl.GetItemText(i, 1);
			const size_t strsize=(szTemp.GetLength()+1)*2; // 宽字符的长度;
			pstr= new char[strsize]; //分配空间;
			sz=0;
			wcstombs_s(&sz,pstr,strsize,szTemp,_TRUNCATE);
			n=atoi((const char*)pstr); // 字符串已经由原来的CString 转换成了 const char*
			obj.KillProcess(n);
		}
	}
	GetProcessListAndPaint();
}

//结束线程
void CMy20150618_01_processmanagerDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	CString szTemp;
	char * pstr;
	size_t sz=0;
	int n = 0;
	int nPid = 0;
	CProcess obj;
	for(int i=0; i<m_MyListCtrlT.GetItemCount(); i++)
	{
		if( m_MyListCtrlT.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		{
			//获取线程的TID
			szTemp = m_MyListCtrlT.GetItemText(i, 0);
			const size_t strsize=(szTemp.GetLength()+1)*2; // 宽字符的长度;
			pstr= new char[strsize]; //分配空间;
			sz=0;
			wcstombs_s(&sz,pstr,strsize,szTemp,_TRUNCATE);
			n=atoi((const char*)pstr); // 字符串已经由原来的CString 转换成了 const char*

			//获取PID，用于刷新线程列表
			szTemp = m_MyListCtrlT.GetItemText(i, 1);
			const size_t strsize2=(szTemp.GetLength()+1)*2; // 宽字符的长度;
			pstr= new char[strsize2]; //分配空间;
			sz=0;
			wcstombs_s(&sz,pstr,strsize2,szTemp,_TRUNCATE);
			nPid=atoi((const char*)pstr); // 字符串已经由原来的CString 转换成了 const char*

			obj.KillThread(n);
		}
	}
	GetThreadListAndPaint(nPid);
}

//挂起线程
void CMy20150618_01_processmanagerDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	CString szTemp;
	char * pstr;
	size_t sz=0;
	int n = 0;
	int nPid = 0;
	CProcess obj;
	for(int i=0; i<m_MyListCtrlT.GetItemCount(); i++)
	{
		if( m_MyListCtrlT.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		{
			//获取线程的TID
			szTemp = m_MyListCtrlT.GetItemText(i, 0);
			const size_t strsize=(szTemp.GetLength()+1)*2; // 宽字符的长度;
			pstr= new char[strsize]; //分配空间;
			sz=0;
			wcstombs_s(&sz,pstr,strsize,szTemp,_TRUNCATE);
			n=atoi((const char*)pstr); // 字符串已经由原来的CString 转换成了 const char*

			//获取PID，用于刷新线程列表
			szTemp = m_MyListCtrlT.GetItemText(i, 1);
			const size_t strsize2=(szTemp.GetLength()+1)*2; // 宽字符的长度;
			pstr= new char[strsize2]; //分配空间;
			sz=0;
			wcstombs_s(&sz,pstr,strsize2,szTemp,_TRUNCATE);
			nPid=atoi((const char*)pstr); // 字符串已经由原来的CString 转换成了 const char*

			obj.SuspendThread(n);
		}
	}
	GetThreadListAndPaint(nPid);
}

//恢复线程
void CMy20150618_01_processmanagerDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	CString szTemp;
	char * pstr;
	size_t sz=0;
	int n = 0;
	int nPid = 0;
	CProcess obj;
	for(int i=0; i<m_MyListCtrlT.GetItemCount(); i++)
	{
		if( m_MyListCtrlT.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		{
			//获取线程的TID
			szTemp = m_MyListCtrlT.GetItemText(i, 0);
			const size_t strsize=(szTemp.GetLength()+1)*2; // 宽字符的长度;
			pstr= new char[strsize]; //分配空间;
			sz=0;
			wcstombs_s(&sz,pstr,strsize,szTemp,_TRUNCATE);
			n=atoi((const char*)pstr); // 字符串已经由原来的CString 转换成了 const char*

			//获取PID，用于刷新线程列表
			szTemp = m_MyListCtrlT.GetItemText(i, 1);
			const size_t strsize2=(szTemp.GetLength()+1)*2; // 宽字符的长度;
			pstr= new char[strsize2]; //分配空间;
			sz=0;
			wcstombs_s(&sz,pstr,strsize2,szTemp,_TRUNCATE);
			nPid=atoi((const char*)pstr); // 字符串已经由原来的CString 转换成了 const char*

			obj.ResumeThread(n);
		}
	}
	GetThreadListAndPaint(nPid);
}

//提升优先级
void CMy20150618_01_processmanagerDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	CString szTemp;
	char * pstr;
	size_t sz=0;
	int n = 0;
	int nPriority;
	CProcess obj;
	for(int i=0; i<m_MyListCtrl.GetItemCount(); i++)
	{
		if( m_MyListCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		{
			szTemp = m_MyListCtrl.GetItemText(i, 1);
			const size_t strsize=(szTemp.GetLength()+1)*2; // 宽字符的长度;
			pstr= new char[strsize]; //分配空间;
			sz=0;
			wcstombs_s(&sz,pstr,strsize,szTemp,_TRUNCATE);
			n=atoi((const char*)pstr); // 字符串已经由原来的CString 转换成了 const char*

			szTemp = m_MyListCtrl.GetItemText(i, 4);
			const size_t strsize2=(szTemp.GetLength()+1)*2; // 宽字符的长度;
			pstr= new char[strsize2]; //分配空间;
			sz=0;
			wcstombs_s(&sz,pstr,strsize2,szTemp,_TRUNCATE);
			nPriority=atoi((const char*)pstr); // 字符串已经由原来的CString 转换成了 const char*

			obj.IncreaseProcessPriority(n, nPriority);
		}
	}
	GetProcessListAndPaint();
}

//降低优先级
void CMy20150618_01_processmanagerDlg::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
	CString szTemp;
	char * pstr;
	size_t sz=0;
	int n = 0;
	int nPriority;
	CProcess obj;
	for(int i=0; i<m_MyListCtrl.GetItemCount(); i++)
	{
		if( m_MyListCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		{
			szTemp = m_MyListCtrl.GetItemText(i, 1);
			const size_t strsize=(szTemp.GetLength()+1)*2; // 宽字符的长度;
			pstr= new char[strsize]; //分配空间;
			sz=0;
			wcstombs_s(&sz,pstr,strsize,szTemp,_TRUNCATE);
			n=atoi((const char*)pstr); // 字符串已经由原来的CString 转换成了 const char*

			szTemp = m_MyListCtrl.GetItemText(i, 4);
			const size_t strsize2=(szTemp.GetLength()+1)*2; // 宽字符的长度;
			pstr= new char[strsize2]; //分配空间;
			sz=0;
			wcstombs_s(&sz,pstr,strsize2,szTemp,_TRUNCATE);
			nPriority=atoi((const char*)pstr); // 字符串已经由原来的CString 转换成了 const char*

			obj.DecreaseProcessPriority(n, nPriority);
		}
	}
	GetProcessListAndPaint();
}

/*
bool UnInjectDll(const TCHAR* ptszDllFile, DWORD dwProcessId)    
{    
	if (NULL == ptszDllFile || 0 == ::_tcslen(ptszDllFile))    
	{    
		return false;    
	}    
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;    
	HANDLE hProcess = NULL;    
	HANDLE hThread = NULL;    
	hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);    
	if (INVALID_HANDLE_VALUE == hModuleSnap)    
	{    
		return false;    
	}    
	MODULEENTRY32 me32;    
	memset(&me32, 0, sizeof(MODULEENTRY32));    
	me32.dwSize = sizeof(MODULEENTRY32);    
	if(FALSE == ::Module32First(hModuleSnap, &me32))    
	{    
		::CloseHandle(hModuleSnap);    
		return false;    
	}    
	bool isFound = false;    
	do    
	{    
		isFound = (0 == ::_tcsicmp(me32.szModule, ptszDllFile) || 0 == ::_tcsicmp(me32.szExePath, ptszDllFile));    
		if (isFound)   
		{    
			break;    
		}    
	} while (TRUE == ::Module32Next(hModuleSnap, &me32));    
	::CloseHandle(hModuleSnap);    
	if (false == isFound)    
	{    
		return false;    
	}    
	hProcess = ::OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION, FALSE, dwProcessId);    
	if (NULL == hProcess)    
	{    
		return false;    
	}    
	LPTHREAD_START_ROUTINE lpThreadFun = (PTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandle(_T("Kernel32")), "FreeLibrary");    
	if (NULL == lpThreadFun)    
	{    
		::CloseHandle(hProcess);    
		return false;    
	}    
	hThread = ::CreateRemoteThread(hProcess, NULL, 0, lpThreadFun, me32.modBaseAddr , 0, NULL);    
	if (NULL == hThread)    
	{    
		::CloseHandle(hProcess);    
		return false;    
	}    
	::WaitForSingleObject(hThread, INFINITE);    
	::CloseHandle(hThread);    
	::CloseHandle(hProcess);    
	return true;    
}  
*/

/*
void CMy20150618_01_processmanagerDlg::UnInjectDll(DWORD dwPid, WCHAR *szDllName)
{
	BOOL flag = FALSE;
	if ( dwPid == 0 || wcslen(szDllName) == 0 )
	{
		return;
	}
	//获取系统运行进程、线程等的列表
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	MODULEENTRY32 Me32 = { 0 };
	Me32.dwSize = sizeof(MODULEENTRY32);
	//检索与进程相关联的第一个模块的信息
	BOOL bRet = Module32First(hSnap, &Me32);
	while ( bRet )
	{
		//查找所注入的DLL
		if ( wcscmp(Me32.szModule, szDllName) == 0 )
		{
			flag = TRUE;
			break;
		}
		//检索下一个模块信息
		bRet = Module32Next(hSnap, &Me32);
	}
	if (flag == FALSE)
	{
		::MessageBox(0,L"找不到相应的模块!",0,0);
		return;
	}

	CloseHandle(hSnap);

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if ( hProcess == NULL )
	{
		::MessageBox(0,L"进程打开失败!",0,0);

		return ;
	}

	FARPROC pFunAddr = GetProcAddress(GetModuleHandle(L"kernel32.dll"),"FreeLibrary");

	HANDLE hThread = CreateRemoteThread(hProcess,
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)pFunAddr,
		Me32.hModule,
		0,
		NULL);
	if (hThread == NULL)
	{
		::MessageBox(0,L"创建远程线程失败!",0,0);
		return;
	}
	::MessageBox(0,L"成功卸载!",0,0);
	//等待线程退出
	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	CloseHandle(hProcess);
}
*/


//卸载模块
void CMy20150618_01_processmanagerDlg::OnBnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码
	CString szTemp;
	//WCHAR * pstr;
	char * pstr2;
	size_t sz=0;
	int n = 0;
	int nPid = 0;
	CProcess obj;
	for(int i=0; i<m_MyListCtrlM.GetItemCount(); i++)
	{
		if( m_MyListCtrlM.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		{
			//获取线程的TID
			szTemp = m_MyListCtrlM.GetItemText(i, 1);
			/*const size_t strsize=(szTemp.GetLength()+1)*2; // 宽字符的长度;
			pstr= new WCHAR[strsize]; //分配空间;
			sz=0;
			wcstombs_s(&sz,pstr,strsize,szTemp,_TRUNCATE);*/
			/*CStringW strWide = CT2CW(szTemp); // 将T字符转换成宽字符
			WCHAR *buf = strWide.GetBuffer(100); // 获取CString内部缓存，并保证缓存长度是STR_LEN*/

			//n=atoi((const char*)pstr); // 字符串已经由原来的CString 转换成了 const char*

			//获取PID，用于刷新线程列表
			szTemp = m_MyListCtrlM.GetItemText(i, 3);
			const size_t strsize2=(szTemp.GetLength()+1)*2; // 宽字符的长度;
			pstr2= new char[strsize2]; //分配空间;
			sz=0;
			wcstombs_s(&sz,pstr2,strsize2,szTemp,_TRUNCATE);
			nPid=atoi((const char*)pstr2); // 字符串已经由原来的CString 转换成了 const char*

			//obj.FreeDll(nPid, GetModuleHandle((LPCTSTR)szTemp));
			obj.MyFreeDll(GetModuleHandle((LPCTSTR)szTemp));
			//UnInjectDll((const TCHAR*)pstr, nPid);
			//UnInjectDll(nPid, buf);
		}
	}
	GetModuleListAndPaint(nPid);
}
 