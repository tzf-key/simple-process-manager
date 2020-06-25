
// 20150618_01_processmanagerDlg.h : 头文件
//

#pragma once
#include "mylistctrl.h"
#include "afxwin.h"


#define POINT_COUNT 100  

// CMy20150618_01_processmanagerDlg 对话框
class CMy20150618_01_processmanagerDlg : public CDialogEx
{
// 构造
public:
	CMy20150618_01_processmanagerDlg(CWnd* pParent = NULL);	// 标准构造函数
	void GetProcessListAndPaint();
	void GetThreadListAndPaint(DWORD dwPid);	
	void GetModuleListAndPaint(DWORD dwPid);
	void GetVMListAndPaint(DWORD dwPid);
	void GetCPUInfoAndPaint();
	void GetMemInfoAndPaint();
	void GetPercentageInfoAndPaint();
	void ReAssignArray(int nArray[]);
	void DrawWave(CDC *pDC, CRect &rectPicture, int nCPU1Mem2);
	void GetWindowList();
	void PaintWindowList();
	//void UnInjectDll(DWORD dwPid, WCHAR *szDllName);
// 对话框数据
	enum { IDD = IDD_MY20150618_01_PROCESSMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl m_MyListCtrl;
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	CMyListCtrl m_MyListCtrlT;
	CMyListCtrl m_MyListCtrlM;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnLvnItemchangedList4(NMHDR *pNMHDR, LRESULT *pResult);
	CMyListCtrl m_MyListCtrlVM;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnLvnItemchangedList5(NMHDR *pNMHDR, LRESULT *pResult);
	CMyListCtrl m_MyListCtrlCPUInfo;
	CMyListCtrl m_MyListCtrlMem;
	CMyListCtrl m_MyListCtrlPercentage;
	double m_Mempercentage;
	afx_msg void OnBnClickedButton4();
	int m_CPUnumber[POINT_COUNT];
	int m_MemNumber[POINT_COUNT];
	CStatic m_StaticCPUWave;
	CStatic m_StaticMemWave;
	CMyListCtrl m_MyListCtrlW;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
};
