
// 20150618_01_processmanager.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CMy20150618_01_processmanagerApp:
// 有关此类的实现，请参阅 20150618_01_processmanager.cpp
//

class CMy20150618_01_processmanagerApp : public CWinApp
{
public:
	CMy20150618_01_processmanagerApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CMy20150618_01_processmanagerApp theApp;