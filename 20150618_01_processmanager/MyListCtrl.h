#pragma once


// CMyListCtrl

class CMyListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMyListCtrl)

public:
	CMyListCtrl();
	virtual ~CMyListCtrl();
	/*//AddColumn(3,
	//	L"列名A",100,
	//  L"列名B",50,
	//  L"列名C",80......)
	void  AddColumn(int nNum,...)
	{
		//设置列表框属性
		SetExtendedStyle(
			LVS_EX_FULLROWSELECT|
			LVS_EX_GRIDLINES|
			LVS_EX_CHECKBOXES);
		//初始化变参列表
		va_list vlist;
		va_start(vlist,nNum);
		//循环获取参数
		for (int i =0;i<nNum;i++)
		{
			TCHAR* pName = va_arg(vlist,TCHAR*);
			int    nWidth = va_arg(vlist,int);
			//插入一列
			InsertColumn(i,pName,NULL,nWidth);
		}
		//结束参数获取
		va_end(vlist);
	}
	//Additem(0,3,
	//	L"列名A",
	//  L"列名B",
	//  L"列名C",......)
	void Additem(int nItemNum,
		int nCount,...)
	{
		// 初始化
		va_list vlist;
		va_start(vlist,nCount);
		// 添加一行，并设置第一个单元格内容
		TCHAR* pText = 
			va_arg(vlist,TCHAR*);
		InsertItem(nItemNum,pText);
		for (int i=1;i<nCount;i++)
		{
			//设置本行其他单元格内容
			pText = 
				va_arg(vlist,TCHAR*);
			SetItemText(nItemNum,
				i,pText);
		}
		//结束获取参数
		va_end(vlist);
	}*/
protected:
	DECLARE_MESSAGE_MAP()
};


