#pragma once


// CMyListCtrl

class CMyListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMyListCtrl)

public:
	CMyListCtrl();
	virtual ~CMyListCtrl();
	/*//AddColumn(3,
	//	L"����A",100,
	//  L"����B",50,
	//  L"����C",80......)
	void  AddColumn(int nNum,...)
	{
		//�����б������
		SetExtendedStyle(
			LVS_EX_FULLROWSELECT|
			LVS_EX_GRIDLINES|
			LVS_EX_CHECKBOXES);
		//��ʼ������б�
		va_list vlist;
		va_start(vlist,nNum);
		//ѭ����ȡ����
		for (int i =0;i<nNum;i++)
		{
			TCHAR* pName = va_arg(vlist,TCHAR*);
			int    nWidth = va_arg(vlist,int);
			//����һ��
			InsertColumn(i,pName,NULL,nWidth);
		}
		//����������ȡ
		va_end(vlist);
	}
	//Additem(0,3,
	//	L"����A",
	//  L"����B",
	//  L"����C",......)
	void Additem(int nItemNum,
		int nCount,...)
	{
		// ��ʼ��
		va_list vlist;
		va_start(vlist,nCount);
		// ���һ�У������õ�һ����Ԫ������
		TCHAR* pText = 
			va_arg(vlist,TCHAR*);
		InsertItem(nItemNum,pText);
		for (int i=1;i<nCount;i++)
		{
			//���ñ���������Ԫ������
			pText = 
				va_arg(vlist,TCHAR*);
			SetItemText(nItemNum,
				i,pText);
		}
		//������ȡ����
		va_end(vlist);
	}*/
protected:
	DECLARE_MESSAGE_MAP()
};


